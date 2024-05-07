#define LEAF_INCLUDE_PRIVATE
#include <arch/cpu/cpu.h>
#include <arch/x86_64/idt/idt.h>
#include <arch/x86_64/pic/pic.h>
#include <libc/stdio/printf.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/leaf.h>
#include <sys/logger.h>

#define IDT_ENTRIES 256

idt_entry_t idt[IDT_ENTRIES];
idt_pointer_t idt_p;
void *irq_handlers[16];
extern void *last_rbp;

extern uint64_t isr_tbl[];

static const char *exception_strings[32] = {"Division By Zero",
                                            "Debug",
                                            "Nonmaskable Interrupt",
                                            "Breakpoint",
                                            "Overflow",
                                            "Bound Range Exceeded",
                                            "Invalid Opcode",
                                            "Device Not Available",
                                            "Double Fault",
                                            "Coprocessor Segment Overrun",
                                            "Invalid TSS",
                                            "Segment Not Present",
                                            "Stack Segment Fault",
                                            "General Protection Fault",
                                            "Page Fault",
                                            "Reserved",
                                            "x87 FPU Error"
                                            "Alignment Check",
                                            "Machine Check",
                                            "Simd Exception",
                                            "Virtualization Exception",
                                            "Control Protection Exception",
                                            "Reserved",
                                            "Reserved",
                                            "Reserved",
                                            "Reserved",
                                            "Reserved",
                                            "Reserved",
                                            "Hypervisor Injection Exception",
                                            "VMM Communication Exception",
                                            "Security Exception",
                                            "Reserved"};

static const char *exception_descriptions[32] = {
    "Division By Zero: Attempted to divide by zero, typically due to a "
    "programming error or uninitialized variable.",
    "Debug: Debugging breakpoint or trap triggered for debugging purposes.",
    "Nonmaskable Interrupt: Critical hardware interrupt that cannot be "
    "ignored, often signaling a hardware failure or urgent system event.",
    "Breakpoint: Execution reached a breakpoint instruction, commonly used for "
    "debugging and tracing.",
    "Overflow: Arithmetic operation resulted in a value exceeding the maximum "
    "limit of the data type.",
    "Bound Range Exceeded: Attempted to access an array or data structure with "
    "an out-of-range index.",
    "Invalid Opcode: Encountered an unrecognized or invalid machine "
    "instruction, possibly due to incompatible CPU architecture or software "
    "corruption.",
    "Device Not Available: Required hardware device is unavailable or "
    "inaccessible.",
    "Double Fault: Occurs when handling an exception triggers another "
    "exception, indicating severe system instability.",
    "Coprocessor Segment Overrun: Coprocessor accessed data outside its "
    "allowed segment boundaries.",
    "Invalid TSS: Task State Segment (TSS) is invalid or corrupted, often due "
    "to memory corruption or misconfiguration.",
    "Segment Not Present: Access to a memory segment that is not currently "
    "loaded into physical memory.",
    "Stack Segment Fault: Stack-related fault such as stack overflow (exceeded "
    "stack size) or underflow (empty stack).",
    "General Protection Fault: Violation of memory access rules or privilege "
    "levels, commonly due to invalid memory access or stack corruption.",
    "Page Fault: Access to a memory page that is not mapped to physical "
    "memory, requiring page fault handling (e.g., page swapping).",
    "Reserved: Unspecified or reserved exception type, may indicate "
    "hardware-specific faults or undefined behavior.",
    "x87 FPU Error: Floating Point Unit (FPU) error during arithmetic "
    "operation, often due to invalid floating-point calculations.",
    "Alignment Check: Memory access misalignment, violating CPU alignment "
    "requirements.",
    "Machine Check: Internal hardware error detected by the CPU or chipset, "
    "indicating possible hardware failure.",
    "Simd Exception: SIMD (Single Instruction, Multiple Data) instruction "
    "error, typically related to vectorized data processing.",
    "Virtualization Exception: Error related to virtualization support, such "
    "as invalid virtual machine configuration or hypervisor issues.",
    "Control Protection Exception: Attempt to modify a protected control "
    "register or execute privileged instructions without sufficient "
    "privileges.",
    "Reserved: Unspecified or reserved exception type, specific to certain "
    "hardware or architectural features.",
    "Reserved: Unspecified or reserved exception type, specific to certain "
    "hardware or architectural features.",
    "Reserved: Unspecified or reserved exception type, specific to certain "
    "hardware or architectural features.",
    "Reserved: Unspecified or reserved exception type, specific to certain "
    "hardware or architectural features.",
    "Reserved: Unspecified or reserved exception type, specific to certain "
    "hardware or architectural features.",
    "Reserved: Unspecified or reserved exception type, specific to certain "
    "hardware or architectural features.",
    "Hypervisor Injection Exception: Exception injected by a hypervisor or "
    "virtualization layer, signaling virtual machine management events.",
    "VMM Communication Exception: Communication error with the Virtual Machine "
    "Monitor (VMM) or virtualization infrastructure.",
    "Security Exception: Security-related exception, such as unauthorized "
    "access attempt or integrity violation.",
    "Reserved: Unspecified or reserved exception type, specific to certain "
    "hardware or architectural features."};

extern void load_idt(uint64_t);

void set_idt_gate(int num, uint64_t base, uint16_t sel, uint8_t flags) {
    idt[num].offset_low = (base & 0xFFFF);
    idt[num].offset_middle = (base >> 16) & 0xFFFF;
    idt[num].offset_high = (base >> 32) & 0xFFFFFFFF;
    idt[num].selector = sel;
    idt[num].ist = 0;
    idt[num].flags = flags;
    idt[num].zero = 0;
}

void init_idt() {
    idt_p.limit = sizeof(idt_entry_t) * IDT_ENTRIES - 1;
    idt_p.base = (uint64_t)&idt;

    vok("limit: 0x%04x base: 0x%016x", idt_p.limit, idt_p.base);

    for(size_t i = 0; i < 16; i++) {
        irq_handlers[i] = NULL;
    }

    asm("sti");
    pic_configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, true);

    for(int i = 0; i < IDT_ENTRIES; ++i) {
        set_idt_gate(i, isr_tbl[i], 0x28, 0x8E);
    }

    pic_disable();
    load_idt((uint64_t)&idt_p);
    pic_enable();
    asm("cli");
    vvok("done.");
}

void excp_handler(int_frame_t frame) {
    if(frame.vector < 0x20) {
        panic(exception_strings[frame.vector],
              exception_descriptions[frame.vector], frame, last_rbp);
        hcf();
    } else if(frame.vector >= 0x20 && frame.vector <= 0x2f) {
        int irq = frame.vector - 0x20;
        typedef void (*handler_func_t)(int_frame_t *);

        handler_func_t handler = irq_handlers[irq];

        if(handler != NULL) {
            handler(&frame);
        }

        pic_sendEOI(irq);
    } else if(frame.vector == 0x80) {
        vok("Handeling system call!");
    }
}

void irq_register(uint8_t irq, void *handler) { irq_handlers[irq] = handler; }

void irq_deregister(uint8_t irq) { irq_handlers[irq] = NULL; }

#ifndef IDT_H
#define IDT_H

#include <stdint.h>

typedef struct
{
	uint16_t offset_low;
	uint16_t selector;
	uint8_t ist;
	uint8_t flags;
	uint16_t offset_middle;
	uint32_t offset_high;
	uint32_t zero;
} __attribute__((packed)) idt_entry_t;

typedef struct
{
	uint16_t limit;
	uint64_t base;
} __attribute__((packed)) idt_pointer_t;

typedef struct
{
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rbp;
	uint64_t rdi;
	uint64_t rsi;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;

	uint64_t vector;
	uint64_t err;

	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} __attribute__((packed)) int_frame_t;

void load_idt(uint64_t);
void trigger_interupt(uint64_t a);
void set_idt_gate(int num, uint64_t base, uint16_t sel, uint8_t flags);
void init_idt();
void irq_register(uint8_t irq, void *handler);
void irq_deregister(uint8_t irq);

#endif /* IDT_H */
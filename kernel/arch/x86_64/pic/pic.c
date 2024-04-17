#include <sys/leaf.h>
#include ARCH_INCLUDE(pic/pic.h)
#include <drivers/stdio/serial.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

enum
{
    PIC_ICW1_ICW4 = 0x01,
    PIC_ICW1_SINGLE = 0x02,
    PIC_ICW1_INTERVAL4 = 0x04,
    PIC_ICW1_LEVEL = 0x08,
    PIC_ICW1_INITIALIZE = 0x10
} PIC_ICW1;

enum
{
    PIC_ICW4_8086 = 0x1,
    PIC_ICW4_AUTO_EOI = 0x2,
    PIC_ICW4_BUFFER_MASTER = 0x4,
    PIC_ICW4_BUFFER_SLAVE = 0x0,
    PIC_ICW4_BUFFERRED = 0x8,
    PIC_ICW4_SFNM = 0x10,
} PIC_ICW4;

enum
{
    PIC_CMD_END_OF_INTERRUPT = 0x20,
    PIC_CMD_READ_IRR = 0x0A,
    PIC_CMD_READ_ISR = 0x0B,
} PIC_CMD;

static uint16_t PICMask = 0xffff;

void pic_setMask(uint16_t mask)
{
    PICMask = mask;
    outb(PIC1_DATA, PICMask & 0xFF);
    iowait();
    outb(PIC2_DATA, PICMask >> 8);
    iowait();
}

uint16_t pic_getMask()
{
    uint16_t mask = inb(PIC1_DATA) | (inb(PIC2_DATA << 8));
    return mask;
}

void pic_configure(uint8_t PIC1_offset, uint8_t PIC2_offset, bool autoEOI)
{
    pic_setMask(0xFFFF);
    outb(PIC1_COMMAND, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE);
    iowait();
    outb(PIC2_COMMAND, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE);
    iowait();
    outb(PIC1_DATA, PIC1_offset);
    iowait();
    outb(PIC2_DATA, PIC2_offset);
    iowait();
    outb(PIC1_DATA, 0x4);
    iowait();
    outb(PIC2_DATA, 0x2);
    iowait();

    uint8_t icw4 = PIC_ICW4_8086;
    if (autoEOI)
    {
        icw4 |= PIC_ICW4_AUTO_EOI;
    }

    outb(PIC1_DATA, icw4);
    iowait();
    outb(PIC2_DATA, icw4);
    iowait();

    pic_setMask(0xFFFF);
}

void pic_sendEOI(int irq)
{

    if (irq >= 8)
        outb(PIC2_COMMAND, PIC_CMD_END_OF_INTERRUPT);

    outb(PIC1_COMMAND, PIC_CMD_END_OF_INTERRUPT);
}

void pic_disable()
{
    pic_setMask(0xFFFF);
}

void pic_enable()
{
    outb(PIC1_COMMAND, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE);
    iowait();
    outb(PIC2_COMMAND, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE);
    iowait();
    outb(PIC1_DATA, PIC_REMAP_OFFSET);
    iowait();
    outb(PIC2_DATA, PIC_REMAP_OFFSET + 8);
    iowait();
    outb(PIC1_DATA, 4);
    iowait();
    outb(PIC2_DATA, 2);
    iowait();
    outb(PIC1_DATA, PIC_ICW4_8086);
    iowait();
    outb(PIC2_DATA, PIC_ICW4_8086);
    iowait();
    uint8_t mask1 = inb(PIC1_DATA) & ~(1 << 2);
    outb(PIC1_DATA, mask1);
    iowait();
    uint8_t mask2 = inb(PIC2_DATA) & ~0x80;
    outb(PIC2_DATA, mask2);
    iowait();
}

void pic_mask(int irq)
{
    pic_setMask(PICMask | (1 << irq));
}

void pic_unmask(int irq)
{
    pic_setMask(PICMask & ~(1 << irq));
}

uint16_t pic_read_irq_request_register()
{
    outb(PIC1_COMMAND, PIC_CMD_READ_IRR);
    outb(PIC2_COMMAND, PIC_CMD_READ_IRR);
    return ((uint16_t)inb(PIC2_COMMAND)) | (((uint16_t)inb(PIC2_COMMAND)) << 8);
}

uint16_t pic_read_in_service_register()
{
    outb(PIC1_COMMAND, PIC_CMD_READ_ISR);
    outb(PIC2_COMMAND, PIC_CMD_READ_ISR);
    return ((uint16_t)inb(PIC2_COMMAND)) | (((uint16_t)inb(PIC2_COMMAND)) << 8);
}
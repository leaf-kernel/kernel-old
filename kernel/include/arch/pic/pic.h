#ifndef __PIC_H__
#define __PIC_H__

#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <stdbool.h>

#define PIC_REMAP_OFFSET 0x20

void pic_configure(uint8_t PIC1_offset, uint8_t PIC2_offset, bool autoEOI);
void pic_disable();
void pic_enable();
void pic_mask(int irq);
void pick_unmask(int irq);
void pic_sendEOI(int irq);
uint16_t pic_read_irq_request_register();
uint16_t pic_read_in_service_register();

#endif // __PIC_H__
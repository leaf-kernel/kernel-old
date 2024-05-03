#ifndef __PCI_H__
#define __PCI_H__

#include <drivers/serial/serial.h>
#define LEAF_INCLUDE_PRIVATE
#include <stdint.h>
#include <sys/leaf.h>

typedef struct device_descriptor {
    uint32_t portBase;
    uint32_t interrupt;

    uint16_t bus;
    uint16_t device;
    uint16_t function;

    uint16_t vendor_id;
    uint16_t device_id;

    uint8_t class_id;
    uint8_t subclass_id;
    uint8_t interface_id;

    uint8_t revision;
} device_descriptor;

uint32_t read_pci(uint16_t bus, uint16_t device, uint16_t function,
                  uint32_t regoffset);
void write_pci(uint16_t bus, uint16_t device, uint16_t function,
               uint32_t regoffset, uint32_t data);
void init_pci();
device_descriptor get_device_descriptor(uint16_t bus, uint16_t device,
                                        uint16_t function);

#endif  // __PCI_H__
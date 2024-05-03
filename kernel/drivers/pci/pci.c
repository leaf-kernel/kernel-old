#include <drivers/pci/pci.h>

uint32_t read_pci(uint16_t bus, uint16_t device, uint16_t function,
                  uint32_t regoffset) {
    uint32_t id = 0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) |
                  ((function & 0x07) << 8) | (regoffset & 0xFC);
    outd(0xCF8, id);
    uint32_t result = ind(0xCFC);
    return result >> ((regoffset % 4) << 3);
}

void write_pci(uint16_t bus, uint16_t device, uint16_t function,
               uint32_t regoffset, uint32_t data) {
    uint32_t id = 0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) |
                  ((function & 0x07) << 8) | (regoffset & 0xFC);
    outd(0xCF8, id);
    outd(0xCFC, data);
}

device_descriptor get_device_descriptor(uint16_t bus, uint16_t device,
                                        uint16_t function) {
    device_descriptor result;

    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = read_pci(bus, device, function, 0x00);
    result.device_id = read_pci(bus, device, function, 0x02);

    result.class_id = read_pci(bus, device, function, 0x0b);
    result.subclass_id = read_pci(bus, device, function, 0x0a);
    result.interface_id = read_pci(bus, device, function, 0x09);

    result.revision = read_pci(bus, device, function, 0x08);
    result.interrupt = read_pci(bus, device, function, 0x3c);

    return result;
}

void init_pci() {
    for(int bus = 0; bus < 8; bus++) {
        for(int device = 0; device < 32; device++) {
            for(int function = 0; function < 8; function++) {
                device_descriptor desc =
                    get_device_descriptor(bus, device, function);

                if(desc.vendor_id == 0x0000 || desc.vendor_id == 0xFFFF)
                    continue;

                cdlog("PCI Bus: %d Device: %d ID: %04X Function: %d USB: %s",
                      (uint8_t)(bus & 0xFF), (uint8_t)(device & 0xFF),
                      desc.device_id, ((uint8_t)((function & 0xFF))),
                      ((desc.class_id == 0x0C) && (desc.subclass_id == 0x03))
                          ? "yes"
                          : "no");
            }
        }
    }
}
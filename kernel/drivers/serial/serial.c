#include <drivers/serial/serial.h>

#define UNUSED 0x80

uint16_t __cur_port;
bool _serial_has_been_init;
char *_serial_cur_com_char;

void iowait() { outb(UNUSED, 0); }

void outb(uint16_t port, uint8_t value) {
    __asm__ volatile("outb %1, %0" : : "dN"(port), "a"(value));
}

void outw(uint16_t port, uint16_t value) {
    __asm__ volatile("outw %1, %0" : : "dN"(port), "a"(value));
}

void outd(uint16_t port, uint32_t value) {
    __asm__ volatile("outl %1, %0" : : "dN"(port), "a"(value));
}

uint8_t inb(uint16_t port) {
    uint8_t r;
    __asm__ volatile("inb %1, %0" : "=a"(r) : "dN"(port));
    return r;
}

uint16_t inw(uint16_t port) {
    uint16_t r;
    __asm__ volatile("inw %1, %0" : "=a"(r) : "dN"(port));
    return r;
}

uint32_t ind(uint16_t port) {
    uint32_t r;
    __asm__ volatile("inl %1, %0" : "=a"(r) : "dN"(port));
    return r;
}

int _is_transmit_empty() { return inb(__cur_port + 5) & 0x20; }
int _iis_transmit_empty(uint16_t port) { return inb(port + 5) & 0x20; }

bool _register_port(uint16_t port) {
    outb(port + 1, 0x00);
    outb(port + 3, 0x80);
    outb(port + 0, 0x03);
    outb(port + 1, 0x00);
    outb(port + 3, 0x03);
    outb(port + 2, 0xC7);
    outb(port + 4, 0x0B);
    outb(port + 4, 0x1E);
    outb(port + 0, 0x69);
    if(inb(port + 0) != 0x69) {
        fail("Failed to register port \"0x%04llx\"", port);
        return false;
    }

    outb(port + 4, 0x0F);
    vok("Registered port \"0x%04llx\"", port);
    return true;
}

void init_serial() {
    _register_port(_SERIAL_COM1);
    switch_serial(1, 0);

    _serial_has_been_init = true;
    vvok("%s: done.", __func__);
}

int _serial_received() { return inb(__cur_port + 5) & 1; }

char read_serial() {
    while(_serial_received() == 0)
        ;

    return inb(__cur_port);
}
void switch_serial(uint8_t id, uint16_t port) {
    if(id != 0 && id <= 8) {
        switch(id) {
        case 1:
            __cur_port = _SERIAL_COM1;
            _serial_cur_com_char = "COM1";
            break;
        case 2:
            __cur_port = _SERIAL_COM2;
            _serial_cur_com_char = "COM2";
            break;
        case 3:
            __cur_port = _SERIAL_COM3;
            _serial_cur_com_char = "COM3";
            break;
        case 4:
            __cur_port = _SERIAL_COM4;
            _serial_cur_com_char = "COM4";
            break;
        case 5:
            __cur_port = _SERIAL_COM5;
            _serial_cur_com_char = "COM5";
            break;
        case 6:
            __cur_port = _SERIAL_COM6;
            _serial_cur_com_char = "COM6";
            break;
        case 7:
            __cur_port = _SERIAL_COM7;
            _serial_cur_com_char = "COM7";
            break;
        case 8:
            __cur_port = _SERIAL_COM8;
            _serial_cur_com_char = "COM8";
            break;
        default:
            fail("Invalid COM id!");
            hcf();
            break;
        }
    } else {
        __cur_port = port;
        _serial_cur_com_char = "???";
    }
}

void write_serial(char a) {
    while(_is_transmit_empty() == 0)
        ;

    if(a != '\0')
        outb(__cur_port, a);
}

void flush_serial() {
    if(_leaf_should_clear_serial) {
        write_serial(27);
        write_serial(99);
    }

    if(_leaf_should_flush_serial) {
        write_serial(0);
    }
}
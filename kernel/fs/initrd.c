#include <fs/initrd.h>
#include <sys/logger.h>
#include <memory/kheap.h>

Ramdisk* init_ramdisk(const char* raw, const size_t size) {
    Ramdisk* initrd = (Ramdisk*)kmalloc(sizeof(Ramdisk));

    (void)raw;
    (void)size;


    cdebug_log(__func__, "done.");
    return initrd;
}
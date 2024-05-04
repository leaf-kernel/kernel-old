#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

#include <arch/cpu/cpu.h>
#include <arch/cpu/cpuid.h>
#include <arch/cpu/utils.h>
#include <arch/x86_64/acpi/mcfg.h>
#include <arch/x86_64/idt/idt.h>
#include <fs/vfs.h>
#include <libc/stdlib/memory/kheap.h>
#include <sys/_config.h>
#include <sys/backtrace.h>
#include <sys/time/rtc.h>
#include <utils/parsing/ini.h>

int main() {
    printf("Hello, World on tty%03d!\r\n", currentTTYid);
    hlt();
    return LEAF_RETURN_SUCCESS;
}

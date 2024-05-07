#include <arch/x86_64/acpi/fadt.h>

#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

fadt_t *fadt_table;

void init_fadt(fadt_t *fadt) {
    if(strncmp(fadt->h.Signature, "FACP", 4) == 0) {
        vvok("%s: done.", __func__);
        fadt_table = fadt;
    } else {
        fail("Failed to init FADT");
        hcf();
    }
}
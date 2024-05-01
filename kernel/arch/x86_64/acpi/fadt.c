#include <arch/x86_64/acpi/fadt.h>

#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

void init_fadt(fadt_t *fadt)
{
    if (strncmp(fadt->h.Signature, "FACP", 4) == 0)
        cdlog("done.");
    else
        dlog("Failed to init FADT");
}
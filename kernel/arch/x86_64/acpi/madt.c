#include <arch/x86_64/acpi/madt.h>

struct MADTTable *madt_table;

void init_madt(struct MADTTable *madt_table)
{
    if (strncmp(madt_table->h.Signature, "APIC", 4) != 0)
    {
        cdlog("Wrong table passed!");
        hcf();
    }

    madt_table = madt_table;
    cdlog("done.");
}

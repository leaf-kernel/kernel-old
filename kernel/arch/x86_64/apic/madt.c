#include <arch/x86_64/apic/madt.h>

struct MADTTable *madt_table;

void *__get_madt(struct RSDPTable *rsdp_table)
{
    int entries = (rsdp_table->h.Length - sizeof(rsdp_table->h)) / sizeof(uint64_t);

    for (int i = 0; i < entries; i++)
    {
        struct ACPISDTHeader *h = (struct ACPISDTHeader *)rsdp_table->sdt[i];
        if (strncmp(h->Signature, "APIC", 4) == 0)
        {
            return (void *)h;
        }
    }

    return NULL;
}

void init_madt()
{
    struct MADTTable *madt_ptr = (struct MADTTable *)__get_madt(rsdp_table);

    if (madt_ptr == NULL)
    {
        dlog("\033[1;31mFailed to get the MADT table!\033[0m");
        hcf();
    }

    madt_table = (struct MADTTable *)((uint64_t)madt_ptr + 0xffff800000000000);

    dprintf("\n");
    cdlog("done.");
}

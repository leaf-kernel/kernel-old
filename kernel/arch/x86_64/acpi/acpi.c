
#include <arch/x86_64/apic/apic.h>
#include <arch/x86_64/acpi/rsdt.h>
#include <sys/limine.h>
#include <sys/leaf.h>

bool _xsdt_is_available;

void init_acpi()
{
    cdlog("rsdp is at 0x%8llx", rsdp_request.response->address);

    rsdp_t *rsdp = (rsdp_t *)rsdp_request.response->address;
    if (rsdp->revision == 0)
    {
        _xsdt_is_available = false;
        init_rsdt();
    }
    else if (rsdp->revision >= 2)
    {
        _xsdt_is_available = true;
        init_rsdt();
    }

    cdlog("done");
}

void *_find_sdt(char *signature)
{
    int entry_divisor = (_use_xsdt() ? 8 : 4);
    int entries =
        (g_rsdt->header.length - sizeof(g_xsdt->header)) / entry_divisor;
    for (int i = 0; i < entries; i++)
    {
        sdt_t *header = NULL;
        if (_use_xsdt())
        {
            header = (sdt_t *)(uintptr_t)PHYS_TO_VIRT(g_xsdt->sdt[i]);
        }
        else
        {
            header = (sdt_t *)(uintptr_t)PHYS_TO_VIRT(g_rsdt->sdt[i]);
        }
        if (!strncmp(header->signature, signature, 4))
        {
            cdlog("found header with signature %.4s", header->signature);
            return (void *)header;
        }
    }

    return NULL;
}

bool _use_xsdt()
{
    return _xsdt_is_available;
}

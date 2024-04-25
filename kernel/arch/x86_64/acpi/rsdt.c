#include <arch/x86_64/acpi/acpi.h>
#include <arch/x86_64/acpi/rsdt.h>
#include <arch/x86_64/acpi/madt.h>

xsdt_t *g_xsdt;
rsdt_t *g_rsdt;

void init_rsdt()
{
    rsdp_t *rsdp = (rsdp_t *)rsdp_request.response->address;

    g_rsdt = (rsdt_t *)(uintptr_t)PHYS_TO_VIRT(rsdp->rsdt_addr);
    if (_use_xsdt())
    {
        xsdp_t *xsdp = (xsdp_t *)rsdp_request.response->address;
        g_xsdt = (xsdt_t *)(uintptr_t)PHYS_TO_VIRT(xsdp->xsdt_addr);
    }

    madt_t *madt = _find_sdt("APIC");
    if (madt == NULL)
    {

        cdlog("Failed to find MADT!");
        hcf();
    }

    init_madt(madt);

    cdlog("cores: %d", g_acpi_cpu_count);
    cdlog("done");
}
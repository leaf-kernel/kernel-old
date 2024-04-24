#include <arch/cpu/rsdt/rsdt.h>

volatile struct limine_rsdp_request rsdp_request = {.id = LIMINE_RSDP_REQUEST, .revision = 0};
struct RSDPTable *rsdp_table;

void init_rsdt()
{
    uint8_t rsdp_checksum = 0;
    uint8_t *rsdp_ptr = (uint8_t *)rsdp_request.response->address;
    for (uintptr_t i = 0; i < sizeof(struct RSDPTable); i++)
    {
        rsdp_checksum += rsdp_ptr[i];
    }

    if (rsdp_checksum == 0)
    {
        dlog("\033[1;31mFailed to get the RSDP table!\033[0m");
        hcf();
    }

    rsdp_table = (struct RSDPTable *)(uintptr_t)PHYS_TO_VIRT(rsdp_ptr);

    cdlog("0x%02x", rsdp_checksum);

    cdlog("done.");
}

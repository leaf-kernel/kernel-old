#include <libc/stdlib/memory/pmm.h>
#include <libc/stdlib/memory/vmm.h>

#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

struct Level4Group {
    PML4E entries[512];
};

struct Level4Group *PML4Array;

void *get_physaddr(uint64_t vaddr) {
    const uint16_t offset = (uint16_t)((vaddr & 0x000000000FFF) >> 0);
    const uint16_t PT_i = (uint16_t)((vaddr & 0x0000001FF000) >> 12);
    const uint16_t PD_i = (uint16_t)((vaddr & 0x00003FE00000) >> 21);
    const uint16_t PDP_i = (uint16_t)((vaddr & 0x007FC0000000) >> 30);
    const uint16_t PML4_i = (uint16_t)((vaddr & 0xFF8000000000) >> 39);

    PML4E PML4 = PML4Array->entries[PML4_i];
    if(PML4.Present == 0)
        return NULL;

    PML3E PML3 = ((
        (PML3E *)VIRT_TO_PHYS((void *)((uint64_t)PML4.Address << 12)))[PDP_i]);
    if(PML3.Present == 0)
        return NULL;

    PML2E PML2 =
        (((PML2E *)VIRT_TO_PHYS((void *)((uint64_t)PML3.Address << 12)))[PD_i]);
    if(PML2.Present == 0)
        return NULL;

    PML1E PML1 =
        (((PML1E *)VIRT_TO_PHYS((void *)((uint64_t)PML2.Address << 12)))[PT_i]);
    if(PML1.Present == 0)
        return NULL;

    return (void *)(((uint64_t)PML1.Address << 12) | offset);
}

void _x86_64_vmm_map(uint64_t vaddr, uint64_t paddr, uint32_t flags) {
    const uint16_t PT_i = (uint16_t)((vaddr & 0x0000001FF000) >> 12);
    const uint16_t PD_i = (uint16_t)((vaddr & 0x00003FE00000) >> 21);
    const uint16_t PDP_i = (uint16_t)((vaddr & 0x007FC0000000) >> 30);
    const uint16_t PML4_i = (uint16_t)((vaddr & 0xFF8000000000) >> 39);

    PML4E PML4 = PML4Array->entries[PML4_i];
    if(PML4.Present == 0) {
        uint64_t temp = ((uint64_t)((flags & 0x0FFF) |
                                    ((uint64_t)(flags & 0x07FF0000) << 36)));
        PML4 = *(PML4E *)(&temp);
        PML4.Present = 1;
        PML4.Address = (uint64_t)pmm_request_page() >> 12;
        memset((void *)VIRT_TO_PHYS(PML4.Address << 12), 0, 4096);

        PML4Array->entries[PML4_i] = PML4;
    } else {
        uint64_t temp = *(uint64_t *)(&PML4);
        temp |= flags & 0xFFF;
        temp |= (uint64_t)(flags & 0x7FF0000) << 36;
        PML4Array->entries[PML4_i] = *(PML4E *)&temp;
    }

    PML3E PML3 = ((PML3E *)VIRT_TO_PHYS(
        (void *)((uint64_t)(PML4.Address) << 12)))[PDP_i];

    if(PML3.Present == 0) {
        uint64_t temp = ((uint64_t)((flags & 0x0FFF) |
                                    ((uint64_t)(flags & 0x07FF0000) << 36)));
        PML3 = *(PML3E *)(&temp);
        PML3.Present = 1;
        PML3.Address = (uint64_t)pmm_request_page() >> 12;
        memset((void *)VIRT_TO_PHYS(PML3.Address << 12), 0, 4096);

        ((PML3E *)VIRT_TO_PHYS(
            (void *)((uint64_t)(PML4.Address) << 12)))[PDP_i] = PML3;
    } else {
        uint64_t temp = *(uint64_t *)(&PML3);
        temp |= flags & 0xFFF;
        temp |= (uint64_t)(flags & 0x7FF0000) << 36;
        ((PML3E *)VIRT_TO_PHYS(
            (void *)((uint64_t)(PML4.Address) << 12)))[PDP_i] = *(PML3E *)&temp;
    }

    PML2E PML2 =
        ((PML2E *)VIRT_TO_PHYS((void *)((uint64_t)(PML3.Address) << 12)))[PD_i];

    if(PML2.Present == 0) {
        uint64_t temp = ((uint64_t)((flags & 0x0FFF) |
                                    ((uint64_t)(flags & 0x07FF0000) << 36)));
        PML2 = *(PML2E *)(&temp);
        PML2.Present = 1;
        PML2.Address = (uint64_t)pmm_request_page() >> 12;
        memset((void *)VIRT_TO_PHYS(PML2.Address << 12), 0, 4096);

        ((PML2E *)VIRT_TO_PHYS(
            (void *)((uint64_t)(PML3.Address) << 12)))[PD_i] = PML2;
    } else {
        uint64_t temp = *(uint64_t *)(&PML2);
        temp |= flags & 0xFFF;
        temp |= (uint64_t)(flags & 0x7FF0000) << 36;
        ((PML2E *)VIRT_TO_PHYS(
            (void *)((uint64_t)(PML3.Address) << 12)))[PD_i] = *(PML2E *)&temp;
    }

    uint64_t temp =
        ((uint64_t)((flags & 0x0FFF) | ((uint64_t)(flags & 0x0FFF0000) << 36)));
    PML1E PML1 = *(PML1E *)(&temp);
    PML1.Address = (paddr >> 12);

    ((PML1E *)VIRT_TO_PHYS((void *)((uint64_t)(PML2.Address) << 12)))[PT_i] =
        PML1;
}

void vmm_map_range(void *virt, void *phys, void *virt_end, uint32_t perms) {
    for(virt; virt < virt_end; virt += PAGE_SIZE, phys += PAGE_SIZE) {
        vmm_map((uint64_t)virt, (uint64_t)phys, perms);
    }
}

void init_vmm() {
    PML4Array = (struct Level4Group *)kmalloc(sizeof(struct Level4Group));
    if(PML4Array == NULL) {
        fail("Failed to allocate memory for PML4 array");
        hcf();
    }

    vok("Kernel Physical Address: 0x%lX", kernel_addr_response->physical_base);
    vok("Kernel Virtual Address: 0x%lx", kernel_addr_response->virtual_base);

    for(uint64_t addr = 0; addr < (UINT32_MAX + 1); addr += 4096) {
        vmm_map(PHYS_TO_VIRT(addr), addr, 0x08000003);
    }

    vvok("%s: done.", __func__);
}
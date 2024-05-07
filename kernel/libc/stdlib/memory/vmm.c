#include <libc/stdlib/memory/pmm.h>
#include <libc/stdlib/memory/vmm.h>

#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>

struct Level4Group {
    PML4E entries[512];
};

struct Level3Group {
    PML3E entries[512];
};

struct Level2Group {
    PML2E entries[512];
};

struct Level1Group {
    PML1E entries[512];
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

void init_vmm() {
    PML4Array = (struct Level4Group *)kmalloc(sizeof(struct Level4Group));
    if(PML4Array == NULL) {
        fail("Failed to allocate memory for PML4 array");
        hcf();
    }
    vvok("%s: done.", __func__);
}
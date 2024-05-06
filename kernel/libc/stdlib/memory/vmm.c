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

void _x86_64_vmm_map(uint64_t vaddr, uint64_t paddr, uint32_t flags) {
    const uint16_t pt = (uint16_t)((vaddr & 0x0000001FF000) >> 12);
    const uint16_t pd = (uint16_t)((vaddr & 0x00003FE00000) >> 21);
    const uint16_t pdptr = (uint16_t)((vaddr & 0x007FC0000000) >> 30);
    const uint16_t pml4 = (uint16_t)((vaddr & 0xFF8000000000) >> 39);

    PML4E entry = PML4Array->entries[pml4];
    if(entry.Present == 0) {
        uint64_t temp = ((uint64_t)((flags & 0x0FFF) |
                                    ((uint64_t)(flags & 0x07FF0000) << 36)));
        entry = *(PML4E *)(&temp);
        entry.Present = 1;
        entry.Address = (uint64_t)pmm_request_page() >> 12;
        memset((void *)VIRT_TO_PHYS(entry.Address << 12), 0, 512);

        PML4Array->entries[pml4] = entry;
    } else {
        uint64_t temp = *(uint64_t *)(&entry);
        temp |= flags & 0xFFF;
        temp |= (uint64_t)(flags & 0x7FF0000) << 36;
        PML4Array->entries[pml4] = *(PML4E *)&temp;
    }
}

void init_vmm() {
    PML4Array = (struct Level4Group *)kmalloc(sizeof(struct Level4Group));
    if(PML4Array == NULL) {
        dlog("Failed to allocate memory for PML4 array");
        hcf();
    }
    vvcplog("done.");
}
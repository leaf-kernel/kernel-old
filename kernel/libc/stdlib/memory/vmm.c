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
    cdlog("PT: 0x%.4llx PD: 0x%.4llx PDPTR: 0x%.4llx PML4: 0x%.4llx ", pt, pd,
          pdptr, pml4);
}

void init_vmm() { cdlog("done."); }
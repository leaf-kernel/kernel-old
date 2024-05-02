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
    vvcdlog("Mapped 0x%.16llx -> 0x%.16llx", vaddr, paddr);
}

void init_vmm() { cdlog("done."); }
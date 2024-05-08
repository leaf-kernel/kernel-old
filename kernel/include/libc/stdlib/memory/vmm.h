#ifndef __VMM_H__
#define __VMM_H__

#include <stdint.h>

typedef struct {
    uint8_t Present : 1;
    uint8_t ReadWrite : 1;
    uint8_t UserSuper : 1;
    uint8_t WriteThrough : 1;
    uint8_t CacheDisable : 1;
    uint8_t Accessed : 1;
    uint8_t Available0 : 1;
    uint8_t Reserved0 : 1;
    uint8_t Available1 : 4;
    uint64_t Address : 40;
    uint16_t Available2 : 11;
    uint8_t ExecuteDisable : 1;
} __attribute((packed)) PML4E;

typedef struct {
    uint8_t Present : 1;
    uint8_t ReadWrite : 1;
    uint8_t UserSuper : 1;
    uint8_t WriteThrough : 1;
    uint8_t CacheDisable : 1;
    uint8_t Accessed : 1;
    uint8_t Available0 : 1;
    uint8_t PageSize : 1;
    uint8_t Available1 : 4;
    uint64_t Address : 40;
    uint16_t Available2 : 11;
    uint8_t ExecuteDisable : 1;
} __attribute((packed)) PML3E;

typedef struct {
    uint8_t Present : 1;
    uint8_t ReadWrite : 1;
    uint8_t UserSuper : 1;
    uint8_t WriteThrough : 1;
    uint8_t CacheDisable : 1;
    uint8_t Accessed : 1;
    uint8_t Available0 : 1;
    uint8_t PageSize : 1;
    uint8_t Available1 : 4;
    uint64_t Address : 40;
    uint16_t Available2 : 11;
    uint8_t ExecuteDisable : 1;
} __attribute((packed)) PML2E;

typedef struct {
    uint8_t Present : 1;
    uint8_t ReadWrite : 1;
    uint8_t UserSuper : 1;
    uint8_t WriteThrough : 1;
    uint8_t CacheDisable : 1;
    uint8_t Accessed : 1;
    uint8_t Dirty : 1;
    uint8_t PAT : 1;
    uint8_t Global : 1;
    uint8_t Available0 : 3;
    uint64_t Address : 40;
    uint8_t Available1 : 7;
    uint8_t ProtKey : 4;
    uint8_t ExecuteDisable : 1;
} __attribute((packed)) PML1E;

#define _VMM_PRESENT 1
#define _VMM_WRITE 2
#define _VMM_USER 4
#define _VMM_EXECUTE_DISABLE 0x8000000

void *get_physaddr(uint64_t vaddr);
void _x86_64_vmm_map(uint64_t vaddr, uint64_t paddr, uint32_t flags);
void vmm_map_range(void *virt, void *phys, void *virt_end, uint32_t perms);

void init_vmm();

#ifdef LEAF_X64
#define vmm_map(vaddr, paddr, flags) _x86_64_vmm_map(vaddr, paddr, flags)
#else
#define vmm_map(vaddr, paddr, flags) __LEAF_VOID_REDIRECT__
#endif

#endif  // __VMM_H__
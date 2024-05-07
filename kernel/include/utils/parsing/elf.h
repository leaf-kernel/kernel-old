#ifndef __ELF_H__
#define __ELF_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/run/runner.h>

typedef uint16_t Elf64_Half;
typedef uint64_t Elf64_Off;
typedef uint64_t Elf64_Addr;
typedef uint32_t Elf64_Word;
typedef int32_t Elf64_Sword;

#define ELF_NIDENT 16

typedef struct {
    uint8_t e_ident[ELF_NIDENT];
    Elf64_Half e_type;
    Elf64_Half e_machine;
    Elf64_Word e_version;
    Elf64_Addr e_entry;
    Elf64_Off e_phoff;
    Elf64_Off e_shoff;
    Elf64_Word e_flags;
    Elf64_Half e_ehsize;
    Elf64_Half e_phentsize;
    Elf64_Half e_phnum;
    Elf64_Half e_shentsize;
    Elf64_Half e_shnum;
    Elf64_Half e_shstrndx;
} Elf64_Ehdr;

typedef struct {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} Elf64_Phdr;

enum Elf_Ident {
    EI_MAG0 = 0,
    EI_MAG1 = 1,
    EI_MAG2 = 2,
    EI_MAG3 = 3,
    EI_CLASS = 4,
    EI_DATA = 5,
    EI_VERSION = 6,
    EI_OSABI = 7,
    EI_ABIVERSION = 8,
    EI_PAD = 9
};

#define ELFMAG0 0x7F
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'

#define ELFDATA2LSB (1)
#define ELFCLASS64 (2)

enum Elf_Type { ET_NONE = 0, ET_REL = 1, ET_EXEC = 2 };

enum Elf_PType {
    PT_NULL = 0,
    PT_LOAD = 1,
    PT_DYNAMIC = 2,
    PT_INTERP = 3,
    PT_NOTE = 4
};

#define EM_X86_64 (0x3E)

#define EV_CURRENT (1)

int parse_elf_service(service_t *self, void *name);
bool elf_check_file(Elf64_Ehdr *hdr);
void parse_elf(const char *name, bool verbose);

#endif
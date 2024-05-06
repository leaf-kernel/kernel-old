#include <sys/logger.h>
#include <utils/parsing/elf.h>

bool elf_check_file(Elf64_Ehdr *hdr) {
    if(!hdr)
        return false;
    if(hdr->e_ident[EI_MAG0] != ELFMAG0) {
        plog_fail("ELF Header EI_MAG0 incorrect.");
        return false;
    }
    if(hdr->e_ident[EI_MAG1] != ELFMAG1) {
        plog_fail("ELF Header EI_MAG1 incorrect.");
        return false;
    }
    if(hdr->e_ident[EI_MAG2] != ELFMAG2) {
        plog_fail("ELF Header EI_MAG2 incorrect.");
        return false;
    }
    if(hdr->e_ident[EI_MAG3] != ELFMAG3) {
        plog_fail("ELF Header EI_MAG3 incorrect.");
        return false;
    }
    return true;
}

bool elf_check_supported(Elf64_Ehdr *hdr) {
    if(!elf_check_file(hdr)) {
        plog_fail("Invalid ELF File.");
        return false;
    }
    if(hdr->e_ident[EI_CLASS] != ELFCLASS64) {
        plog_fail("Unsupported ELF File Class.");
        return false;
    }
    if(hdr->e_ident[EI_DATA] != ELFDATA2LSB) {
        plog_fail("Unsupported ELF File byte order.");
        return false;
    }
    if(hdr->e_machine != EM_X86_64) {
        plog_fail("Unsupported ELF File target.");
        return false;
    }
    if(hdr->e_ident[EI_VERSION] != EV_CURRENT) {
        plog_fail("Unsupported ELF File version.");
        return false;
    }
    if(hdr->e_type != ET_REL && hdr->e_type != ET_EXEC) {
        plog_fail("Unsupported ELF File type.");
        return false;
    }
    return true;
}

void parse_elf(const char *file) {
    Elf64_Ehdr *hdr = (Elf64_Ehdr *)file;
    if(!elf_check_file(hdr)) {
        plog_fail("Invalid ELF file.");
        return;
    }

    if(!elf_check_supported(hdr)) {
        plog_fail("Unsupported ELF file.");
        return;
    }

    plog_ok("ELF Magic: 0x%X + \"%c%c%c\"", hdr->e_ident[EI_MAG0],
            hdr->e_ident[EI_MAG1], hdr->e_ident[EI_MAG2],
            hdr->e_ident[EI_MAG3]);
}

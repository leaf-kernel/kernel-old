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

int parse_elf_service(service_t *self, void *name) {
    (void)self;
    parse_elf((char *)name, self->config->verbose);
}

void parse_elf(const char *name, bool verbose) {

    char *elf_raw;
    vfs_op_status status;

    VFS_t *vfs = (VFS_t *)__LEAF_GET_VFS__();

    status = drive_read(vfs, 0, (char *)name, &elf_raw);

    if(status != STATUS_OK) {
        plog_fatal("Failed to read \"%s\"!, name");
        return;
    }

    Elf64_Ehdr *hdr = (Elf64_Ehdr *)elf_raw;
    if(!elf_check_file(hdr)) {
        plog_fail("Invalid ELF file.");
        return;
    }

    if(!elf_check_supported(hdr)) {
        plog_fail("Unsupported ELF file.");
        return;
    }

    if(hdr->e_type == ET_REL) {
        plog_fail("REL files are not supported.");
        return;
    } else if(hdr->e_type == ET_EXEC) {
        if(verbose)
            plog_ok("Indentified \"%s\" as an ELF Executable.", name);
    }

    if(verbose)
        plog_ok("Found %d program headers.", hdr->e_phnum);
    for(int i = 0; i < hdr->e_phnum; i++) {
        Elf64_Phdr *phdr =
            (Elf64_Phdr *)(elf_raw + hdr->e_phoff) + (i * hdr->e_phentsize);

        if(phdr->p_type == PT_LOAD) {
            if(verbose)
                plog_ok("Program header %d type is PT_LOAD.", i);
        } else {
            plog_fail("Program header %d is not PT_LOAD. It is %d.", i,
                      phdr->p_type);
            return;
        }
    }

    return;
}

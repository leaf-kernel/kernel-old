#include <sys/logger.h>
#include <utils/parsing/elf.h>

bool elf_check_file(Elf64_Ehdr *hdr) {
    if(!hdr)
        return false;
    if(hdr->e_ident[EI_MAG0] != ELFMAG0) {
        fail("ELF Header EI_MAG0 incorrect.");
        return false;
    }
    if(hdr->e_ident[EI_MAG1] != ELFMAG1) {
        fail("ELF Header EI_MAG1 incorrect.");
        return false;
    }
    if(hdr->e_ident[EI_MAG2] != ELFMAG2) {
        fail("ELF Header EI_MAG2 incorrect.");
        return false;
    }
    if(hdr->e_ident[EI_MAG3] != ELFMAG3) {
        fail("ELF Header EI_MAG3 incorrect.");
        return false;
    }
    return true;
}

bool elf_check_supported(Elf64_Ehdr *hdr) {
    if(!elf_check_file(hdr)) {
        fail("Invalid ELF File.");
        return false;
    }
    if(hdr->e_ident[EI_CLASS] != ELFCLASS64) {
        fail("Unsupported ELF File Class.");
        return false;
    }
    if(hdr->e_ident[EI_DATA] != ELFDATA2LSB) {
        fail("Unsupported ELF File byte order.");
        return false;
    }
    if(hdr->e_machine != EM_X86_64) {
        fail("Unsupported ELF File target.");
        return false;
    }
    if(hdr->e_ident[EI_VERSION] != EV_CURRENT) {
        fail("Unsupported ELF File version.");
        return false;
    }
    if(hdr->e_type != ET_REL && hdr->e_type != ET_EXEC) {
        fail("Unsupported ELF File type.");
        return false;
    }
    return true;
}

int parse_elf_service(service_t *self, void *name) {
    (void)self;
    int s = parse_elf((char *)name, self->config->verbose);
    if(s == -1) {
        return SERVICE_ERROR_UNKNOWN;
    } else if(s == 1) {
        return SERVICE_ERROR_FILE_NOT_FOUND;
    }

    return LEAF_RETURN_SUCCESS;
}

int parse_elf(const char *name, bool verbose) {
    char *elf_raw;
    vfs_op_status status;

    VFS_t *vfs = (VFS_t *)__LEAF_GET_VFS__();

    status = drive_read(vfs, 0, (char *)name, &elf_raw);

    if(status != STATUS_OK) {
        return 1;
    }

    Elf64_Ehdr *hdr = (Elf64_Ehdr *)elf_raw;
    if(!elf_check_file(hdr)) {
        fail("Invalid ELF file.");
        return -1;
    }

    if(!elf_check_supported(hdr)) {
        fail("Unsupported ELF file.");
        return -1;
    }

    if(hdr->e_type == ET_REL) {
        fail("REL files are not supported.");
        return -1;
    } else if(hdr->e_type == ET_EXEC) {
        if(verbose)
            ok("Indentified \"%s\" as an ELF Executable.", name);
    }

    if(verbose)
        ok("Found %d program headers.", hdr->e_phnum);
    for(int i = 0; i < hdr->e_phnum; i++) {
        Elf64_Phdr *phdr =
            (Elf64_Phdr *)(elf_raw + hdr->e_phoff) + (i * hdr->e_phentsize);

        if(phdr->p_type == PT_LOAD) {
            if(verbose)
                ok("Program header %d type is PT_LOAD.", i);
        } else {
            fail("Program header %d is not PT_LOAD. It is %d.", i,
                 phdr->p_type);
            return -1;
        }
    }

    return SERVICE_ERROR_NO_VMM;
}

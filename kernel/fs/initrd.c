#include <fs/initrd.h>
#include <libc/stdlib/memory/kheap.h>
#include <sys/logger.h>
#include <utils/hash.h>

Ramdisk *init_ramdisk(const char *raw, const size_t size) {
    Ramdisk *initrd = (Ramdisk *)kmalloc(sizeof(Ramdisk));
    TAREntry *entry = (TAREntry *)kmalloc(sizeof(TAREntry));

    if(entry == NULL) {
        plog("Failed to allocate memory for TAREntry!");
        return NULL;
    }

    if(initrd == NULL) {
        plog("Failed to allocate memory for initrd!");
        kfree(entry);
        return NULL;
    }

    tar_extract(raw, size, entry);

    initrd->content =
        (RamdiskEntry **)kmalloc(entry->fileCount * sizeof(RamdiskEntry *));
    if(initrd->content == NULL) {
        plog("Failed to allocate memory for RamdiskEntry pointers!");
        kfree(entry);
        kfree(initrd);
        return NULL;
    }

    for(int i = 0; i < entry->fileCount; i++) {
        PathComponent *comp = entry->files[i].raw_path;

        RamdiskEntry *ramEntry = (RamdiskEntry *)kmalloc(sizeof(RamdiskEntry));
        if(ramEntry == NULL) {
            plog("Failed to allocate memory for RamdiskEntry!");
            for(int j = 0; j < i; j++)
                kfree(initrd->content[j]);
            kfree(initrd->content);
            kfree(entry);
            kfree(initrd);
            return NULL;
        }

        ramEntry->file = &entry->files[i];
        ramEntry->path = &comp[entry->files[i].number_path_comonents - 1];
        ramEntry->hash = hash_string(entry->files[i].path);

        initrd->content[i] = ramEntry;
        vcplog("hash: 0x%08x", initrd->content[i]->hash);
    }

    initrd->count = entry->fileCount;
    vvcplog("done.");
    kfree(entry);
    return initrd;
}

int find_file_by_hash(Ramdisk *initrd, uint32_t hash) {
    for(int i = 0; i < initrd->count; i++) {
        if(initrd->content[i]->hash == hash) {
            return i;
        }
    }

    return -1;
}

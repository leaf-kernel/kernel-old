#include <fs/initrd.h>
#include <sys/logger.h>
#include <memory/kheap.h>
#include <utils/hash.h>

Ramdisk *init_ramdisk(const char *raw, const size_t size)
{
    Ramdisk *initrd = (Ramdisk *)kmalloc(sizeof(Ramdisk));
    TAREntry *entry = (TAREntry *)kmalloc(sizeof(TAREntry));

    if (entry == NULL)
    {
        debug_log(__FILE__, __LINE__, __func__, "Failed to allocate memory for TAREntry!");
        return NULL;
    }

    if (initrd == NULL)
    {
        debug_log(__FILE__, __LINE__, __func__, "Failed to allocate memory for initrd!");
        kfree(entry);
        return NULL;
    }

    TARExtract(raw, size, entry);

    initrd->content = (RamdiskEntry **)kmalloc(entry->fileCount * sizeof(RamdiskEntry *));
    if (initrd->content == NULL)
    {
        debug_log(__FILE__, __LINE__, __func__, "Failed to allocate memory for RamdiskEntry pointers!");
        kfree(entry);
        kfree(initrd);
        return NULL;
    }

    for (int i = 0; i < entry->fileCount; i++)
    {
        PathComponent *comp = entry->files[i].raw_path;
        char *name = comp[entry->files[i].number_path_comonents - 1].name;
        bool dir = comp[entry->files[i].number_path_comonents - 1].directory;

        RamdiskEntry *ramEntry = (RamdiskEntry *)kmalloc(sizeof(RamdiskEntry));
        if (ramEntry == NULL)
        {
            debug_log(__FILE__, __LINE__, __func__, "Failed to allocate memory for RamdiskEntry!");
            for (int j = 0; j < i; j++)
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

        const char *nameValue = (name != NULL) ? name : "NULL";
        const char *pathValue = (entry->files[i].path != NULL) ? entry->files[i].path : "NULL";
        const char *dirValue = (dir) ? "true" : "false";
        cdebug_log(__func__, "{ path: %s, name: %s, directory: %s, hash: 0x%08x }", pathValue, nameValue, dirValue, initrd->content[i]->hash);
    }

    initrd->count = entry->fileCount;
    cdebug_log(__func__, "done.");
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
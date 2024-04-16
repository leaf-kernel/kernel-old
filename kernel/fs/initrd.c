#include <fs/initrd.h>
#include <sys/logger.h>
#include <memory/kheap.h>

Ramdisk *init_ramdisk(const char *raw, const size_t size)
{
    Ramdisk *initrd = (Ramdisk *)kmalloc(sizeof(Ramdisk));
    TAREntry *entry = (TAREntry *)kmalloc(sizeof(TAREntry));

    if (entry == NULL)
    {
        debug_log(__FILE__, __LINE__, __func__, "Failed to allocate memory for TAREntry!");
        return NULL;
    }

    TARExtract(raw, size, entry);
    cdebug_log(__func__, "%d files extracted.", entry->fileCount);

    RamdiskNode *cur_node = (RamdiskNode *)kmalloc(sizeof(RamdiskNode));
    if (cur_node == NULL)
    {
        debug_log(__FILE__, __LINE__, __func__, "Failed to allocate memory for current node!");
        return NULL;
    }

    for (int i = 0; i < entry->fileCount; i++)
    {
        PathComponent *comp = entry->files[i].raw_path;
        char *name = comp[entry->files[i].number_path_comonents - 1].name;
        bool dir = comp[entry->files[i].number_path_comonents - 1].directory;

        bool root = false;
        const char *nameValue = (name != NULL) ? name : "NULL";
        const char *pathValue = (entry->files[i].path != NULL) ? entry->files[i].path : "NULL";
        const char *dirValue = (dir) ? "true" : "false";
        cdebug_log(__func__, "{ path: %s, name: %s, directory: %s, root: %s }", pathValue, nameValue, dirValue, root ? "true" : "false");
    }
    return initrd;
}
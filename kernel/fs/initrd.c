#include <fs/initrd.h>
#include <sys/logger.h>
#include <memory/kheap.h>

Ramdisk *init_ramdisk(const char *raw, const size_t size)
{
    Ramdisk *initrd = (Ramdisk *)kmalloc(sizeof(Ramdisk));
    TAREntry *entry = (TAREntry *)kmalloc(sizeof(TAREntry));

    if (entry == NULL)
    {
        debug_log(__FILE__, __LINE__, __func__, "Failed to allocate memory for TAREntry for initrd!");
        return NULL;
    }

    TARExtract(raw, size, entry);
    cdebug_log(__func__, "%d files extracted.", entry->fileCount);

    cdebug_log(__func__, "done.");
    return initrd;
}
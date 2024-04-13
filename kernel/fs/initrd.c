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

        int dirs = 0;
        for (int j = 0; j < entry->files[i].number_path_comonents; j++)
        {
            PathComponent *c = entry->files[i].raw_path;
            bool d = c[j].directory;
            if (d)
                dirs++;
        }

        if (dirs > 1)
            root = false;
        else if (dirs == 1 && !dir)
            root = false;
        else
            root = true;

        if (root)
        {
            RamdiskNode *new_node = (RamdiskNode *)kmalloc(sizeof(RamdiskNode));

            if (new_node == NULL)
            {
                debug_log(__FILE__, __LINE__, __func__, "Failed to allocate memory for a new node!");
                return NULL;
            }

            new_node->entry = (RamdiskEntry *)kmalloc(sizeof(RamdiskEntry));
            if (new_node->entry == NULL)
            {
                debug_log(__FILE__, __LINE__, __func__, "Failed to allocate memory for a node entry!");
                return NULL;
            }

            new_node->entry->file = (TARFile *)kmalloc(sizeof(TARFile));
            if (new_node->entry->file == NULL)
            {
                debug_log(__FILE__, __LINE__, __func__, "Failed to allocate memory for a entry file!");
                return NULL;
            }

            new_node->entry->file = &entry->files[i];

            cdebug_log(__func__, "%s -> /!", new_node->entry->file->name);
            if (initrd->root != NULL)
            {
                initrd->root->child = (RamdiskNode *)kmalloc(sizeof(RamdiskNode));
                if (initrd->root->child == NULL)
                {
                    debug_log(__FILE__, __LINE__, __func__, "Failed to allocate memory for child!");
                    return NULL;
                }

                cur_node = initrd->root->child;

                cur_node->entry = (RamdiskEntry *)kmalloc(sizeof(RamdiskEntry));
                if (cur_node->entry == NULL)
                {
                    debug_log(__FILE__, __LINE__, __func__, "Failed to allocate memory for a child entry!");
                    return NULL;
                }

                cur_node->entry->file = (TARFile *)kmalloc(sizeof(TARFile));
                if (cur_node->entry->file == NULL)
                {
                    debug_log(__FILE__, __LINE__, __func__, "Failed to allocate memory for a child file!");
                    return NULL;
                }

                cur_node->entry->file = &entry->files[i];

                cdebug_log(__func__, "Added child (%s) to root!", cur_node->entry->file->name);
            }
            else
            {
                initrd->root = new_node;
                cur_node = new_node;
                cdebug_log(__func__, "Added %s to root!", cur_node->entry->file->name);
            }
        }
        cdebug_log(__func__, "{ path: %s, name: %s, directory: %s, root: %s }", pathValue, nameValue, dirValue, root ? "true" : "false");
    }

    cdebug_log(__func__, "Root file: %s, Root child file: %s", initrd->root->entry->file->name, initrd->root->child->entry->file->name);
    cdebug_log(__func__, "done.");
    return initrd;
}
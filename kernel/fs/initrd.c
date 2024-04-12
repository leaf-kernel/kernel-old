#include <fs/initrd.h>

void init_ramdisk(Ramdisk *initrd, char *raw, size_t size)
{
    DirectoryChild *root_directory = (DirectoryChild *)kmalloc(sizeof(DirectoryChild));
    if (root_directory == NULL)
    {
        debug_log(__FILE__, __LINE__, __func__, "Error: Failed to allocate memory for root directory!");
        return;
    }

    root_directory->name = strdup("/");
    if (root_directory->name == NULL)
    {
        debug_log(__FILE__, __LINE__, __func__, "Error: Failed to allocate memory for root directory name!");
        kfree(root_directory);
        return;
    }

    root_directory->dir_children = NULL;
    root_directory->file_children = NULL;

    initrd->root = root_directory;

    TAREntry *entry = (TAREntry *)kmalloc(sizeof(TAREntry));
    if (entry == NULL)
    {
        debug_log(__FILE__, __LINE__, __func__, "Error: Failed to allocate memory for TAREntry!");
        kfree(root_directory->name);
        kfree(root_directory);
        return;
    }

    TARExtract(raw, size, entry);

    for (uint64_t i = 0; i < entry->fileCount; i++)
    {
        const char *path = entry->files[i].name;
        int num_components = 0;
        PathComponent *components = split_path(path, &num_components);

        if (components != NULL)
        {
            if (components[0].directory == false)
            {
                FileChild *newFileChild = kmalloc(sizeof(FileChild));
                if (newFileChild == NULL)
                {
                    debug_log(__FILE__, __LINE__, __func__, "Error: Failed to allocate memory for FileChild!");
                    free_components(components, num_components);
                    break;
                }

                newFileChild->entry = &entry->files[i];
                newFileChild->dir_parent = initrd->root;

                size_t newFileCount = initrd->fileCount + 1;
                FileChild **newFileChildren = krealloc(root_directory->file_children, newFileCount * sizeof(FileChild *));
                if (newFileChildren == NULL)
                {
                    debug_log(__FILE__, __LINE__, __func__, "Error: Failed to reallocate memory for file_children!");
                    kfree(newFileChild);
                    free_components(components, num_components);
                    break;
                }

                root_directory->file_children = newFileChildren;
                root_directory->file_children[initrd->fileCount] = newFileChild;
                cdebug_log(__func__, "Added %s to root", components[0].name);
                initrd->fileCount = newFileCount;
            }

            free_components(components, num_components);
        }
    }

    kfree(entry);
}

void ramdisk_searchFile(Ramdisk *initrd, char *name)
{
    (void)initrd;
    (void)name;
}
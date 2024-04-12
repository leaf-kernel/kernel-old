#include <fs/tar.h>

uint64_t TARGetSize(const char *in)
{
    unsigned int size = 0;
    unsigned int count = 1;

    for (int j = 11; j > 0; j--, count *= 8)
        size += ((in[j - 1] - '0') * count);

    return size;
}

char *TARRemovePrefix(const char *str, const char *prefix)
{
    size_t prefix_len = strlen(prefix);
    if (strncmp(str, prefix, prefix_len) == 0)
    {
        return strdup(str + prefix_len);
    }
    return strdup(str);
}

void TARExtract(const char *raw, uint64_t size, TAREntry *tar)
{
    tar->files = NULL;
    tar->fileCount = 0;

    for (uint64_t offset = 0; offset < size;)
    {
        TARHeader *header = (TARHeader *)(raw + offset);

        if (header->filename[0] == '\0')
        {
            break;
        }

        TARFile file;
        file.size = TARGetSize(header->size);
        file.name = TARRemovePrefix(header->filename, RAMDISK_PATH_PREFIX);
        file.directory = header->typeflag[0] == '5';

        if (!file.directory)
        {
            file.content = (char *)kmalloc(file.size + 1);
            memcpy(file.content, raw + offset + 512, file.size);
            file.content[file.size] = '\0';
        }
        else
        {
            file.content = NULL;
        }

        TARFile *temp_files = krealloc(tar->files, (tar->fileCount + 1) * sizeof(TARFile));
        if (temp_files == NULL)
        {
            debug_log(__FILE__, __LINE__, __func__, "Failed to allocate memory for temp file!");
            return;
        }

        tar->files = temp_files;
        tar->files[tar->fileCount] = file;
        tar->fileCount++;

        offset += ((file.size + 511) / 512 + 1) * 512;
    }
}

void TARFree(TAREntry *tar)
{

    for (uint64_t i = 0; i < tar->fileCount; ++i)
    {
        kfree(tar->files[i].name);
        kfree(tar->files[i].content);
    }

    kfree(tar->files);
}
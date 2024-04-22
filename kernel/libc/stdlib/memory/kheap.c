#include <libc/stdlib/memory/kheap.h>
#include <libc/stdlib/memory/pmm.h>
#include <sys/logger.h>

void *kmalloc(size_t size)
{
    char *PP = (char *)pmm_request_pages((size / PAGE_SIZE) + 1);
    if (PP == NULL)
    {
        cdebug_log(__func__, "Failed to allocate memory");
        return NULL;
    }
    *((size_t *)PP) = size;
    PP += sizeof(size_t);
    return (void *)PP;
}

void kfree(void *tofree)
{
    if (tofree == NULL)
    {
        return;
    }
    char *PP = (char *)tofree;
    PP -= sizeof(size_t);
    size_t size = *((size_t *)PP);
    pmm_free_pages(PP, (size / PAGE_SIZE) + 1);
}

void *kcalloc(size_t size)
{
    void *kmallocVal = kmalloc(size);
    if (kmallocVal == NULL)
    {
        return NULL;
    }
    memset(kmallocVal, 0, size);
    return kmallocVal;
}

void *krealloc(void *old, size_t size)
{
    if (old == NULL)
    {
        return kmalloc(size);
    }

    size_t old_size = *((size_t *)old - 1);

    size_t new_num_pages = (size / PAGE_SIZE) + 1;
    size_t old_num_pages = (old_size / PAGE_SIZE) + 1;

    if (new_num_pages > old_num_pages)
    {
        void *new_mem = pmm_request_pages(new_num_pages - old_num_pages);
        if (new_mem == NULL)
        {
            cdebug_log(__func__, "Failed to allocate additional memory pages");
            return NULL;
        }

        memcpy(new_mem, old, old_size);
        kfree(old);

        return new_mem;
    }
    else
    {
        return old;
    }
}

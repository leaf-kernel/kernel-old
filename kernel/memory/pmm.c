#include <memory/pmm.h>
#include <bitmap/bitmap.h>
#include <sys/logger.h>
#include <libc/math.h>
#include <sys/limine.h>
#include <sys/leaf.h>
#include <utils/convertion.h>

volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST, .revision = 0};
volatile struct limine_memmap_response *memmap;

uint8_t *bitmap;
uint64_t bitmap_pages;
uint64_t bitmap_size;
uint64_t free_memory;
uint64_t total_memory;

void init_pmm()
{
    memmap = memmap_request.response;
    uint64_t top_address;
    uint64_t higher_address = 0;
    uint64_t hhdm_offset = hhdm_request.response->offset;
    free_memory = 0;
    total_memory = 0;

    for (uint64_t entryCount = 0; entryCount < memmap->entry_count; entryCount++)
    {
        struct limine_memmap_entry *entry = memmap->entries[entryCount];

        if (entry->type == LIMINE_MEMMAP_USABLE)
        {
            top_address = entry->base + entry->length;
            if (top_address > higher_address)
                higher_address = top_address;
        }
    }
    bitmap_pages = higher_address / PAGE_SIZE;
    bitmap_size = ALIGN_UP(bitmap_pages / 8, PAGE_SIZE);

    for (uint64_t entryCount = 0; entryCount < memmap->entry_count; entryCount++)
    {
        struct limine_memmap_entry *entry = memmap->entries[entryCount];

        if (entry->type == LIMINE_MEMMAP_USABLE)
        {
            if (entry->length >= bitmap_size)
            {
                bitmap = (uint8_t *)(entry->base + hhdm_offset);
                memset(bitmap, 0xFF, bitmap_size);
                entry->base += bitmap_size;
                entry->length -= bitmap_size;
                break;
            }
        }
    }

    for (uint64_t entryCount = 0; entryCount < memmap->entry_count; entryCount++)
    {
        struct limine_memmap_entry *entry = memmap->entries[entryCount];

        if (entry->type == LIMINE_MEMMAP_USABLE)
        {
            for (uint64_t i = 0; i < entry->length; i += PAGE_SIZE)
            {
                bitmap_clear(bitmap, (entry->base + i) / PAGE_SIZE);
            }
        }
    }

    update_memory();
    cdebug_log(__func__, "done.");
}

void update_memory()
{
    free_memory = 0;
    total_memory = 0;

    for (uint64_t i = 0; i < bitmap_pages; i++)
    {
        if (!bitmap_get(bitmap, i))
        {
            free_memory += PAGE_SIZE;
        }
    }

    for (uint64_t entryCount = 0; entryCount < memmap->entry_count; entryCount++)
    {
        struct limine_memmap_entry *entry = memmap->entries[entryCount];
        if (entry->type == LIMINE_MEMMAP_USABLE)
        {
            total_memory += entry->length;
        }
    }
}


void *pmm_request_page()
{
    uint64_t last_bit_val;
    uint64_t last_allocated_index = 0;

    while (1)
    {
        last_bit_val = bitmap_get(bitmap, last_allocated_index);
        if (last_bit_val == 0)
        {
            return (void *)(last_allocated_index * PAGE_SIZE);
        }
        else
        {
            if (last_allocated_index >= bitmap_pages)
            {
                return NULL;
            }
            else
            {
                last_allocated_index++;
            }
        }
    }
}

void *pmm_request_pages(size_t numPages)
{
    uint64_t last_allocated_index = 0;

    while (1)
    {
        if (!bitmap_get(bitmap, last_allocated_index))
        {
            size_t consecutive_free_pages = 1;

            for (size_t i = 1; i < numPages; ++i)
            {
                if (!bitmap_get(bitmap, last_allocated_index + i))
                {
                    ++consecutive_free_pages;
                }
                else
                {
                    consecutive_free_pages = 0;
                    break;
                }
            }

            if (consecutive_free_pages == numPages)
            {
                for (size_t i = 0; i < numPages; ++i)
                {
                    bitmap_set(bitmap, last_allocated_index + i);
                }

                return (void *)(last_allocated_index * PAGE_SIZE);
            }
        }

        ++last_allocated_index;

        if (last_allocated_index >= bitmap_pages)
        {
            return NULL;
        }
    }
}

void pmm_free(void *ptr)
{
    uint64_t bit_idx = ((uint64_t)ptr / PAGE_SIZE);
    bitmap_clear(bitmap, bit_idx);
}

void pmm_free_pages(void *ptr, size_t numPages)
{
    uint64_t start_bit_idx = ((uint64_t)ptr / PAGE_SIZE);

    for (size_t i = 0; i < numPages; ++i)
    {
        bitmap_clear(bitmap, start_bit_idx + i);
    }
}

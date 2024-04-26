#include <sys/stable.h>
#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>
#include <libc/string.h>
#include <libc/stdlib/memory/kheap.h>
#include <libc/ctype.h>

table_entry_t st_entries[MAX_STABLE_COUNT];
#define MAX_MAP_LINES MAX_STABLE_COUNT

uint64_t _hex_to_u64(char *hex)
{
    uint64_t val = 0;
    while (*hex)
    {
        uint8_t byte = *hex++;
        if (byte >= '0' && byte <= '9')
            byte = byte - '0';
        else if (byte >= 'a' && byte <= 'f')
            byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <= 'F')
            byte = byte - 'A' + 10;
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}

table_entry_t _parse_entry(char *line)
{
    table_entry_t entry = {
        .name = NULL,
        .addr = 0};

    size_t length = 0;
    while (line[length] != ' ' && line[length] != '\0')
    {
        ++length;
    }

    char *address = (char *)kmalloc(length + 1);
    if (address == NULL)
    {
        dlog("Memory allocation failed");
        return entry;
    }

    strncpy(address, line, length);
    address[length] = '\0';

    entry.addr = _hex_to_u64(address);
    kfree(address);

    if (line[length + 1] == 'T')
    {
        char *name_start = line + length + 3;
        size_t name_length = 0;
        while (name_start[name_length] != ' ' && name_start[name_length] != '\0')
        {
            ++name_length;
        }

        entry.name = (char *)kmalloc(name_length + 1);
        if (entry.name == NULL)
        {
            dlog("Memory allocation failed for name");
            return entry;
        }

        strncpy(entry.name, name_start, name_length);
        entry.name[name_length] = '\0';
    }
    if (entry.name != NULL)
    {
        cdlog("Address: 0x%016x Label: %s", entry.addr, entry.name);
        return entry;
    }
    else
    {
        return (table_entry_t){0};
    }
}

void init_stable()
{
    vfs_op_status status;
    char *map = (char *)kmalloc(sizeof(char));
    size_t map_size = sizeof(char);
    if (map == NULL)
    {
        dlog("Failed to allocate space for map buffer!");
        return;
    }

    VFS_t *vfs = (VFS_t *)__LEAF_GET_VFS__();

    status = drive_read(vfs, 0, "/sys/kernel/kernel.map", &map);

    if (status != STATUS_OK)
    {
        dlog("Failed to read /sys/kernel/kernel.map!");
        kfree(map);
        return;
    }

    char **map_lines = (char **)kmalloc(sizeof(char *) * MAX_MAP_LINES);
    size_t *line_sizes = (size_t *)kmalloc(sizeof(size_t) * MAX_MAP_LINES);
    if (map_lines == NULL || line_sizes == NULL)
    {
        dlog("Failed to allocate space for map lines!");
        kfree(map);
        kfree(map_lines);
        kfree(line_sizes);
        return;
    }

    memset(map_lines, 0, sizeof(char *) * MAX_MAP_LINES);

    char *token = strtok(map, "\n");
    int line_count = 0;
    while (token != NULL)
    {
        if (line_count >= MAX_MAP_LINES)
        {
            dlog("Exceeded maximum number of map lines!");
            break;
        }

        size_t token_len = strlen(token) + 1;
        if (map_size + token_len > SIZE_MAX)
        {
            dlog("Map size exceeds maximum allowed size!");
            break;
        }

        char *new_map = (char *)krealloc(map, map_size + token_len);
        if (new_map == NULL)
        {
            dlog("Failed to reallocate memory for map buffer!");
            for (int i = 0; i < line_count; ++i)
                kfree(map_lines[i]);
            kfree(map_lines);
            kfree(line_sizes);
            kfree(map);
            return;
        }
        map = new_map;
        strcpy(map + map_size - 1, token);
        map_size += token_len;

        map_lines[line_count] = map + map_size - token_len;
        line_sizes[line_count] = token_len;
        line_count++;

        token = strtok(NULL, "\n");
    }

    for (int i = 0; i < line_count; ++i)
    {
        table_entry_t entry = _parse_entry(map_lines[i]);
        (void)entry;
    }

    for (int i = 0; i < line_count; ++i)
        kfree(map_lines[i]);
    kfree(map_lines);
    kfree(line_sizes);
    kfree(map);
}

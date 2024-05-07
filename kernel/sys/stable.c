#include <sys/stable.h>
#define LEAF_INCLUDE_PRIVATE
#include <libc/ctype.h>
#include <libc/stdlib/memory/kheap.h>
#include <libc/string.h>
#include <sys/leaf.h>

table_entry_t st_entries[MAX_STABLE_COUNT];
int line_count;
int st_entry_count;

uint8_t _hex_to_u8(char d) {
    const char *hexdigits = "0123456789abcdef";
    return strchr(hexdigits, d) - hexdigits;
}

uint64_t _hex_to_u64(const char *hex) {
    uint64_t result = 0;
    size_t len = strlen(hex);

    for(size_t i = 0; i < len; i++) {
        result = (result << 4) + _hex_to_u8(hex[i]);
    }

    return result;
}

table_entry_t _parse_entry(char *line) {
    table_entry_t entry = {.name = NULL, .addr = 0, .id = '\0'};

    size_t length = strcspn(line, " ");
    char *address = kmalloc(length + 1);
    if(address == NULL) {
        fail("Memory allocation failed");
        return entry;
    }

    strncpy(address, line, length);
    address[length] = '\0';

    entry.addr = _hex_to_u64(address);
    kfree(address);

    entry.id = line[length + 1];

    char *name_start = line + length + 3;
    size_t name_length = strcspn(name_start, " ");
    entry.name = kmalloc(name_length + 1);
    if(entry.name == NULL) {
        fail("Memory allocation failed for name");
        return entry;
    }

    strncpy(entry.name, name_start, name_length);
    entry.name[name_length] = '\0';

#ifdef __LEAF_VVVERBOSE__
    vvok("%-25s Address: 0x%-16llx ID: %-c", entry.name, entry.addr, entry.id);
#endif

    return entry;
}

void init_stable() {
    vfs_op_status status;
    char *map;
    VFS_t *vfs = (VFS_t *)__LEAF_GET_VFS__();

    // Read the map of the ramdisk. ID: 0
    status = drive_read(vfs, 0, "/sys/kernel/kernel.map", &map);
    if(status != STATUS_OK) {
        fail("Failed to read /sys/kernel/kernel.map!");
        hcf();
        return;
    }

    char **map_lines = kmalloc(sizeof(char *) * MAX_STABLE_COUNT);
    if(map_lines == NULL) {
        fail("Failed to allocate space for map lines!");
        kfree(map);
        return;
    }

    char *token = strtok(map, "\n");
    line_count = 0;
    while(token != NULL && line_count < MAX_STABLE_COUNT) {
        map_lines[line_count] = strdup(token);
        if(map_lines[line_count] == NULL) {
            fail("Memory allocation failed for map line");
            for(int i = 0; i < line_count; ++i)
                kfree(map_lines[i]);
            kfree(map_lines);
            kfree(map);
            return;
        }
        line_count++;
        token = strtok(NULL, "\n");
    }
    kfree(map);

    for(int i = 0; i < line_count; ++i) {
        if(map_lines[i][0] != '\0' && map_lines[i][0] != '\n') {
            table_entry_t entry = _parse_entry(map_lines[i]);
            for(int j = 0; j < MAX_STABLE_COUNT; ++j) {
                if(st_entries[j].addr == 0) {
                    st_entries[j] = entry;
                    break;
                }
            }
            st_entry_count++;
        } else {
            break;
        }
    }

    for(int k = 0; k < line_count; k++) {
        kfree(map_lines[k]);
    }

    kfree(map_lines);
}

char *get_symbol_name(uint64_t addr) {
    for(int i = 0; i < st_entry_count; i++) {
        if(st_entries[i].addr == addr) {
            return st_entries[i].name;
        }
    }

    return NULL;
}

int get_symbol_int(uint64_t addr) {
    for(int i = 0; i < st_entry_count; i++) {
        if(st_entries[i].addr == addr) {
            return i;
        }
    }

    return -1;
}

table_entry_t *lookup_symbol(uint64_t addr) {

    for(int i = 0; i < st_entry_count; i++) {
        if(addr >= st_entries[i].addr && addr < st_entries[i + 1].addr) {
            return &st_entries[i];
        }
    }

    return NULL;
}

uint64_t get_symbol_addr(char *name) {

    for(int i = 0; i < st_entry_count; i++) {
        if(st_entries[i].name == name) {
            return st_entries[i].addr;
        }
    }

    return 0;
}
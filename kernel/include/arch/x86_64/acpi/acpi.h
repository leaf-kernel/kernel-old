#ifndef __ACPI_H_
#define __ACPI_H_

#define LEAF_INCLUDE_PRIVATE
#include <sys/leaf.h>
#include <stdint.h>
#include <stdbool.h>
#include <libc/string.h>

typedef struct
{
    char signature[8];
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    uint32_t rsdt_addr;
} __attribute__((packed)) rsdp_t;

typedef struct
{
    rsdp_t rsdp;
    uint32_t length;
    uint64_t xsdt_addr;
    uint8_t checksum;
    uint8_t reserved[3];
} __attribute__((packed)) xsdp_t;

typedef struct
{
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_tableid[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed)) sdt_t;

void init_acpi();

void *_find_sdt(char *signature);
bool _use_xsdt();

#endif // __ACPI_H_

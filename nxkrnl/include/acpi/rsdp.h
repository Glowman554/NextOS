#ifndef RSDP_H
#define RSDP_H

#include <stdint.h>
#include <mem.h>
#include <string.h>

struct acpihdr {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char OEMID[6];
    char OEM_table_id[8];
    uint32_t OEM_revision;
    uint32_t creator_ID;
    uint32_t creator_revision;
} __attribute__((packed));

struct rsdp {
    char signature[8];
    uint8_t checksum;
    char OEMID[6];
    uint8_t revision; 
    uint32_t rsdt_addr;
    uint32_t length;
    uint64_t xsdt_addr;
    uint8_t ext_checksum;
    uint8_t reserved[3];
} __attribute__((packed));

struct rsdt {
    struct acpihdr acpihdr;
    uint32_t acpiptr[];
} __attribute((packed));

struct xsdt {
    struct acpihdr acpihdr;
    uint64_t acpiptr[];
} __attribute__((packed));

extern struct rsdp* rsdp;
extern struct rsdt* rsdt;
extern struct xsdt* xsdt;


struct rsdp* scan_for_rsdp(char* start, uint32_t length);
void* find_SDT(const char *signature);
void rsdp_init();

#endif
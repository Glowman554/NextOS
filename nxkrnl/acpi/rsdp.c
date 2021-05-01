#include <acpi/rsdp.h>

struct rsdp* rsdp = NULL;
struct rsdt* rsdt = NULL;
struct xsdt* xsdt = NULL;

#pragma GCC diagnostic ignored "-Wint-to-pointer-cast" // im lazy ok

struct rsdp* scan_for_rsdp (char* start, uint32_t length) {
	char* end = start + length;

	for (; start < end; start += 16) {
		if (!memcmp(start, "RSD PTR ", sizeof("RSD PTR ") -1)) {
			return (struct rsdp*) start;
		}
	}
	return NULL;
}

void rsdp_init() {
	rsdp = scan_for_rsdp((char*) 0xE0000, 0xFFFFF);
	debug_write("rsdp found at %x", rsdp);


	if(rsdp->xsdt_addr) {
		xsdt = (struct xsdt*)((uint64_t) rsdp->xsdt_addr);
		debug_write("xsdt found at %x", xsdt);
	} else if(rsdp->rsdt_addr) {
		rsdt = (struct rsdt*)((uint64_t) rsdp->rsdt_addr);
		debug_write("rsdt found at %x", rsdt);
	}
}

void* find_SDT(const char *signature) {
    if(xsdt != NULL) {
        for(uint64_t i = 0; i < (xsdt->acpihdr.length - sizeof(struct acpihdr)); i++) {
            struct acpihdr *acpihdr = (struct acpihdr*)(xsdt->acpiptr[i]);
            if(strncmp(acpihdr->signature, signature, 4) == 0) {
                debug_write("%s found\n", signature);
                return acpihdr;
            }
        }
    } 

    if(rsdt != NULL) {
        for(uint64_t i = 0; i < (rsdt->acpihdr.length - sizeof(struct acpihdr)); i++) {
            struct acpihdr *acpihdr = (struct acpihdr*)(rsdt->acpiptr[i]);
            if(strncmp(acpihdr->signature, signature, 4) == 0) {
                debug_write("%s found\n", signature);
                return acpihdr;
            }
        }
    } 

    debug_write("%s not found\n", signature);

    return NULL;
}
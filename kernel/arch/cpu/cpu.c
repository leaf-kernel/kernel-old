#include <arch/cpu/cpu.h>
#include <sys/backtrace.h>
#include <sys/stable.h>

void hcf() {
    asm("cli");
    hlt();
}

void hlt() {
    for(;;) {
        asm("hlt");
    }
}

void panic(const char *reason, const char *description, int_frame_t frame,
           void *rbp) {
    // TODO: Get the CPU ID
    int cpuId = 1;
    fatal("panic(cpu %d @ 0x%016llx) type: %d (Name: %s)!\r\n", cpuId,
          frame.rip, frame.vector, reason);
    __LEAF_ENABLE_PRE_LOG();
    ok("panic(cpu %d @ 0x%016llx) type: %d (Name: %s)!", cpuId, frame.rip,
       frame.vector, reason);
    __LEAF_ENABLE_PRE_LOG();
#ifdef LEAF_DEBUG
    printf("Description: %s\r\n", description);
    printf("Register dump:\r\n");
    printf(
        "  rax: 0x%.16llx, rbx: 0x%.16llx, rcx: 0x%.16llx, rdx: 0x%.16llx\r\n",
        frame.rax, frame.rbx, frame.rcx, frame.rdx);
    printf(
        "  rsp: 0x%.16llx, rbp: 0x%.16llx, rsi: 0x%.16llx, rdi: 0x%.16llx\r\n",
        frame.rsp, frame.rbp, frame.rsi, frame.rdi);
    printf(
        "  r8:  0x%.16llx, r9:  0x%.16llx, r10: 0x%.16llx, r11: 0x%.16llx\r\n",
        frame.r8, frame.r9, frame.r10, frame.r11);
    printf(
        "  r12: 0x%.16llx, r13: 0x%.16llx, r14: 0x%.16llx, r15: 0x%.16llx\r\n",
        frame.r12, frame.r13, frame.r14, frame.r15);
    printf(
        "  rfl: 0x%.16llx, rip: 0x%.16llx, cs:  0x%.16llx, ss:  0x%.16llx\r\n",
        frame.rflags, frame.rip, frame.cs, frame.ss);
    printf("  ds: 0x%.16llx,  cr2: 0x%.16llx, cr3: 0x%.16llx\r\n", frame.ds,
           frame.cr2, frame.cr3);
#endif
    printf("\r\n");
    backtrace(rbp, frame.rip);
    hcf();
}

char *get_intel_brand() {
#define MAX_BRAND_STRING_LENGTH 48
    if(cpuid_string(CPUID_INTELEXTENDED, NULL) < CPUID_INTELBRANDSTRINGEND)
        return NULL;

    char *brand_string =
        (char *)kmalloc(MAX_BRAND_STRING_LENGTH * sizeof(char));
    if(!brand_string)
        return NULL;

    uint32_t brand[4];
    memset(brand, 0, sizeof(brand));

    cpuid_string(CPUID_INTELBRANDSTRING, brand);

    memcpy(brand_string, brand, sizeof(brand));

    return brand_string;
}

void get_cpu_vendor_string(char *vendor_string) {
    uint32_t vendor[4];
    cpuid_string(0, vendor);
    memcpy(vendor_string, &vendor[1], sizeof(uint32_t));
    memcpy(vendor_string + sizeof(uint32_t), &vendor[3], sizeof(uint32_t));
    memcpy(vendor_string + 2 * sizeof(uint32_t), &vendor[2], sizeof(uint32_t));
    vendor_string[12] = '\0';
}

int get_model() {
    int ebx, unused;
    __cpuid(0, unused, ebx, unused, unused);
    return ebx;
}

// Check functions

bool check_feature(uint32_t feat) {
    unsigned int eax, unused, edx;
    __get_cpuid(1, &eax, &unused, &unused, &edx);
    return edx & feat;
}

bool check_apic() { return check_feature(CPUID_FEAT_EDX_APIC); }

bool check_msr() { return check_feature(CPUID_FEAT_EDX_MSR); }

void check_all_features() {
    printf("CPU Features:\r\n");
    printf(" - SSE3: %s\r\n",
           check_feature(CPUID_FEAT_ECX_SSE3) ? "yes" : "no");
    printf(" - PCLMUL: %s\r\n",
           check_feature(CPUID_FEAT_ECX_PCLMUL) ? "yes" : "no");
    printf(" - DTES64: %s\r\n",
           check_feature(CPUID_FEAT_ECX_DTES64) ? "yes" : "no");
    printf(" - MONITOR: %s\r\n",
           check_feature(CPUID_FEAT_ECX_MONITOR) ? "yes" : "no");
    printf(" - DS_CPL: %s\r\n",
           check_feature(CPUID_FEAT_ECX_DS_CPL) ? "yes" : "no");
    printf(" - VMX: %s\r\n", check_feature(CPUID_FEAT_ECX_VMX) ? "yes" : "no");
    printf(" - SMX: %s\r\n", check_feature(CPUID_FEAT_ECX_SMX) ? "yes" : "no");
    printf(" - EST: %s\r\n", check_feature(CPUID_FEAT_ECX_EST) ? "yes" : "no");
    printf(" - TM2: %s\r\n", check_feature(CPUID_FEAT_ECX_TM2) ? "yes" : "no");
    printf(" - SSSE3: %s\r\n",
           check_feature(CPUID_FEAT_ECX_SSSE3) ? "yes" : "no");
    printf(" - CID: %s\r\n", check_feature(CPUID_FEAT_ECX_CID) ? "yes" : "no");
    printf(" - SDBG: %s\r\n",
           check_feature(CPUID_FEAT_ECX_SDBG) ? "yes" : "no");
    printf(" - FMA: %s\r\n", check_feature(CPUID_FEAT_ECX_FMA) ? "yes" : "no");
    printf(" - CX16: %s\r\n",
           check_feature(CPUID_FEAT_ECX_CX16) ? "yes" : "no");
    printf(" - XTPR: %s\r\n",
           check_feature(CPUID_FEAT_ECX_XTPR) ? "yes" : "no");
    printf(" - PDCM: %s\r\n",
           check_feature(CPUID_FEAT_ECX_PDCM) ? "yes" : "no");
    printf(" - PCID: %s\r\n",
           check_feature(CPUID_FEAT_ECX_PCID) ? "yes" : "no");
    printf(" - DCA: %s\r\n", check_feature(CPUID_FEAT_ECX_DCA) ? "yes" : "no");
    printf(" - SSE4.1: %s\r\n",
           check_feature(CPUID_FEAT_ECX_SSE4_1) ? "yes" : "no");
    printf(" - SSE4.2: %s\r\n",
           check_feature(CPUID_FEAT_ECX_SSE4_2) ? "yes" : "no");
    printf(" - X2APIC: %s\r\n",
           check_feature(CPUID_FEAT_ECX_X2APIC) ? "yes" : "no");
    printf(" - MOVBE: %s\r\n",
           check_feature(CPUID_FEAT_ECX_MOVBE) ? "yes" : "no");
    printf(" - POPCNT: %s\r\n",
           check_feature(CPUID_FEAT_ECX_POPCNT) ? "yes" : "no");
    printf(" - TSC: %s\r\n", check_feature(CPUID_FEAT_ECX_TSC) ? "yes" : "no");
    printf(" - AES: %s\r\n", check_feature(CPUID_FEAT_ECX_AES) ? "yes" : "no");
    printf(" - XSAVE: %s\r\n",
           check_feature(CPUID_FEAT_ECX_XSAVE) ? "yes" : "no");
    printf(" - OSXSAVE: %s\r\n",
           check_feature(CPUID_FEAT_ECX_OSXSAVE) ? "yes" : "no");
    printf(" - AVX: %s\r\n", check_feature(CPUID_FEAT_ECX_AVX) ? "yes" : "no");
    printf(" - F16C: %s\r\n",
           check_feature(CPUID_FEAT_ECX_F16C) ? "yes" : "no");
    printf(" - RDRAND: %s\r\n",
           check_feature(CPUID_FEAT_ECX_RDRAND) ? "yes" : "no");
    printf(" - HYPERVISOR: %s\r\n",
           check_feature(CPUID_FEAT_ECX_HYPERVISOR) ? "yes" : "no");
    printf(" - FPU: %s\r\n", check_feature(CPUID_FEAT_EDX_FPU) ? "yes" : "no");
    printf(" - VME: %s\r\n", check_feature(CPUID_FEAT_EDX_VME) ? "yes" : "no");
    printf(" - DE: %s\r\n", check_feature(CPUID_FEAT_EDX_DE) ? "yes" : "no");
    printf(" - PSE: %s\r\n", check_feature(CPUID_FEAT_EDX_PSE) ? "yes" : "no");
    printf(" - TSC: %s\r\n", check_feature(CPUID_FEAT_EDX_TSC) ? "yes" : "no");
    printf(" - MSR: %s\r\n", check_feature(CPUID_FEAT_EDX_MSR) ? "yes" : "no");
    printf(" - PAE: %s\r\n", check_feature(CPUID_FEAT_EDX_PAE) ? "yes" : "no");
    printf(" - MCE: %s\r\n", check_feature(CPUID_FEAT_EDX_MCE) ? "yes" : "no");
    printf(" - CX8: %s\r\n", check_feature(CPUID_FEAT_EDX_CX8) ? "yes" : "no");
    printf(" - APIC: %s\r\n",
           check_feature(CPUID_FEAT_EDX_APIC) ? "yes" : "no");
    printf(" - SEP: %s\r\n", check_feature(CPUID_FEAT_EDX_SEP) ? "yes" : "no");
    printf(" - MTRR: %s\r\n",
           check_feature(CPUID_FEAT_EDX_MTRR) ? "yes" : "no");
    printf(" - PGE: %s\r\n", check_feature(CPUID_FEAT_EDX_PGE) ? "yes" : "no");
    printf(" - MCA: %s\r\n", check_feature(CPUID_FEAT_EDX_MCA) ? "yes" : "no");
    printf(" - CMOV: %s\r\n",
           check_feature(CPUID_FEAT_EDX_CMOV) ? "yes" : "no");
    printf(" - PAT: %s\r\n", check_feature(CPUID_FEAT_EDX_PAT) ? "yes" : "no");
    printf(" - PSE36: %s\r\n",
           check_feature(CPUID_FEAT_EDX_PSE36) ? "yes" : "no");
    printf(" - PSN: %s\r\n", check_feature(CPUID_FEAT_EDX_PSN) ? "yes" : "no");
    printf(" - CLFLUSH: %s\r\n",
           check_feature(CPUID_FEAT_EDX_CLFLUSH) ? "yes" : "no");
    printf(" - DS: %s\r\n", check_feature(CPUID_FEAT_EDX_DS) ? "yes" : "no");
    printf(" - ACPI: %s\r\n",
           check_feature(CPUID_FEAT_EDX_ACPI) ? "yes" : "no");
    printf(" - MMX: %s\r\n", check_feature(CPUID_FEAT_EDX_MMX) ? "yes" : "no");
    printf(" - FXSR: %s\r\n",
           check_feature(CPUID_FEAT_EDX_FXSR) ? "yes" : "no");
    printf(" - SSE: %s\r\n", check_feature(CPUID_FEAT_EDX_SSE) ? "yes" : "no");
    printf(" - SSE2: %s\r\n",
           check_feature(CPUID_FEAT_EDX_SSE2) ? "yes" : "no");
    printf(" - SS: %s\r\n", check_feature(CPUID_FEAT_EDX_SS) ? "yes" : "no");
    printf(" - HTT: %s\r\n", check_feature(CPUID_FEAT_EDX_HTT) ? "yes" : "no");
    printf(" - TM: %s\r\n", check_feature(CPUID_FEAT_EDX_TM) ? "yes" : "no");
    printf(" - IA64: %s\r\n",
           check_feature(CPUID_FEAT_EDX_IA64) ? "yes" : "no");
    printf(" - PBE: %s\r\n", check_feature(CPUID_FEAT_EDX_PBE) ? "yes" : "no");
}

// MSR
void get_msr(uint32_t msr, uint32_t *lo, uint32_t *hi) {
    asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

void set_msr(uint32_t msr, uint32_t lo, uint32_t hi) {
    asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}

uint64_t _read_reg() { return 0; }

void _write_reg() { __LEAF_VOID_REDIRECT__; }

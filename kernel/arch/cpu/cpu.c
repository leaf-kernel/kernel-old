#include <arch/cpu/cpu.h>
#include <sys/stable.h>

void hcf()
{
    asm("cli");
    hlt();
}

void hlt()
{
    for (;;)
    {
        asm("hlt");
    }
}

void panic(const char *reason, const char *description, int_frame_t frame)
{
    // TODO: Get the CPU ID
    int cpuId = 1;
    dprintf("\n\npanic(cpu %d @ 0x%016llx) type: %d (Name: %s)!\n", cpuId, frame.rip, frame.vector, reason);
#ifdef LEAF_DEBUG
    dprintf("Description: %s\n", description);
    dprintf("Register dump:\n");
    dprintf("  rax: 0x%.16llx, rbx: 0x%.16llx, rcx: 0x%.16llx, rdx: 0x%.16llx\n",
            frame.rax, frame.rbx, frame.rcx, frame.rdx);
    dprintf("  rsp: 0x%.16llx, rbp: 0x%.16llx, rsi: 0x%.16llx, rdi: 0x%.16llx\n",
            frame.rsp, frame.rbp, frame.rsi, frame.rdi);
    dprintf("  r8:  0x%.16llx, r9:  0x%.16llx, r10: 0x%.16llx, r11: 0x%.16llx\n",
            frame.r8, frame.r9, frame.r10, frame.r11);
    dprintf("  r12: 0x%.16llx, r13: 0x%.16llx, r14: 0x%.16llx, r15: 0x%.16llx\n",
            frame.r12, frame.r13, frame.r14, frame.r15);
    dprintf("  rfl: 0x%.16llx, rip: 0x%.16llx, cs:  0x%.16llx, ss:  0x%.16llx\n",
            frame.rflags, frame.rip, frame.cs, frame.ss);
#endif

    table_entry_t entry = get_symbol(frame.rip);
    dprintf("\nfunction: %s\n", entry.name);
    hcf();
}

// Utils functions (CPUID)
void get_intel_cpu_brand_string(char *brand_string)
{
    uint32_t brand[12];

    cpuid_string(CPUID_INTELBRANDSTRING, brand);
    cpuid_string(CPUID_INTELBRANDSTRINGMORE, brand + 4);
    cpuid_string(CPUID_INTELBRANDSTRINGEND, brand + 8);

    memcpy(brand_string, brand, sizeof(brand));
    brand_string[sizeof(brand)] = '\0';
}

void get_cpu_vendor_string(char *vendor_string)
{
    uint32_t vendor[4];
    cpuid_string(0, vendor);
    memcpy(vendor_string, &vendor[1], sizeof(uint32_t));
    memcpy(vendor_string + sizeof(uint32_t), &vendor[3], sizeof(uint32_t));
    memcpy(vendor_string + 2 * sizeof(uint32_t), &vendor[2], sizeof(uint32_t));
    vendor_string[12] = '\0';
}

int get_model()
{
    int ebx, unused;
    __cpuid(0, unused, ebx, unused, unused);
    return ebx;
}

// Check functions

bool check_feature(uint32_t feat)
{
    unsigned int eax, unused, edx;
    __get_cpuid(1, &eax, &unused, &unused, &edx);
    return edx & feat;
}

bool check_apic()
{
    return check_feature(CPUID_FEAT_EDX_APIC);
}

bool check_msr()
{
    return check_feature(CPUID_FEAT_EDX_MSR);
}

void check_all_features()
{
    dprintf("CPU Features:\n");
    dprintf(" - SSE3: %s\n", check_feature(CPUID_FEAT_ECX_SSE3) ? "yes" : "no");
    dprintf(" - PCLMUL: %s\n", check_feature(CPUID_FEAT_ECX_PCLMUL) ? "yes" : "no");
    dprintf(" - DTES64: %s\n", check_feature(CPUID_FEAT_ECX_DTES64) ? "yes" : "no");
    dprintf(" - MONITOR: %s\n", check_feature(CPUID_FEAT_ECX_MONITOR) ? "yes" : "no");
    dprintf(" - DS_CPL: %s\n", check_feature(CPUID_FEAT_ECX_DS_CPL) ? "yes" : "no");
    dprintf(" - VMX: %s\n", check_feature(CPUID_FEAT_ECX_VMX) ? "yes" : "no");
    dprintf(" - SMX: %s\n", check_feature(CPUID_FEAT_ECX_SMX) ? "yes" : "no");
    dprintf(" - EST: %s\n", check_feature(CPUID_FEAT_ECX_EST) ? "yes" : "no");
    dprintf(" - TM2: %s\n", check_feature(CPUID_FEAT_ECX_TM2) ? "yes" : "no");
    dprintf(" - SSSE3: %s\n", check_feature(CPUID_FEAT_ECX_SSSE3) ? "yes" : "no");
    dprintf(" - CID: %s\n", check_feature(CPUID_FEAT_ECX_CID) ? "yes" : "no");
    dprintf(" - SDBG: %s\n", check_feature(CPUID_FEAT_ECX_SDBG) ? "yes" : "no");
    dprintf(" - FMA: %s\n", check_feature(CPUID_FEAT_ECX_FMA) ? "yes" : "no");
    dprintf(" - CX16: %s\n", check_feature(CPUID_FEAT_ECX_CX16) ? "yes" : "no");
    dprintf(" - XTPR: %s\n", check_feature(CPUID_FEAT_ECX_XTPR) ? "yes" : "no");
    dprintf(" - PDCM: %s\n", check_feature(CPUID_FEAT_ECX_PDCM) ? "yes" : "no");
    dprintf(" - PCID: %s\n", check_feature(CPUID_FEAT_ECX_PCID) ? "yes" : "no");
    dprintf(" - DCA: %s\n", check_feature(CPUID_FEAT_ECX_DCA) ? "yes" : "no");
    dprintf(" - SSE4.1: %s\n", check_feature(CPUID_FEAT_ECX_SSE4_1) ? "yes" : "no");
    dprintf(" - SSE4.2: %s\n", check_feature(CPUID_FEAT_ECX_SSE4_2) ? "yes" : "no");
    dprintf(" - X2APIC: %s\n", check_feature(CPUID_FEAT_ECX_X2APIC) ? "yes" : "no");
    dprintf(" - MOVBE: %s\n", check_feature(CPUID_FEAT_ECX_MOVBE) ? "yes" : "no");
    dprintf(" - POPCNT: %s\n", check_feature(CPUID_FEAT_ECX_POPCNT) ? "yes" : "no");
    dprintf(" - TSC: %s\n", check_feature(CPUID_FEAT_ECX_TSC) ? "yes" : "no");
    dprintf(" - AES: %s\n", check_feature(CPUID_FEAT_ECX_AES) ? "yes" : "no");
    dprintf(" - XSAVE: %s\n", check_feature(CPUID_FEAT_ECX_XSAVE) ? "yes" : "no");
    dprintf(" - OSXSAVE: %s\n", check_feature(CPUID_FEAT_ECX_OSXSAVE) ? "yes" : "no");
    dprintf(" - AVX: %s\n", check_feature(CPUID_FEAT_ECX_AVX) ? "yes" : "no");
    dprintf(" - F16C: %s\n", check_feature(CPUID_FEAT_ECX_F16C) ? "yes" : "no");
    dprintf(" - RDRAND: %s\n", check_feature(CPUID_FEAT_ECX_RDRAND) ? "yes" : "no");
    dprintf(" - HYPERVISOR: %s\n", check_feature(CPUID_FEAT_ECX_HYPERVISOR) ? "yes" : "no");
    dprintf(" - FPU: %s\n", check_feature(CPUID_FEAT_EDX_FPU) ? "yes" : "no");
    dprintf(" - VME: %s\n", check_feature(CPUID_FEAT_EDX_VME) ? "yes" : "no");
    dprintf(" - DE: %s\n", check_feature(CPUID_FEAT_EDX_DE) ? "yes" : "no");
    dprintf(" - PSE: %s\n", check_feature(CPUID_FEAT_EDX_PSE) ? "yes" : "no");
    dprintf(" - TSC: %s\n", check_feature(CPUID_FEAT_EDX_TSC) ? "yes" : "no");
    dprintf(" - MSR: %s\n", check_feature(CPUID_FEAT_EDX_MSR) ? "yes" : "no");
    dprintf(" - PAE: %s\n", check_feature(CPUID_FEAT_EDX_PAE) ? "yes" : "no");
    dprintf(" - MCE: %s\n", check_feature(CPUID_FEAT_EDX_MCE) ? "yes" : "no");
    dprintf(" - CX8: %s\n", check_feature(CPUID_FEAT_EDX_CX8) ? "yes" : "no");
    dprintf(" - APIC: %s\n", check_feature(CPUID_FEAT_EDX_APIC) ? "yes" : "no");
    dprintf(" - SEP: %s\n", check_feature(CPUID_FEAT_EDX_SEP) ? "yes" : "no");
    dprintf(" - MTRR: %s\n", check_feature(CPUID_FEAT_EDX_MTRR) ? "yes" : "no");
    dprintf(" - PGE: %s\n", check_feature(CPUID_FEAT_EDX_PGE) ? "yes" : "no");
    dprintf(" - MCA: %s\n", check_feature(CPUID_FEAT_EDX_MCA) ? "yes" : "no");
    dprintf(" - CMOV: %s\n", check_feature(CPUID_FEAT_EDX_CMOV) ? "yes" : "no");
    dprintf(" - PAT: %s\n", check_feature(CPUID_FEAT_EDX_PAT) ? "yes" : "no");
    dprintf(" - PSE36: %s\n", check_feature(CPUID_FEAT_EDX_PSE36) ? "yes" : "no");
    dprintf(" - PSN: %s\n", check_feature(CPUID_FEAT_EDX_PSN) ? "yes" : "no");
    dprintf(" - CLFLUSH: %s\n", check_feature(CPUID_FEAT_EDX_CLFLUSH) ? "yes" : "no");
    dprintf(" - DS: %s\n", check_feature(CPUID_FEAT_EDX_DS) ? "yes" : "no");
    dprintf(" - ACPI: %s\n", check_feature(CPUID_FEAT_EDX_ACPI) ? "yes" : "no");
    dprintf(" - MMX: %s\n", check_feature(CPUID_FEAT_EDX_MMX) ? "yes" : "no");
    dprintf(" - FXSR: %s\n", check_feature(CPUID_FEAT_EDX_FXSR) ? "yes" : "no");
    dprintf(" - SSE: %s\n", check_feature(CPUID_FEAT_EDX_SSE) ? "yes" : "no");
    dprintf(" - SSE2: %s\n", check_feature(CPUID_FEAT_EDX_SSE2) ? "yes" : "no");
    dprintf(" - SS: %s\n", check_feature(CPUID_FEAT_EDX_SS) ? "yes" : "no");
    dprintf(" - HTT: %s\n", check_feature(CPUID_FEAT_EDX_HTT) ? "yes" : "no");
    dprintf(" - TM: %s\n", check_feature(CPUID_FEAT_EDX_TM) ? "yes" : "no");
    dprintf(" - IA64: %s\n", check_feature(CPUID_FEAT_EDX_IA64) ? "yes" : "no");
    dprintf(" - PBE: %s\n", check_feature(CPUID_FEAT_EDX_PBE) ? "yes" : "no");
}

// MSR
void get_msr(uint32_t msr, uint32_t *lo, uint32_t *hi)
{
    asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

void set_msr(uint32_t msr, uint32_t lo, uint32_t hi)
{
    asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}

uint64_t _read_reg()
{
    return 0;
}

void _write_reg()
{
    __LEAF_VOID_REDIRECT__;
}

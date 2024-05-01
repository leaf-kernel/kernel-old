#ifndef __FADT_H__
#define __FADT_H__

#include <arch/x86_64/acpi/rsdt.h>
#include <arch/cpu/cpu.h>
#include <stdint.h>

typedef struct
{
    uint8_t AddressSpace;
    uint8_t BitWidth;
    uint8_t BitOffset;
    uint8_t AccessSize;
    uint64_t Address;
} __attribute((packed)) generic_address_t;

typedef struct
{
    struct ACPISDTHeader h;
    uint32_t FirmwareCtrl;
    uint32_t Dsdt;
    uint8_t Reserved;
    uint8_t PreferredPowerManagementProfile;
    uint16_t SCI_Interrupt;
    uint32_t SMI_CommandPort;
    uint8_t AcpiEnable;
    uint8_t AcpiDisable;
    uint8_t S4BIOS_REQ;
    uint8_t PSTATE_Control;
    uint32_t PM1aEventBlock;
    uint32_t PM1bEventBlock;
    uint32_t PM1aControlBlock;
    uint32_t PM1bControlBlock;
    uint32_t PM2ControlBlock;
    uint32_t PMTimerBlock;
    uint32_t GPE0Block;
    uint32_t GPE1Block;
    uint8_t PM1EventLength;
    uint8_t PM1ControlLength;
    uint8_t PM2ControlLength;
    uint8_t PMTimerLength;
    uint8_t GPE0Length;
    uint8_t GPE1Length;
    uint8_t GPE1Base;
    uint8_t CStateControl;
    uint16_t WorstC2Latency;
    uint16_t WorstC3Latency;
    uint16_t FlushSize;
    uint16_t FlushStride;
    uint8_t DutyOffset;
    uint8_t DutyWidth;
    uint8_t DayAlarm;
    uint8_t MonthAlarm;
    uint8_t Century;
    uint16_t BootArchitectureFlags;
    uint8_t Reserved2;
    uint32_t Flags;
    generic_address_t ResetReg;
    uint8_t ResetValue;
    uint8_t Reserved3[3];
    uint64_t X_FirmwareControl;
    uint64_t X_Dsdt;
    generic_address_t X_PM1aEventBlock;
    generic_address_t X_PM1bEventBlock;
    generic_address_t X_PM1aControlBlock;
    generic_address_t X_PM1bControlBlock;
    generic_address_t X_PM2ControlBlock;
    generic_address_t X_PMTimerBlock;
    generic_address_t X_GPE0Block;
    generic_address_t X_GPE1Block;
} __attribute((packed)) fadt_t;

void init_fadt(fadt_t *fadt);

#endif // __FADT_H__
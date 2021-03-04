/** @file
  Platform specific defines for constructing ACPI tables

  Copyright (c) 2012, 2013, Red Hat, Inc.
  Copyright (c) 2008, Intel Corporation. All rights reserved.<BR>
  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
  All intellectual property rights(Copyright, Patent and Trademark) reserved.

  Any violations of copyright or other intellectual property rights of the Loongson
  Technology Corporation Limited will be held accountable in accordance with the law,
  if you (or any of your subsidiaries, corporate affiliates or agents) initiate directly
  or indirectly any Intellectual Property Assertion or Intellectual Property Litigation:
  (i) against Loongson Technology Corporation Limited or any of its subsidiaries or corporate affiliates,
  (ii) against any party if such Intellectual Property Assertion or Intellectual Property Litigation arises
  in whole or in part from any software, technology, product or service of Loongson Technology Corporation Limited
  or any of its subsidiaries or corporate affiliates, or (iii) against any party relating to the Software.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION).

**/

#ifndef _Platform_h_INCLUDED_
#define _Platform_h_INCLUDED_

#include <PiDxe.h>
#include <IndustryStandard/Acpi.h>

//
// ACPI table information used to initialize tables.
//
#define EFI_ACPI_OEM_ID           'L','O','O','N','G',' '   // OEMID 6 bytes long
#define EFI_ACPI_OEM_TABLE_ID     SIGNATURE_64('L','O','O','N','G','S','O','N') // OEM table id 8 bytes long
#define EFI_ACPI_OEM_REVISION     0x20130221
#define EFI_ACPI_CREATOR_ID       SIGNATURE_32('L','G','S','N')
#define EFI_ACPI_CREATOR_REVISION 0x00000099

#define SCI_INT_VECTOR  0x0009
#define SMI_CMD_IO_PORT 0xB2
#define ACPI_ENABLE     0xF1
#define ACPI_DISABLE    0xF0
#define S4BIOS_REQ      0x00
#define PM1a_EVT_BLK    0x0000b000
#define PM1a_CNT_BLK    0x0000b004
#define PM_TMR_BLK      0x0000b008
#define GPE0_BLK        0x0000afe0
#define PM1_EVT_LEN     0x04
#define PM1_CNT_LEN     0x02
#define PM_TM_LEN       0x04
#define GPE0_BLK_LEN    0x04
#define RESERVED        0x00
#define P_LVL2_LAT      0x0065
#define P_LVL3_LAT      0x03E9
#define FLUSH_SIZE      0x0000
#define FLUSH_STRIDE    0x0000
#define DUTY_OFFSET     0x00
#define DUTY_WIDTH      0x00
#define DAY_ALRM        0x00
#define MON_ALRM        0x00
#define CENTURY         0x00
#define FLAG            (EFI_ACPI_2_0_WBINVD | \
                         EFI_ACPI_2_0_PROC_C1 | \
                         EFI_ACPI_2_0_SLP_BUTTON | \
                         EFI_ACPI_2_0_RTC_S4 | \
                         EFI_ACPI_2_0_RESET_REG_SUP)
#define RESET_REG       0xCF9
#define RESET_VALUE     (BIT2 | BIT1) // PIIX3 Reset CPU + System Reset

//
// Byte-aligned IO port register block initializer for
// EFI_ACPI_2_0_GENERIC_ADDRESS_STRUCTURE
//
#define GAS2_IO(Base, Size) {                             \
          EFI_ACPI_2_0_SYSTEM_IO, /* AddressSpaceId    */ \
          (Size) * 8,             /* RegisterBitWidth  */ \
          0,                      /* RegisterBitOffset */ \
          0,                      /* Reserved          */ \
          (Base)                  /* Address           */ \
          }

#endif

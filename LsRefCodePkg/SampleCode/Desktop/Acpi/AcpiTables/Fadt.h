/** @file
  This file describes the contents of the ACPI Fixed ACPI Description Table (FADT).
  Some additional ACPI values are defined in Acpi10.h, Acpi20.h, and Acpi30.h
  All changes to the FADT contents should be done in this file.

  Copyright (c) 2013-2015 Intel Corporation.
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

#ifndef _FADT_H_
#define _FADT_H_

//
// Statements that include other files
//
#include <IndustryStandard/Acpi20.h>

//
// ACPI table information used to initialize tables.
//
#define EFI_ACPI_OEM_ID           'L','O','O','N','G','S'   // OEMID 6 bytes long
#define EFI_ACPI_OEM_TABLE_ID     SIGNATURE_64('T','P','-','R','0','0',' ',' ') // OEM table id 8 bytes long
#define EFI_ACPI_OEM_REVISION     0x00000004
#define EFI_ACPI_CREATOR_ID       SIGNATURE_32('P','T','E','C')
#define EFI_ACPI_CREATOR_REVISION 0x20160527

//
// FADT Definitions
//
#define RESERVED        0x00
#define PM_PROFILE      0x01
#define SCI_INT_VECTOR  0x006f//0x6f = 47 + 64
#define PM1_EVT_LEN     0x08
#define PM1_CNT_LEN     0x04
#define PM_TM_LEN       0x04
#define GPE0_BLK_LEN    0x08
#define P_LVL2_LAT      0x0065 // > 0x64, not support C2
#define P_LVL3_LAT      0x03e9 // > 0x3e8, not support C3
#define FLAGS           (EFI_ACPI_2_0_WBINVD | EFI_ACPI_2_0_PROC_C1 | EFI_ACPI_2_0_SLP_BUTTON | EFI_ACPI_2_0_RESET_REG_SUP)

#define ACPI_BASE_ADDR                 0x00000000100d0000

//RST_CNT
#define RESET_REG_ADDRESS_SPACE_ID     EFI_ACPI_2_0_SYSTEM_MEMORY
#define RESET_REG_BIT_WIDTH            0x20
#define RESET_REG_BIT_OFFSET           0x00
#define RESET_REG_ADDRESS              (ACPI_BASE_ADDR + 0x30)
#define RESET_VALUE                    0x01

//PM1_STS & PM1_EN
#define PM1a_EVT_BLK_ADDRESS_SPACE_ID  EFI_ACPI_2_0_SYSTEM_MEMORY
#define PM1a_EVT_BLK_BIT_WIDTH         0x40
#define PM1a_EVT_BLK_BIT_OFFSET        0x00
#define PM1a_EVT_BLK_ADDRESS           (ACPI_BASE_ADDR + 0x0c)

//PM1_CNT
#define PM1a_CNT_BLK_ADDRESS_SPACE_ID  EFI_ACPI_2_0_SYSTEM_MEMORY
#define PM1a_CNT_BLK_BIT_WIDTH         0x20
#define PM1a_CNT_BLK_BIT_OFFSET        0x00
#define PM1a_CNT_BLK_ADDRESS           (ACPI_BASE_ADDR + 0x14)

//PM_TMR
#define PM_TMR_BLK_ADDRESS_SPACE_ID    EFI_ACPI_2_0_SYSTEM_MEMORY
#define PM_TMR_BLK_BIT_WIDTH           0x20
#define PM_TMR_BLK_BIT_OFFSET          0x00
#define PM_TMR_BLK_ADDRESS             (ACPI_BASE_ADDR + 0x18)

//GPE0_STS & GPE0_EN
#define GPE0_BLK_ADDRESS_SPACE_ID      EFI_ACPI_2_0_SYSTEM_MEMORY
#define GPE0_BLK_BIT_WIDTH             0x40
#define GPE0_BLK_BIT_OFFSET            0x00
#define GPE0_BLK_ADDRESS               (ACPI_BASE_ADDR + 0x28)
#endif

/** @file
  This file describes the contents of the ACPI System Resource Affinity Table (SRAT).
  Some additional ACPI values are defined in Acpi10.h, Acpi20.h, and Acpi30.h
  All changes to the SRAT contents should be done in this file.

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

#ifndef _SRAT_H_
#define _SRAT_H_

#include <IndustryStandard/Acpi30.h>

//
// ACPI table information used to initialize tables.
//

#define RESERVED8                 0x00
#define RESERVED16                0x0000
#define RESERVED32                0x00000000
#define RESERVED64                0x0000000000000000

#define CORE_PER_NODE             4
#define PROC_TYPE                 EFI_ACPI_3_0_PROCESSOR_LOCAL_APIC_SAPIC_AFFINITY
#define PROC_LENGTH               16
#define PROC_FLAGS                EFI_ACPI_3_0_PROCESSOR_LOCAL_APIC_SAPIC_ENABLED
#define PROC_SAPIC_EID            0
#define PROC_PROX_DOMAIN_31TO8    0x00

#define MEM_TYPE                  EFI_ACPI_3_0_MEMORY_AFFINITY
#define MEM_LENGTH                40
#define MEM_FLAGS                 EFI_ACPI_3_0_MEMORY_ENABLED

EFI_STATUS
EFIAPI
InstallAcpiSratTable (
  IN   EFI_ACPI_TABLE_PROTOCOL       *AcpiProtocol
  );

#endif

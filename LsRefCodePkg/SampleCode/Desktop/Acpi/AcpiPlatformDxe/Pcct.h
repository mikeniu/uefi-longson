/** @file
  This file describes the contents of the ACPI System Locality Distance Information Table (SLIT).
  Some additional ACPI values are defined in Acpi10.h, Acpi20.h, and Acpi30.h
  All changes to the SLIT contents should be done in this file.

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

#ifndef _PCCT_H_
#define _PCCT_H_

#include <IndustryStandard/Acpi30.h>

//
// ACPI table information used to initialize tables.
//

//
// Platform Communications Channel Table (PCCT).
// The platform communication channel is a generic mechanism for OSPM to communicate with an
// entity in the platform (e.g. a platform controller, or a Baseboard Management Controller (BMC)).
//
typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER Header;
  UINT32                      Flags;
  UINT64                      Reserved;
} EFI_ACPI_3_0_PLATFORM_COMMUNICATIONS_CHANNEL_TABLE_HEADER;

typedef struct {
  UINT8                                      Type;
  UINT8                                      Length;
  UINT32                                     Reserver;
  UINT16                                     Reserver1;
  UINT64                                     BaseAddress;
  UINT64                                     MemLength;
  EFI_ACPI_3_0_GENERIC_ADDRESS_STRUCTURE     Doorbell_Register_Address;
  UINT64                                     Doorbell_Preserve;
  UINT64                                     Doorbell_Write;
  UINT32                                     Nominal_Latency;
  UINT32                                     Maximum_Periodic_Access_Rate;
  UINT16                                     Maximum_Request_Turnaround_Time;
} EFI_ACPI_3_0_GENERIC_COMMUNICATIONS_SUBSPACE_STRUCTURE;

//
// "PCCT" Platform Communications Channel Table
//

#define EFI_ACPI_3_0_PLATFORM_COMMUNICATIONS_CHANNEL_TABLE_SIGATURE  0x54434350
EFI_STATUS
EFIAPI
InstallAcpiPccTable (
  IN   EFI_ACPI_TABLE_PROTOCOL       *AcpiProtocol
  );
#endif

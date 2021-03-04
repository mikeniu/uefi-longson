/** @file
  This file contains Pcct Talbe initialized work.

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

#include "AcpiPlatform.h"

EFI_STATUS
EFIAPI
InstallAcpiPccTable (
  IN   EFI_ACPI_TABLE_PROTOCOL       *AcpiProtocol
  )
{
  EFI_STATUS                                                 Status;
  UINT32                                                     PccTableSize;
  UINTN                                                      TableHandle;
  EFI_ACPI_3_0_PLATFORM_COMMUNICATIONS_CHANNEL_TABLE_HEADER  *Pcct;
  EFI_ACPI_3_0_GENERIC_COMMUNICATIONS_SUBSPACE_STRUCTURE     *SubStruct;

  EFI_PHYSICAL_ADDRESS                                       PageAddress;

  TableHandle = 0;
  PccTableSize = sizeof(*Pcct) + sizeof(*SubStruct);
  Pcct = AllocateZeroPool (PccTableSize);
  if (Pcct == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Pcct->Header.Signature         = EFI_ACPI_3_0_PLATFORM_COMMUNICATIONS_CHANNEL_TABLE_SIGATURE;
  Pcct->Header.Length            = (UINT32)PccTableSize;
  Pcct->Header.Revision          = 2;
  Pcct->Header.Checksum          = 0x0;
  Pcct->Header.OemId[0]          = 'L';
  Pcct->Header.OemId[1]          = 'O';
  Pcct->Header.OemId[2]          = 'O';
  Pcct->Header.OemId[3]          = 'N';
  Pcct->Header.OemId[4]          = 'G';
  Pcct->Header.OemId[5]          = 'S';
  Pcct->Header.OemTableId        = EFI_ACPI_OEM_TABLE_ID;
  Pcct->Header.OemRevision       = EFI_ACPI_OEM_REVISION;
  Pcct->Header.CreatorId         = EFI_ACPI_CREATOR_ID;
  Pcct->Header.CreatorRevision   = EFI_ACPI_CREATOR_REVISION;
  Pcct->Flags	                 = 0;

  SubStruct = (EFI_ACPI_3_0_GENERIC_COMMUNICATIONS_SUBSPACE_STRUCTURE *)(Pcct + 1);

  SubStruct->Type = 0;
  SubStruct->Length = 62;

  PageAddress   = PHYS_TO_CACHED(0xFFFFFFFF);
  Status = gBS->AllocatePages (
                  AllocateAnyPages,
                  EfiACPIMemoryNVS,
                  0x200,
                  &PageAddress
                  );
  if (EFI_ERROR (Status)) {
    gBS->FreePool (Pcct);
    return EFI_OUT_OF_RESOURCES;
  }

  SubStruct->BaseAddress = VA_TO_PHYS(PageAddress);
  SubStruct->MemLength = 0x100;

  SubStruct->Doorbell_Register_Address.AddressSpaceId = 0x0;
  SubStruct->Doorbell_Register_Address.RegisterBitWidth = 0x20;
  SubStruct->Doorbell_Register_Address.RegisterBitOffset = 0x0;
  SubStruct->Doorbell_Register_Address.AccessSize = 0x0;
  SubStruct->Doorbell_Register_Address.Address = VA_TO_PHYS(PageAddress) + 0x100;
  SubStruct->Doorbell_Preserve = 0x0;
  SubStruct->Doorbell_Write = 0x0;
  SubStruct->Nominal_Latency = 0x100;
  SubStruct->Maximum_Periodic_Access_Rate = 0x0;
  SubStruct->Maximum_Request_Turnaround_Time = 0x100;

  AcpiPlatformChecksum ((UINT8*)Pcct, Pcct->Header.Length);
  Status = AcpiProtocol->InstallAcpiTable (AcpiProtocol, Pcct, PccTableSize, &TableHandle);

  FreePool (Pcct);

  return Status;
}

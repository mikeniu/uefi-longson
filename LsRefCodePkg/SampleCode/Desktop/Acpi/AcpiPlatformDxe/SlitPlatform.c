/** @file
  This file contains Slit Talbe initialized work.

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
InstallAcpiSlitTable (
  IN   EFI_ACPI_TABLE_PROTOCOL       *AcpiProtocol
  )
{
  EFI_STATUS                                                 Status;
  UINTN                                                      SlitTableSize;
  UINTN                                                      SlitMatrixSize;
  UINTN                                                      TableHandle;
  EFI_ACPI_3_0_SYSTEM_LOCALITY_DISTANCE_INFORMATION_TABLE_HEADER     *Slit;
  VOID                                                       *TempPtr;
  UINT8                                                      ArrayEntry[NODE_COUNT][NODE_COUNT];

  TableHandle = 0;
  SlitMatrixSize = sizeof(UINT8) * NODE_COUNT * NODE_COUNT;
  SlitTableSize = sizeof(*Slit) + SlitMatrixSize;
  Slit = AllocatePool (SlitTableSize);
  if (Slit == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Slit->Header.Signature         = EFI_ACPI_3_0_SYSTEM_LOCALITY_INFORMATION_TABLE_SIGNATURE;
  Slit->Header.Length            = (UINT32)SlitTableSize;
  Slit->Header.Revision          = EFI_ACPI_3_0_SYSTEM_LOCALITY_DISTANCE_INFORMATION_TABLE_REVISION;
  Slit->Header.Checksum          = 0xD0;
  Slit->Header.OemId[0]          = 'L';
  Slit->Header.OemId[1]          = 'O';
  Slit->Header.OemId[2]          = 'O';
  Slit->Header.OemId[3]          = 'N';
  Slit->Header.OemId[4]          = 'G';
  Slit->Header.OemId[5]          = 'S';
  Slit->Header.OemTableId        = EFI_ACPI_OEM_TABLE_ID;
  Slit->Header.OemRevision       = EFI_ACPI_OEM_REVISION;
  Slit->Header.CreatorId         = EFI_ACPI_CREATOR_ID;
  Slit->Header.CreatorRevision   = EFI_ACPI_CREATOR_REVISION;
  Slit->NumberOfSystemLocalities = NODE_COUNT;

  TempPtr = Slit + 1;

  if (Slit->NumberOfSystemLocalities == 1) {
    ArrayEntry[0][0] = 10;
  } else if (Slit->NumberOfSystemLocalities == 2) {
    ArrayEntry[0][0] = 10;
    ArrayEntry[0][1] = 15;
    ArrayEntry[1][0] = 15;
    ArrayEntry[1][1] = 10;
  } else if (Slit->NumberOfSystemLocalities == 4) {
    ArrayEntry[0][0] = 10;
    ArrayEntry[0][1] = 15;
    ArrayEntry[0][2] = 20;
    ArrayEntry[0][3] = 18;
    ArrayEntry[1][0] = 15;
    ArrayEntry[1][1] = 10;
    ArrayEntry[1][2] = 16;
    ArrayEntry[1][3] = 24;
    ArrayEntry[2][0] = 20;
    ArrayEntry[2][1] = 16;
    ArrayEntry[2][2] = 10;
    ArrayEntry[2][3] = 12;
    ArrayEntry[3][0] = 18;
    ArrayEntry[3][1] = 24;
    ArrayEntry[3][2] = 12;
    ArrayEntry[3][3] = 10;
  } else {
    //no support
  }

  CopyMem (TempPtr, ArrayEntry[0], SlitMatrixSize);

  AcpiPlatformChecksum ((UINT8*)Slit, Slit->Header.Length);
  Status = AcpiProtocol->InstallAcpiTable (AcpiProtocol, Slit, SlitTableSize, &TableHandle);

  FreePool (Slit);

  return Status;
}


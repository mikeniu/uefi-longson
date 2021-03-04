/** @file
  BIOS vendor information boot time changes.
  Misc. subclass type 2.
  SMBIOS type 0.

  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
  All intellectual property rights(Copyright, Patent and Trademark) reserved.

  Any violations of copyright or other intellectual property rights of the Loongson Technology
  Corporation Limited will be held accountable in accordance with the law,
  if you (or any of your subsidiaries, corporate affiliates or agents) initiate
  directly or indirectly any Intellectual Property Assertion or Intellectual Property Litigation:
  (i) against Loongson Technology Corporation Limited or any of its subsidiaries or corporate affiliates,
  (ii) against any party if such Intellectual Property Assertion or Intellectual Property Litigation arises
  in whole or in part from any software, technology, product or service of Loongson Technology Corporation
  Limited or any of its subsidiaries or corporate affiliates, or (iii) against any party relating to the Software.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR
  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION).

**/

#include "SmbiosTable.h"

//
// Static (possibly build generated) type 32.
//
MISC_SMBIOS_TABLE_DATA(EFI_MISC_BOOT_INFORMATION_STATUS_DATA, MiscBootInformationStatus) = {
  {
    EFI_SMBIOS_TYPE_SYSTEM_BOOT_INFORMATION,
    sizeof(SMBIOS_TABLE_TYPE32),
    0x2000
  },
  {0},  // BootInformationStatus
  0                                 // BootInformationData
};
/**
  This function makes boot time changes to the contents of the
  MiscBootInformation (Type 32).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/

MISC_SMBIOS_TABLE_FUNCTION(MiscBootInformationStatus)
{
  EFI_STATUS                         Status;
  EFI_SMBIOS_HANDLE                  SmbiosHandle;
  SMBIOS_TABLE_TYPE32                *SmbiosRecord;
  EFI_MISC_BOOT_INFORMATION_STATUS_DATA  *ForType32InputData;

  ForType32InputData = (EFI_MISC_BOOT_INFORMATION_STATUS_DATA *)RecordData;

  if (RecordData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SmbiosRecord = AllocatePool(sizeof (SMBIOS_TABLE_TYPE32) + 1 + 1);
  ZeroMem(SmbiosRecord, sizeof (SMBIOS_TABLE_TYPE32) + 1 + 1);
  CopyMem(SmbiosRecord,ForType32InputData,sizeof (SMBIOS_TABLE_TYPE32));

  DbgPrint(DEBUG_INFO,"%a() (type:%d) (size:0x%x)\n",__FUNCTION__,SmbiosRecord->Hdr.Type,sizeof (SMBIOS_TABLE_TYPE32) + 1 + 1);

  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);

  FreePool(SmbiosRecord);
  return Status;
}

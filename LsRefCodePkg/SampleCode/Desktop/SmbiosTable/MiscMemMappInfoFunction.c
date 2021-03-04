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
#include "Lib.h"

//
// Static (possibly build generated) Boot info (type 19).
//
MISC_SMBIOS_TABLE_DATA(EFI_MISC_MEMORY_ARRAY_MAPPED_DATA, MiscMemMappInfo) = {
  {
    EFI_SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS,
    sizeof(SMBIOS_TABLE_TYPE19),
    0x1300
  },
  0xffffffff,//StartingAddress
  0x0,//EndingAddress
  0,//MemoryArrayHandle
  0,//PartitionWidth
  0,//ExtendedStartingAd
  0//ExtendedEndingAddr

};

MISC_SMBIOS_TABLE_FUNCTION(MiscMemMappInfo)
{
  EFI_STATUS                         Status;
  UINT64                             TotalMemorySize;
  SMBIOS_TABLE_TYPE19                *SmbiosRecord;
  LS_MEM_INFO_HOB                    MemInfoHobData;
  EFI_SMBIOS_HANDLE                  MemArrayMappedAddrSmbiosHandle;
  EFI_MISC_MEMORY_ARRAY_MAPPED_DATA  *ForType19InputData;

  ForType19InputData = (EFI_MISC_MEMORY_ARRAY_MAPPED_DATA *)RecordData;

  GetMemInfoHob (&MemInfoHobData);
  TotalMemorySize = MemInfoHobData.MemInfo.SystemMemSize;
  ASSERT(TotalMemorySize != 0);
  //
  // Generate Memory Array Mapped Address info
  //
  SmbiosRecord = AllocatePool(sizeof (SMBIOS_TABLE_TYPE19) + 1);
  ZeroMem(SmbiosRecord, sizeof(SMBIOS_TABLE_TYPE19) + 1);
  CopyMem(SmbiosRecord,ForType19InputData,sizeof (SMBIOS_TABLE_TYPE19));

  SmbiosRecord->ExtendedEndingAddress = TotalMemorySize;
  Status = AddSmbiosRecord (Smbios, &MemArrayMappedAddrSmbiosHandle, (EFI_SMBIOS_TABLE_HEADER*) SmbiosRecord);

  DbgPrint(DEBUG_INFO,"%a() (type:%d) (size:0x%x)\n",__FUNCTION__,SmbiosRecord->Hdr.Type,sizeof(SMBIOS_TABLE_TYPE19) + 1);
  FreePool(SmbiosRecord);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

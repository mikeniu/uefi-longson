/** @file
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
#include <Ppi/Ls3aCpu/Ls3aPlatformTablePpi.h>

//
// Static (possibly build generated) Boot info (type 16).
//
MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE16, MiscPhysicalStoreInfo) = {
  {
    EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY,
    sizeof(SMBIOS_TABLE_TYPE16),
    0x1000
  }, 0
};

EFI_STATUS
Type16PhysicalStoreInitialize (
  SMBIOS_TABLE_TYPE16       *SmbiosRecord,
  LS_MEM_INFO_HOB            *MemInfoHobData
  )
{
  UINT8  MaxCapacityPerSlot = 0;

  SmbiosRecord->Location = 0x03;                                  /* Location, System board or motherboard */
  SmbiosRecord->Use = 0x3;                                        /* Use, System memory */
  if (MemInfoHobData->MemInfo.MemEccEnable) {
    SmbiosRecord->MemoryErrorCorrection = 0x5;                    /* MemoryErrorCorrection, Single-bit ECC */
  } else {
    SmbiosRecord->MemoryErrorCorrection = 0x3;                    /* MemoryErrorCorrection, None */
  }
  MaxCapacityPerSlot = 8;                                         /* 8G per dimm */
  SmbiosRecord->MaximumCapacity =
    (MaxCapacityPerSlot*1024*1024)*(MemInfoHobData->MemInfo.DimmNum);    /* MaximumCapacity, on the 3000 platform, each DIMM slot supports up to 8GB */
  SmbiosRecord->NumberOfMemoryDevices = MemInfoHobData->MemInfo.DimmNum; /* NumberOfMemoryDevice, */
  SmbiosRecord->MemoryErrorInformationHandle = 0xFFFE;            /* MemoryErrorInformationHandle, reserved */

  return EFI_SUCCESS;
}

/**
  This function makes boot time changes to the contents of the
  MiscBiosVendor (Type 16).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
MISC_SMBIOS_TABLE_FUNCTION (
  MiscPhysicalStoreInfo
  )
{
  CHAR8                        *OptionalStrStart;
  EFI_STATUS                   Status;
  SMBIOS_TABLE_TYPE16          *SmbiosRecord;
  EFI_SMBIOS_HANDLE            SmbiosHandle;
  LS_MEM_INFO_HOB              MemInfoHobData;

  //
  // First check for invalid parameters.
  //
  if (RecordData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SmbiosRecord = AllocateZeroPool(sizeof(SMBIOS_TABLE_TYPE16) + 1);
  CopyMem(SmbiosRecord, RecordData, sizeof(SMBIOS_TABLE_TYPE16));

  GetMemInfoHob (&MemInfoHobData);
  Status = Type16PhysicalStoreInitialize (SmbiosRecord, &MemInfoHobData);

  OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
  *OptionalStrStart = 0;

  DbgPrint(DEBUG_INFO,"%a() (type:%d) (size:0x%x)\n",__FUNCTION__,SmbiosRecord->Hdr.Type,sizeof(SMBIOS_TABLE_TYPE16) + 1);
  //
  // Now we have got the full smbios record, call smbios protocol to add this record.
  //
  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);

  FreePool(SmbiosRecord);
  return Status;
}

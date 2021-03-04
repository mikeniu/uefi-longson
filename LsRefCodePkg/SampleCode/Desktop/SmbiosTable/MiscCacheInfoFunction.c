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
#include <Io.h>
#include <Ppi/Ls3aCpu/Ls3aPlatformTablePpi.h>

//
// Static (possibly build generated) type 7.
//
MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE7, MiscCacheInfo) = {
  {
    EFI_SMBIOS_TYPE_CACHE_INFORMATION,
    sizeof(SMBIOS_TABLE_TYPE7),
    0x700
  }, 0
};

VOID
CreatCacheLeve1 (
  SMBIOS_TABLE_TYPE7   *SmbiosRecord,
  CHAR16*              ChipStr
  )
{
  if (!StrCmp(ChipStr,L"3A5000")) {
      SmbiosRecord->CacheConfiguration = 0x180;     /* Write Back, Enabled, Internal, Not socketed, L1 */
      SmbiosRecord->MaximumCacheSize = 0x8001;      /* Maximum Cache Size, 64KB */
      SmbiosRecord->InstalledSize = 0x8001;         /* Installed Size, 64KB */
      SmbiosRecord->SupportedSRAMType.Unknown = 1;  /* SRAM Type, Unknown */
      SmbiosRecord->CurrentSRAMType.Unknown = 1;    /* Current SRAM Type, Unknown */
      SmbiosRecord->ErrorCorrectionType = 0x5;      /* Error Correction Type, Single-bit ECC */
      SmbiosRecord->SystemCacheType = 0x4;          /* System Cache Type, Data */
      SmbiosRecord->Associativity = 0x5;            /* Associativity, L1 4-way-Set */
      // Add for smbios 3.1.0
      SmbiosRecord->MaximumCacheSize2 = 0x80000001; /* Maximum Cache Size, 64KB */
      SmbiosRecord->InstalledSize2 = 0x80000001;    /* Installed Size, 64KB */
  } else {
    DbgPrint(DEBUG_INFO,"Error:Did not match the chip!!!\n");
    ASSERT(FALSE);
  }
}

VOID
CreatCacheLeve2 (
  SMBIOS_TABLE_TYPE7   *SmbiosRecord,
  CHAR16*              ChipStr
  )
{
  if (!StrCmp(ChipStr,L"3A5000")) {
      SmbiosRecord->CacheConfiguration = 0x181;     /* Write Back, Enabled, Internal, Not socketed, L2 */
      SmbiosRecord->MaximumCacheSize = 0x8004;      /* Maximum Cache Size, 256KB */
      SmbiosRecord->InstalledSize = 0x8004;         /* Installed Size, 256KB */
      SmbiosRecord->SupportedSRAMType.Unknown = 1;  /* SRAM Type, Unknown */
      SmbiosRecord->CurrentSRAMType.Unknown = 1;    /* Current SRAM Type, Unknown */
      SmbiosRecord->ErrorCorrectionType = 0x5;      /* Error Correction Type, Single-bit ECC */
      SmbiosRecord->SystemCacheType = 0x4;          /* System Cache Type, Data */
      SmbiosRecord->Associativity = 0x8;            /* Associativity, L2 16-way-Set */
      // Add for smbios 3.1.0
      SmbiosRecord->MaximumCacheSize2 = 0x80000004; /* Maximum Cache Size, 256KB */
      SmbiosRecord->InstalledSize2 = 0x80000004;    /* Installed Size, 256KB */
  } else {
    DbgPrint(DEBUG_INFO,"Error:Did not match the chip!!!\n");
    ASSERT(FALSE);
  }
}

VOID
CreatCacheLeve3 (
  SMBIOS_TABLE_TYPE7   *SmbiosRecord,
  CHAR16*              ChipStr
  )
{
  if(!StrCmp(ChipStr,L"3A5000")){
      SmbiosRecord->CacheConfiguration = 0x182;     /* Write Back, Enabled, Internal, Not socketed, L3 */
      SmbiosRecord->MaximumCacheSize = 0x8080;      /* Maximum Cache Size 8MB */
      SmbiosRecord->InstalledSize = 0x8080;         /* Installed Size 8MB */
      SmbiosRecord->SupportedSRAMType.Unknown = 1;  /* SRAM Type, Unknown */
      SmbiosRecord->CurrentSRAMType.Unknown = 1;    /* Current SRAM Type, Unknown */
      SmbiosRecord->ErrorCorrectionType = 0x5;      /* Error Correction Type, Single-bit ECC */
      SmbiosRecord->SystemCacheType = 0x4;          /* System Cache Type, Data */
      SmbiosRecord->Associativity = 0x8;            /* Associativity, L3 16-way-Set */
      // Add for smbios 3.1.0
      SmbiosRecord->MaximumCacheSize2 = 0x80000080; /* Maximum Cache Size 8MB */
      SmbiosRecord->InstalledSize2 = 0x80000080;    /* Installed Size 8MB */
  } else {
    DbgPrint(DEBUG_INFO,"Error:Did not match the chip!!!\n");
    ASSERT(FALSE);
  }
}

EFI_STATUS
Type7CacheInfoInitialize (
  SMBIOS_TABLE_TYPE7      *SmbiosRecord,
  UINT8                   CacheLevel,
  LS_CPU_INFO_HOB         *CpuInfoHobData
  )
{
  CHAR16* UnicodeStr;
  UnicodeStr = AllocatePool(sizeof (CHAR16)*8);
  ZeroMem(UnicodeStr,sizeof(CHAR16)*8);
  AsciiStrToUnicodeStr((CHAR8 *)(&(Read64(LOONGSON_CPU_NAME_REG_ADDR))),UnicodeStr);

  switch (CacheLevel) {
    case 1:
      CreatCacheLeve1(SmbiosRecord,UnicodeStr);
      break;

    case 2:
      CreatCacheLeve2(SmbiosRecord,UnicodeStr);
      break;

    case 3:
      CreatCacheLeve3(SmbiosRecord,UnicodeStr);
      break;

    default:
      break;
  }
  FreePool(UnicodeStr);
  return EFI_SUCCESS;
}

MISC_SMBIOS_TABLE_FUNCTION(MiscCacheInfo)
{
  EFI_STATUS                      Status;
  SMBIOS_TABLE_TYPE7              *SmbiosRecord;
  LS_CPU_INFO_HOB                 CpuInfoHobData;
  UINT8                           CacheLevel;
  UINT8                           SocketNum, Loop;
  EFI_SMBIOS_HANDLE               SmbiosHandle;

  if (RecordData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

#if (TOT_NODE_NUM == 2)
  SocketNum = 2;
#else
  SocketNum = 1;
#endif

  Status = GetCpuInfoHob (&CpuInfoHobData);
  ASSERT_EFI_ERROR (Status);

  for (Loop = 0; Loop < SocketNum; Loop++) {
    for (CacheLevel = 1; CacheLevel < 4; CacheLevel++) {
      SmbiosRecord = AllocateZeroPool(sizeof(SMBIOS_TABLE_TYPE7));
      CopyMem(SmbiosRecord, RecordData,sizeof (SMBIOS_TABLE_TYPE7));

      Status = Type7CacheInfoInitialize (SmbiosRecord, CacheLevel, &CpuInfoHobData);
      ASSERT_EFI_ERROR (Status);

      Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER*) SmbiosRecord);
      ASSERT_EFI_ERROR (Status);

      FreePool(SmbiosRecord);
    }
  }
  return Status;
}

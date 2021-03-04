/** @file
  BIOS vendor information boot time changes.
  Misc. subclass type 2.
  SMBIOS type 0.

  Copyright (c) 2009, Intel Corporation
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

#include <Vers.h>
#include "SmbiosTable.h"
#include "Lib.h"

//
// Static (possibly build generated) Bios infomation data (type 0).
//
MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE0, MiscBiosInfo) = {
  {
    EFI_SMBIOS_TYPE_BIOS_INFORMATION,
    sizeof (SMBIOS_TABLE_TYPE0),
    0x0
  }, 0
};

VOID
GetBiosReleaseTime (
  CHAR16    *Source,
  CHAR16    *ReleaseTime
  )
{
  CHAR16        *Month;
  CHAR16        *Day;
  CHAR16        *Year;
  CHAR16        BakChr;


  /* Get Month */
  Month = StrStr(Source, L" ");
  Month++;

  BakChr = *(Month + 3);
  *(Month + 3) = '\0';

  if (!StrCmp(Month, L"JAN")) {
    StrCpy(ReleaseTime, L"01/");
  } else if (!StrCmp(Month, L"Feb")) {
    StrCpy(ReleaseTime, L"02/");
  } else if (!StrCmp(Month, L"Mar")) {
    StrCpy(ReleaseTime, L"03/");
  } else if (!StrCmp(Month, L"Apr")) {
    StrCpy(ReleaseTime, L"04/");
  } else if (!StrCmp(Month, L"May")) {
    StrCpy(ReleaseTime, L"05/");
  } else if (!StrCmp(Month, L"Jun")) {
    StrCpy(ReleaseTime, L"06/");
  } else if (!StrCmp(Month, L"Jul")) {
    StrCpy(ReleaseTime, L"07/");
  } else if (!StrCmp(Month, L"Aug")) {
    StrCpy(ReleaseTime, L"08/");
  } else if (!StrCmp(Month, L"Sep")) {
    StrCpy(ReleaseTime, L"09/");
  } else if (!StrCmp(Month, L"Oct")) {
    StrCpy(ReleaseTime, L"10/");
  } else if (!StrCmp(Month, L"Nov")) {
    StrCpy(ReleaseTime, L"11/");
  } else if (!StrCmp(Month, L"Dec")) {
    StrCpy(ReleaseTime, L"12/");
  } else {
    StrCpy(ReleaseTime, L"Unknown");
    return;
  }
  *(Month + 3) = BakChr;

  /* Get Day */
  Day = Month;
  Day = StrStr(Day, L" ");
  Day++;

  BakChr = *(Day + 2);
  *(Day + 2) = '\0';

  if (*Day == ' ') {
    *Day = '0';
  }

  StrCat(ReleaseTime, Day);
  StrCat(ReleaseTime, L"/");

  *(Day + 2) = BakChr;

  /* Get Year */
  Year = Day + (StrLen(Day) - 4); /* The last 4 bytes is year */

  StrCat(ReleaseTime, Year);
}

VOID
UpdateBiosDateVersion (
  VOID
  )
{
  CHAR16                *Version;
  CHAR16                *VersionStr;
  CHAR16                *BiosReleaseTime;
  STRING_REF            TokenToUpdate;

  if (VERS) {
    Version = StrStr ((CHAR16 *)VERS, L"V");
  } else {
    Version = AllocatePool (sizeof(CHAR16) * 10);
    StrCpy (Version, L"V0.3.0");
  }
  TokenToUpdate = STRING_TOKEN (STR_MISC_BIOS_VERSION);
  VersionStr = HiiGetPackageString(&gEfiCallerIdGuid, TokenToUpdate, NULL);
  VersionStr = StrCat (VersionStr, Version);
  HiiSetString (mHiiHandle, TokenToUpdate, VersionStr, NULL);

  if (BLD) { /* BLD time format is EN UTF-8 */
    BiosReleaseTime = AllocateZeroPool(30);
    GetBiosReleaseTime(BLD, BiosReleaseTime);
    TokenToUpdate = STRING_TOKEN (STR_MISC_BIOS_RELEASE_DATE);
    HiiSetString (mHiiHandle, TokenToUpdate, BiosReleaseTime, NULL);
    FreePool(BiosReleaseTime);
  }
  if (!VERS) {
    FreePool (Version);
  }
}

EFI_STATUS
Type0BiosTableInitialize (
  SMBIOS_TABLE_TYPE0  *SmbiosRecord
  )
{
  STRING_REF            TokenToGet;
  CHAR16                *VersionStr;

  /* Get BIOS version */
  TokenToGet = STRING_TOKEN (STR_MISC_BIOS_VERSION);
  VersionStr = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  VersionStr = StrStr(VersionStr, L"V");
  VersionStr++;

  if(!SmbiosRecord)
    return EFI_INVALID_PARAMETER;

  SmbiosRecord->Vendor = 1;                                                      /* SMBIOS_TABLE_STRING   Vendor */
  SmbiosRecord->BiosVersion = 2;                                                 /* SMBIOS_TABLE_STRING   BiosVersion */
  SmbiosRecord->BiosReleaseDate = 3;                                             /* SMBIOS_TABLE_STRING   BiosReleaseDate */
  SmbiosRecord->BiosSize = (0x400000 / 0x10000) - 1;                             /* BiosSize, 64K * (n + 1). The size of ROM who CRB is 4MB */

  /* BIOS character infomation */
  SmbiosRecord->BiosCharacteristics.PciIsSupported = 1;
  SmbiosRecord->BiosCharacteristics.BiosIsUpgradable = 1;
  SmbiosRecord->BiosCharacteristics.BootFromCdIsSupported = 1;
  SmbiosRecord->BiosCharacteristics.SelectableBootIsSupported = 1;
  SmbiosRecord->BiosCharacteristics.RomBiosIsSocketed = 1;                      /* BIOS is socketed on CRB */
  SmbiosRecord->BiosCharacteristics.SerialIsSupported = 1;

  SmbiosRecord->BIOSCharacteristicsExtensionBytes[0] = (0x1 << 1);              /* USB Legacy Supported */
#if (TOT_NODE_NUM != 2)
  SmbiosRecord->BIOSCharacteristicsExtensionBytes[1] = (0x1 << 3);              /* UEFI SPEC support */
#else
  SmbiosRecord->BIOSCharacteristicsExtensionBytes[1] = (0x1 << 3) | (0x1 << 1); /* Network Boot support, UEFI SPEC support */
#endif
  SmbiosRecord->SystemBiosMajorRelease = VersionStr[0] - '0';                   /* System BIOS Major Release */
  SmbiosRecord->SystemBiosMinorRelease = VersionStr[2] - '0';                   /* System BIOS Minor Release */
  SmbiosRecord->EmbeddedControllerFirmwareMajorRelease = 0xFF;
  SmbiosRecord->EmbeddedControllerFirmwareMinorRelease = 0xFF;

  return EFI_SUCCESS;
}

/**
  This function makes boot time changes to the contents of the
  MiscBiosVendor (Type 0).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
MISC_SMBIOS_TABLE_FUNCTION (
  MiscBiosInfo
  )
{
  CHAR8                 *OptionalStrStart;
  UINTN                 VendorStrLen;
  UINTN                 VerStrLen;
  UINTN                 DateStrLen;
  UINTN                 TotalSize;
  CHAR16                *Version;
  CHAR16                *ReleaseDate;
  EFI_STATUS            Status;
  EFI_STRING            Vendor;
  STRING_REF            TokenToGet;
  SMBIOS_TABLE_TYPE0    *SmbiosRecord;
  EFI_SMBIOS_HANDLE     SmbiosHandle;

  //
  // First check for invalid parameters.
  //
  if (RecordData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  UpdateBiosDateVersion();

  TokenToGet = STRING_TOKEN (STR_MISC_BIOS_VENDOR);
  Vendor = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  VendorStrLen = StrLen(Vendor);
  if (VendorStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_BIOS_VERSION);
  Version = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  VerStrLen = StrLen(Version);
  if (VerStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_BIOS_RELEASE_DATE);
  ReleaseDate = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  DateStrLen = StrLen(ReleaseDate);
  if (DateStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TotalSize = sizeof(SMBIOS_TABLE_TYPE0) + (VendorStrLen + 1) + (VerStrLen + 1) + (DateStrLen + 1) + 1;
  SmbiosRecord = AllocateZeroPool(TotalSize);
  CopyMem(SmbiosRecord, RecordData, sizeof(SMBIOS_TABLE_TYPE0));

  Status = Type0BiosTableInitialize (SmbiosRecord);
  OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
  UnicodeStrToAsciiStr(Vendor, OptionalStrStart);
  UnicodeStrToAsciiStr(Version, OptionalStrStart + VendorStrLen + 1);
  UnicodeStrToAsciiStr(ReleaseDate, OptionalStrStart + VendorStrLen + 1 + VerStrLen + 1);

  DbgPrint(DEBUG_INFO,"%a() (type:%d) (size:0x%x)\n",__FUNCTION__,SmbiosRecord->Hdr.Type,
  sizeof (SMBIOS_TABLE_TYPE0) + VendorStrLen + 1 + VerStrLen + 1 + DateStrLen + 1 + 1 );

  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);

  FreePool(SmbiosRecord);
  return Status;
}

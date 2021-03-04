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
// Static (possibly build generated) Chassis Manufacturer data(type 3).
//
MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE3, MiscChassisInfo) = {
  {
    EFI_SMBIOS_TYPE_SYSTEM_ENCLOSURE,
    sizeof(SMBIOS_TABLE_TYPE3),
    0x300
  }, 0
};

EFI_STATUS
Type3ChassisInfoInitialize (
  SMBIOS_TABLE_TYPE3   *SmbiosRecord
  )
{
  if(!SmbiosRecord)
    return EFI_INVALID_PARAMETER;

#if (TOT_NODE_NUM >= 2)
  SmbiosRecord->Type = 0x17;             /* Server default is Rack Mount Chassis */
  SmbiosRecord->Height = 0x2;            /* The server CRB chassis height is usually 2U */
#else
  SmbiosRecord->Type = 0x3;              /* Desktop */
  SmbiosRecord->Height = 0;              /* The Desktop CRB chassis height is unknown */
#endif
  SmbiosRecord->Manufacturer = 1;        /* SMBIOS_TABLE_STRING       Manufacturer */
  SmbiosRecord->Version = 2;             /* SMBIOS_TABLE_STRING       Version */
  SmbiosRecord->SerialNumber = 3;        /* SMBIOS_TABLE_STRING       SerialNumber */
  SmbiosRecord->AssetTag = 4;            /* SMBIOS_TABLE_STRING       AssetTag */
  SmbiosRecord->BootupState = 0x3;       /* Boot-up State        Save */
  SmbiosRecord->PowerSupplyState = 0x3;  /* Power Supply State   Save */
  SmbiosRecord->ThermalState = 0x3;      /* Thermal State        Save */
  SmbiosRecord->SecurityStatus = 0x2;    /* Security State       Unknown */

  return EFI_SUCCESS;
}

/**
  This function makes boot time changes to the contents of the
  MiscChassisManufacturer (Type 3).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
MISC_SMBIOS_TABLE_FUNCTION (
  MiscChassisInfo
  )
{
  CHAR8                           *OptionalStrStart;
  UINTN                           ManuStrLen;
  UINTN                           VerStrLen;
  UINTN                           AssetTagStrLen;
  UINTN                           SerialNumStrLen;
  UINTN                           SkuNumberStrLen;
  UINTN                           TotalSize;
  EFI_STATUS                      Status;
  EFI_STRING                      Manufacturer;
  EFI_STRING                      Version;
  EFI_STRING                      SerialNumber;
  EFI_STRING                      AssetTag;
  EFI_STRING                      SkuNumber;
  STRING_REF                      TokenToGet;
  EFI_SMBIOS_HANDLE               SmbiosHandle;
  SMBIOS_TABLE_TYPE3              *SmbiosRecord;

  //
  // First check for invalid parameters.
  //
  if (RecordData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_CHASSIS_MANUFACTURER);
  Manufacturer = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  ManuStrLen = StrLen(Manufacturer);
  if (ManuStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_CHASSIS_VERSION);
  Version = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  VerStrLen = StrLen(Version);
  if (VerStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_CHASSIS_SERIAL_NUMBER);
  SerialNumber = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  SerialNumStrLen = StrLen(SerialNumber);
  if (SerialNumStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_CHASSIS_ASSET_TAG);
  AssetTag = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  AssetTagStrLen = StrLen(AssetTag);
  if (AssetTagStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_CHASSIS_SKU_NUMBER);
  SkuNumber = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  SkuNumberStrLen = StrLen(SkuNumber);
  if (SkuNumberStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TotalSize = sizeof (SMBIOS_TABLE_TYPE3) + ManuStrLen + 1 + VerStrLen + 1 + SerialNumStrLen + 1 + AssetTagStrLen + 1 + SkuNumberStrLen + 1 + 1;

  SmbiosRecord = AllocateZeroPool(TotalSize);
  CopyMem(SmbiosRecord, RecordData, sizeof (SMBIOS_TABLE_TYPE3));

  Status = Type3ChassisInfoInitialize (SmbiosRecord);

  OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
  UnicodeStrToAsciiStr (Manufacturer, OptionalStrStart);
  UnicodeStrToAsciiStr (Version, OptionalStrStart + ManuStrLen + 1);
  UnicodeStrToAsciiStr (SerialNumber, OptionalStrStart + ManuStrLen + 1 + VerStrLen + 1);
  UnicodeStrToAsciiStr (AssetTag, OptionalStrStart + ManuStrLen + 1 + VerStrLen + 1 + SerialNumStrLen + 1);
  UnicodeStrToAsciiStr (SkuNumber, OptionalStrStart + ManuStrLen + 1 + VerStrLen + 1 + SerialNumStrLen + 1 + AssetTagStrLen + 1);

  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);

  FreePool(SmbiosRecord);
  return Status;
}

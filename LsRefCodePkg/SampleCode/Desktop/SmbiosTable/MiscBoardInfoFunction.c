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

#include "SmbiosTable.h"
#include "Lib.h"

//
// Static (possibly build generated) Base board data(type 2).
//
MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE2, MiscBoardInfo) = {
  {
    EFI_SMBIOS_TYPE_BASEBOARD_INFORMATION,
    sizeof(SMBIOS_TABLE_TYPE2),
    0x200
  }, 0
};

EFI_STATUS
Type2BoardInfoInitialize (
  SMBIOS_TABLE_TYPE2     *SmbiosRecord
  )
{
  if(!SmbiosRecord)
    return EFI_INVALID_PARAMETER;

  SmbiosRecord->Manufacturer = 1;               /* SMBIOS_TABLE_STRING       Manufacturer */
  SmbiosRecord->ProductName = 2;                /* SMBIOS_TABLE_STRING       ProductName */
  SmbiosRecord->Version = 3;                    /* SMBIOS_TABLE_STRING       Version */
  SmbiosRecord->SerialNumber = 4;               /* SMBIOS_TABLE_STRING       SerialNumber */
  SmbiosRecord->AssetTag = 5;                   /* SMBIOS_TABLE_STRING       AssetTag */
  SmbiosRecord->FeatureFlag.Motherboard = 1;
  SmbiosRecord->FeatureFlag.Replaceable = 1;
  SmbiosRecord->BoardType = BaseBoardTypeMotherBoard;

  return EFI_SUCCESS;
}

/**
  This function makes boot time changes to the contents of the
  MiscBaseBoardManufacturer (Type 2).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
MISC_SMBIOS_TABLE_FUNCTION (
  MiscBoardInfo
  )
{
  CHAR8                           *OptionalStrStart;
  UINTN                           ManuStrLen;
  UINTN                           ProductStrLen;
  UINTN                           VerStrLen;
  UINTN                           AssetTagStrLen;
  UINTN                           SerialNumStrLen;
  UINTN                           ChassisStrLen;
  UINTN                           TotalSize;
  EFI_STATUS                      Status;
  EFI_STRING                      Manufacturer;
  EFI_STRING                      Product;
  EFI_STRING                      Version;
  EFI_STRING                      SerialNumber;
  EFI_STRING                      AssetTag;
  EFI_STRING                      Chassis;
  STRING_REF                      TokenToGet;
  EFI_SMBIOS_HANDLE               SmbiosHandle;
  SMBIOS_TABLE_TYPE2              *SmbiosRecord;


  //
  // First check for invalid parameters.
  //
  if (RecordData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_BASE_BOARD_MANUFACTURER);
  Manufacturer = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  ManuStrLen = StrLen(Manufacturer);
  if (ManuStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }
#ifdef LS3A5000
#if (TOT_NODE_NUM == 1)
  TokenToGet = STRING_TOKEN (STR_MISC_BASE_BOARD_PRODUCT_NAME_5000_1W);
#elif (TOT_NODE_NUM == 4)
  TokenToGet = STRING_TOKEN (STR_MISC_BASE_BOARD_PRODUCT_NAME_5000_4W);
#endif
#else
#if (TOT_NODE_NUM == 2)
  TokenToGet = STRING_TOKEN (STR_MISC_BASE_BOARD_PRODUCT_NAME_DUAL);
#else
  TokenToGet = STRING_TOKEN (STR_MISC_BASE_BOARD_PRODUCT_NAME);
#endif
#endif
  Product = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  ProductStrLen = StrLen(Product);
  if (ProductStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_BASE_BOARD_VERSION);
  Version = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  VerStrLen = StrLen(Version);
  if (VerStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_BASE_BOARD_SERIAL_NUMBER);
  SerialNumber = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  SerialNumStrLen = StrLen(SerialNumber);
  if (SerialNumStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_BASE_BOARD_ASSET_TAG);
  AssetTag = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  AssetTagStrLen = StrLen(AssetTag);
  if (AssetTagStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_BASE_BOARD_CHASSIS_LOCATION);
  Chassis = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  ChassisStrLen = StrLen(Chassis);
  if (ChassisStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TotalSize = sizeof (SMBIOS_TABLE_TYPE2) + (ManuStrLen + 1) + (ProductStrLen + 1) + (VerStrLen + 1) + (SerialNumStrLen + 1) + (AssetTagStrLen + 1) + (ChassisStrLen + 1) + 1;
  SmbiosRecord = AllocateZeroPool(TotalSize);
  CopyMem(SmbiosRecord, RecordData, sizeof (SMBIOS_TABLE_TYPE2));

  Status = Type2BoardInfoInitialize(SmbiosRecord);
  OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
  UnicodeStrToAsciiStr(Manufacturer, OptionalStrStart);
  UnicodeStrToAsciiStr(Product, OptionalStrStart + ManuStrLen + 1);
  UnicodeStrToAsciiStr(Version, OptionalStrStart + ManuStrLen + 1 + ProductStrLen + 1);
  UnicodeStrToAsciiStr(SerialNumber, OptionalStrStart + ManuStrLen + 1 + ProductStrLen + 1 + VerStrLen + 1);
  UnicodeStrToAsciiStr(AssetTag, OptionalStrStart + ManuStrLen + 1 + ProductStrLen + 1 + VerStrLen + 1 + SerialNumStrLen + 1);
  UnicodeStrToAsciiStr(Chassis, OptionalStrStart + ManuStrLen + 1 + ProductStrLen + 1 + VerStrLen + 1 + SerialNumStrLen + 1 + AssetTagStrLen + 1);

  DbgPrint(DEBUG_INFO,"%a() (type:%d) (size:0x%x)\n",__FUNCTION__,SmbiosRecord->Hdr.Type,
  sizeof (SMBIOS_TABLE_TYPE2) + ManuStrLen + 1 + ProductStrLen + 1 + VerStrLen + 1 + SerialNumStrLen + 1 + AssetTagStrLen + 1 + ChassisStrLen + 1 + 1);

  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);

  FreePool(SmbiosRecord);
  return Status;
}

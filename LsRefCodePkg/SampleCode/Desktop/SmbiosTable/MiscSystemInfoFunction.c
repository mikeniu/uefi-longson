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
// Static (possibly build generated) System Manufacturer data(type 1).
//
MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE1, MiscSystemInfo) = {
  {
    EFI_SMBIOS_TYPE_SYSTEM_INFORMATION,
    sizeof (SMBIOS_TABLE_TYPE1),
    0x100
  }, 0
};

VOID
GetUuidValue (
  UINT32  *UuidHH,
  UINT32  *UuidHL,
  UINT32  *UuidLH,
  UINT32  *UuidLL
  )
{
  /* Demo code, you can get the UUID from the variable, it can be modified by user */
  *UuidHH = 0xFFFFFFFF;
  *UuidHL = 0xFFFFFFFF;
  *UuidLH = 0xFFFFFFFF;
  *UuidLL = 0xFFFFFFFF;
}

VOID
GetUuid (
  GUID  *Uuid
  )
{
  UINT32  i;
  UINT32  UuidHH;
  UINT32  UuidHL;
  UINT32  UuidLH;
  UINT32  UuidLL;

  GetUuidValue (&UuidHH, &UuidHL, &UuidLH, &UuidLL);

  Uuid->Data1 = UuidHL;
  Uuid->Data2 = (UINT16) UuidHH;
  Uuid->Data3 = (UINT16) (UuidHH >> 16);

  for (i = 0; i < 4; i++) {
    Uuid->Data4[i] = (UINT8)UuidLL;
    UuidLL >>= 8;
  }

  for (i = 0; i < 4; i++) {
    Uuid->Data4[i + 4] = (UINT8)UuidLH;
    UuidLH >>= 8;
  }
}

EFI_STATUS
Type1SystemTableInitialize (
  SMBIOS_TABLE_TYPE1  *SmbiosRecord
  )
{

  GetUuid (&(SmbiosRecord->Uuid));
  SmbiosRecord->Manufacturer = 1;               /* SMBIOS_TABLE_STRING   Manufacturer */
  SmbiosRecord->ProductName = 2;                /* SMBIOS_TABLE_STRING   ProductName */
  SmbiosRecord->Version = 3;                    /* SMBIOS_TABLE_STRING   Version */
  SmbiosRecord->SerialNumber= 4;                /* SMBIOS_TABLE_STRING   SerialNumber */
  SmbiosRecord->WakeUpType = 0x6;               /* Power Switch */
  SmbiosRecord->SKUNumber = 5;                  /* SMBIOS_TABLE_STRING   SKUNumber*/
  SmbiosRecord->Family = 6;                     /* SMBIOS_TABLE_STRING   Family*/

  return EFI_SUCCESS;
}

/**
  This function makes boot time changes to the contents of the
  MiscSystemManufacturer (Type 1).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
MISC_SMBIOS_TABLE_FUNCTION (
  MiscSystemInfo
  )
{
  STRING_REF                        TokenToGet;
  EFI_SMBIOS_HANDLE                 SmbiosHandle;
  SMBIOS_TABLE_TYPE1                *SmbiosRecord;
  EFI_STATUS                        Status;
  EFI_STRING                        Manufacturer;
  EFI_STRING                        ProductName;
  EFI_STRING                        Version;
  EFI_STRING                        SerialNumber;
  EFI_STRING                        SkuNum;
  EFI_STRING                        Family;
  CHAR8                             *OptionalStrStart;
  UINTN                             ManuStrLen;
  UINTN                             VerStrLen;
  UINTN                             PdNameStrLen;
  UINTN                             SerialNumStrLen;
  UINTN                             FamilyStrLen;
  UINTN                             SkuNumStrLen;
  UINTN                             TotalSize;

  //
  // First check for invalid parameters.
  //
  if (RecordData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_SYSTEM_MANUFACTURER);
  Manufacturer = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  ManuStrLen = StrLen(Manufacturer);
  if (ManuStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }
#ifdef LS3A5000
#if (TOT_NODE_NUM == 2)
  TokenToGet = STRING_TOKEN (STR_MISC_SYSTEM_PRODUCT_NAME_DUAL_5000);
#else
  TokenToGet = STRING_TOKEN (STR_MISC_SYSTEM_PRODUCT_NAME_5000);
#endif
#else
#if (TOT_NODE_NUM == 2)
  TokenToGet = STRING_TOKEN (STR_MISC_SYSTEM_PRODUCT_NAME_DUAL);
#else
  TokenToGet = STRING_TOKEN (STR_MISC_SYSTEM_PRODUCT_NAME);
#endif
#endif
  ProductName = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  PdNameStrLen = StrLen(ProductName);
  if (PdNameStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_SYSTEM_VERSION);
  Version = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  VerStrLen = StrLen(Version);
  if (VerStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_SYSTEM_SERIAL_NUMBER);
  SerialNumber = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  SerialNumStrLen = StrLen(SerialNumber);
  if (SerialNumStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_SYSTEM_SKUNUM);
  SkuNum = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  SkuNumStrLen = StrLen(SkuNum);
  if (SkuNumStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_SYSTEM_FAMILY);
  Family = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  FamilyStrLen = StrLen(Family);
  if (FamilyStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  //
  // Two zeros following the last string.
  //

  TotalSize = sizeof (SMBIOS_TABLE_TYPE1) + (ManuStrLen + 1) + (PdNameStrLen + 1) + (VerStrLen + 1) + (SerialNumStrLen + 1) + (SkuNumStrLen + 1) + (FamilyStrLen + 1) + 1;
  SmbiosRecord = AllocateZeroPool(TotalSize);
  CopyMem(SmbiosRecord , RecordData, sizeof(SMBIOS_TABLE_TYPE1));

  Status = Type1SystemTableInitialize (SmbiosRecord);

  OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
  UnicodeStrToAsciiStr(Manufacturer, OptionalStrStart);
  UnicodeStrToAsciiStr(ProductName, OptionalStrStart + ManuStrLen + 1);
  UnicodeStrToAsciiStr(Version, OptionalStrStart + ManuStrLen + 1 + PdNameStrLen + 1);
  UnicodeStrToAsciiStr(SerialNumber, OptionalStrStart + ManuStrLen + 1 + PdNameStrLen + 1 + VerStrLen + 1);
  UnicodeStrToAsciiStr(SkuNum, OptionalStrStart + ManuStrLen + 1 + PdNameStrLen + 1 + VerStrLen + 1 +SerialNumStrLen + 1);
  UnicodeStrToAsciiStr(Family, OptionalStrStart + ManuStrLen + 1 + PdNameStrLen + 1 + VerStrLen + 1 +SerialNumStrLen + 1 + SkuNumStrLen + 1 );

  DbgPrint(DEBUG_INFO,"%a() (type:%d) (size:0x%x)\n",__FUNCTION__,SmbiosRecord->Hdr.Type,
  sizeof (SMBIOS_TABLE_TYPE1) + ManuStrLen + 1 + PdNameStrLen + 1 + VerStrLen + 1 + SerialNumStrLen + 1 + SkuNumStrLen+1 + FamilyStrLen+1 + 1);

  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);

  FreePool(SmbiosRecord);
  return Status;
}

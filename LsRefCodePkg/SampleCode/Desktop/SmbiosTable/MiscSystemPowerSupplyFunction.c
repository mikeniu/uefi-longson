/** @file
  SMBIOS MiscPowerSupply(type 39).

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
// Static PowerSupply data(type 39).
//
MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE39, MiscPowerSupplyInfo) = {
  {
    EFI_SMBIOS_TYPE_SYSTEM_POWER_SUPPLY,
    sizeof(SMBIOS_TABLE_TYPE39),
    0x2700
  }, 0
};

EFI_STATUS
Type39PowerSupplyInfoInitialize (
  SMBIOS_TABLE_TYPE39   *SmbiosRecord
  )
{
  if(!SmbiosRecord)
    return EFI_INVALID_PARAMETER;

  SmbiosRecord->PowerUnitGroup             = 1;
  SmbiosRecord->Location                   = 1;
  SmbiosRecord->DeviceName                 = 2;
  SmbiosRecord->Manufacturer               = 3;
  SmbiosRecord->SerialNumber               = 4;
  SmbiosRecord->AssetTagNumber             = 5;
  SmbiosRecord->ModelPartNumber            = 6;
  SmbiosRecord->RevisionLevel              = 7;
  SmbiosRecord->MaxPowerCapacity           = 0x8000;

  SmbiosRecord->PowerSupplyCharacteristics.PowerSupplyHotReplaceable = 1;
  SmbiosRecord->PowerSupplyCharacteristics.PowerSupplyPresent        = 1;
  SmbiosRecord->PowerSupplyCharacteristics.PowerSupplyUnplugged      = 1;
  SmbiosRecord->PowerSupplyCharacteristics.InputVoltageRangeSwitch   = 2;
  SmbiosRecord->PowerSupplyCharacteristics.PowerSupplyStatus         = 2;
  SmbiosRecord->PowerSupplyCharacteristics.PowerSupplyType           = 2;
  SmbiosRecord->PowerSupplyCharacteristics.Reserved                  = 0;

  SmbiosRecord->InputVoltageProbeHandle    = 0xffff;
  SmbiosRecord->CoolingDeviceHandle        = 0xffff;
  SmbiosRecord->InputCurrentProbeHandle    = 0xffff;

  return EFI_SUCCESS;
}

MISC_SMBIOS_TABLE_FUNCTION (
  MiscPowerSupplyInfo
  )
{
  CHAR8                           *OptionalStrStart;
  UINTN                           LocationStrLen;
  UINTN                           NameStrLen;
  UINTN                           ManuStrLen;
  UINTN                           SerialStrLen;
  UINTN                           AssetStrLen;
  UINTN                           ModelStrLen;
  UINTN                           RevStrLen;
  UINTN                           TotalSize;
  EFI_STATUS                      Status;
  EFI_STRING                      Location;
  EFI_STRING                      DeviceName;
  EFI_STRING                      Manufacturer;
  EFI_STRING                      SerialNum;
  EFI_STRING                      AssetTag;
  EFI_STRING                      ModelPart;
  EFI_STRING                      Revision;
  STRING_REF                      TokenToGet;
  EFI_SMBIOS_HANDLE               SmbiosHandle;
  SMBIOS_TABLE_TYPE39             *SmbiosRecord;

  //
  // First check for invalid parameters.
  //
  if (RecordData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

/*String 1*/
  TokenToGet     = STRING_TOKEN (STR_MISC_SYSTEM_POWER_SUPPLY_LOCATION);
  Location       = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  LocationStrLen = StrLen(Location);
  if (LocationStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }
/*String 2*/
  TokenToGet = STRING_TOKEN (STR_MISC_SYSTEM_POWER_SUPPLY_DEVICE_NAME);
  DeviceName = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  NameStrLen = StrLen(DeviceName);
  if (NameStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

/*String 3*/
  TokenToGet   = STRING_TOKEN (STR_MISC_SYSTEM_POWER_SUPPLY_MANUFACTURER);
  Manufacturer = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  ManuStrLen   = StrLen(Manufacturer);
  if (ManuStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

/*String 4*/
  TokenToGet   = STRING_TOKEN (STR_MISC_SYSTEM_POWER_SUPPLY_SERIAL_NUMBER);
  SerialNum    = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  SerialStrLen = StrLen(SerialNum);
  if (SerialStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

/*String 5*/
  TokenToGet  = STRING_TOKEN (STR_MISC_SYSTEM_POWER_SUPPLY_ASSET_TAG);
  AssetTag    = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  AssetStrLen = StrLen(AssetTag);
  if (AssetStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

/*String 6*/
  TokenToGet  = STRING_TOKEN (STR_MISC_SYSTEM_POWER_SUPPLY_MODEL_PART);
  ModelPart   = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  ModelStrLen = StrLen(ModelPart);
  if (ModelStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

/*String 7*/
  TokenToGet  = STRING_TOKEN (STR_MISC_SYSTEM_POWER_SUPPLY_REVISION);
  Revision    = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  RevStrLen = StrLen(Revision);
  if (RevStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TotalSize = sizeof (SMBIOS_TABLE_TYPE39) + LocationStrLen + 1 + NameStrLen + 1 +\
              ManuStrLen + 1 + SerialStrLen + 1 + AssetStrLen + 1 + ModelStrLen + 1 + RevStrLen + 1 + 1;

  SmbiosRecord = AllocateZeroPool(TotalSize);
  CopyMem(SmbiosRecord, RecordData, sizeof (SMBIOS_TABLE_TYPE39));

  Status = Type39PowerSupplyInfoInitialize (SmbiosRecord);

  OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
  UnicodeStrToAsciiStr (Location, OptionalStrStart);
  UnicodeStrToAsciiStr (DeviceName, OptionalStrStart + LocationStrLen + 1);
  UnicodeStrToAsciiStr (Manufacturer, OptionalStrStart + LocationStrLen + 1 + NameStrLen + 1);
  UnicodeStrToAsciiStr (SerialNum, OptionalStrStart + LocationStrLen + 1 + NameStrLen + 1 + ManuStrLen + 1);
  UnicodeStrToAsciiStr (AssetTag, OptionalStrStart + LocationStrLen + 1 + NameStrLen + 1 + ManuStrLen + 1 +\
      SerialStrLen + 1);
  UnicodeStrToAsciiStr (ModelPart, OptionalStrStart + LocationStrLen + 1 + NameStrLen + 1 + ManuStrLen + 1 +\
      SerialStrLen + 1 + AssetStrLen + 1);
  UnicodeStrToAsciiStr (Revision, OptionalStrStart + LocationStrLen + 1 + NameStrLen + 1 + ManuStrLen + 1 +\
      SerialStrLen + 1 + AssetStrLen + 1 + ModelStrLen + 1);

  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);

  FreePool(SmbiosRecord);
  return Status;
}

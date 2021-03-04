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
// Static (possibly build generated) Boot info (type 28).
//
MISC_SMBIOS_TABLE_DATA(EFI_MISC_TEMPERATURE_DATA, MiscTemperatureInfo) = {
  {
    EFI_SMBIOS_TYPE_TEMPERATURE_PROBE ,
    sizeof(SMBIOS_TABLE_TYPE28),
    0x1c00
  },
  1,//Description
  0x63,//LocationAndStatus
  1280,//MaximumValue
  0x8000,//MinimumValue
  0x8000,//Resolution
  0x8000,//Tolerance
  0x8000,//Accuracy
  0x0,//OEMDefined
  0x0//NominalValue

};

/**
  This function makes boot time changes to the contents of the
  MiscBiosVendor (Type 28).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
MISC_SMBIOS_TABLE_FUNCTION(MiscTemperatureInfo)
{
  SMBIOS_TABLE_TYPE28         *SmbiosRecord;
  EFI_SMBIOS_HANDLE           SmbiosHandle;
  EFI_MISC_TEMPERATURE_DATA   *ForType28InputData;
  CHAR8                       *OptionalStrStart;
  CHAR16                      *Temperature;
  UINTN                       TemperatureStrLen;
  UINT16                      TotalSize;
  EFI_STATUS                  Status;
  STRING_REF                  TokenToGet;

  ForType28InputData = (EFI_MISC_TEMPERATURE_DATA *)RecordData;

  //
  // First check for invalid parameters.
  //
  if (RecordData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_TEMPERATURE_STR);
  Temperature = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  TemperatureStrLen = StrLen(Temperature);
  if (TemperatureStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TotalSize = sizeof(SMBIOS_TABLE_TYPE28) + TemperatureStrLen +1 + 1;
  //
  // Two zeros following the last string.
  //
  SmbiosRecord = AllocatePool(TotalSize);
  ZeroMem(SmbiosRecord, TotalSize);
  CopyMem(SmbiosRecord, ForType28InputData,sizeof(SMBIOS_TABLE_TYPE28));

  OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
  UnicodeStrToAsciiStr(Temperature, OptionalStrStart);

  DbgPrint(DEBUG_INFO,"%a() (type:%d) (size:0x%x)\n",__FUNCTION__,SmbiosRecord->Hdr.Type,TotalSize);
  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);

  FreePool(SmbiosRecord);
  return Status;
}

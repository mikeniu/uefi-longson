/** @file
  boot information boot time changes.
  SMBIOS type 11.

  Copyright (c) 2009 - 2011, Intel Corporation. All rights reserved.<BR>
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
// Static (possibly build generated) type 11.
//
MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE11, MiscOemString) = {
  {
    EFI_SMBIOS_TYPE_OEM_STRINGS,
    sizeof(SMBIOS_TABLE_TYPE11),
    0xb00
  },
  1   //STRING_TOKEN(STR_MISC_OEM_STRING)
};

/**
  This function makes boot time changes to the contents of the
  MiscOemString (Type 11).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
MISC_SMBIOS_TABLE_FUNCTION (
  MiscOemString
  )
{
  UINTN                    OemStrLen;
  CHAR8                    *OptionalStrStart;
  EFI_STATUS               Status;
  EFI_STRING               OemStr;
  STRING_REF               TokenToGet;
  EFI_SMBIOS_HANDLE        SmbiosHandle;
  SMBIOS_TABLE_TYPE11      *SmbiosRecord;

  //
  // First check for invalid parameters.
  //
  if (RecordData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_OEM_STRING);
  OemStr = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  OemStrLen = StrLen(OemStr);
  if (OemStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  SmbiosRecord = AllocateZeroPool(sizeof (SMBIOS_TABLE_TYPE11) + OemStrLen + 1 + 1);
  CopyMem(SmbiosRecord, RecordData, sizeof (SMBIOS_TABLE_TYPE11));

  OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
  UnicodeStrToAsciiStr(OemStr, OptionalStrStart);

  DbgPrint(DEBUG_INFO,"%a() (type:%d) (size:0x%x)\n",__FUNCTION__,SmbiosRecord->Hdr.Type,sizeof (SMBIOS_TABLE_TYPE11) + OemStrLen + 1 + 1);
  Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);

  FreePool(SmbiosRecord);
  return Status;
}

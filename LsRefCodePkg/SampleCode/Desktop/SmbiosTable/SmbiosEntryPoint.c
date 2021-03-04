/**@file

  Copyright (c) 2006 - 2012, Intel Corporation. All rights reserved.<BR>
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
#include <Protocol/PciIo.h>
#include <Uefi/UefiSpec.h>

//
// Data Table.
//
EFI_MISC_SMBIOS_DATA_TABLE  mSmbiosDataTable[] = {
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION( MiscBiosInfo,MiscBiosInfo ),                             //type 0
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION( MiscSystemInfo, MiscSystemInfo),                         //type 1
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION( MiscBoardInfo, MiscBoardInfo),                           //type 2
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION( MiscChassisInfo, MiscChassisInfo),                       //type 3
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION( MiscProcessorInfo, MiscProcessorInfo),                   //type 4
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION( MiscCacheInfo, MiscCacheInfo),                           //type 7
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION( MiscSystemSlotDesignation, MiscSystemSlotDesignation),   //type 9
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION( MiscOemString, MiscOemString),                           //type 11
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION( MiscPhysicalStoreInfo, MiscPhysicalStoreInfo),           //type 16
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION( MiscStoreDeviceInfo, MiscStoreDeviceInfo),               //type 17
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION( MiscMemMappInfo, MiscMemMappInfo),                       //type 19
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION( MiscTemperatureInfo, MiscTemperatureInfo),               //type 28
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION( MiscBootInformationStatus, MiscBootInformationStatus),   //type 32
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION( MiscPowerSupplyInfo, MiscPowerSupplyInfo),               //type 39
};

//
// Number of Data Table entries.
//
UINTN mSmbiosDataTableEntries = (sizeof mSmbiosDataTable) / sizeof (EFI_MISC_SMBIOS_DATA_TABLE);
EFI_HII_HANDLE  mHiiHandle;

/**
  Add an SMBIOS record.

  @param  Smbios                The EFI_SMBIOS_PROTOCOL instance.
  @param  SmbiosHandle          A unique handle will be assigned to the SMBIOS record.
  @param  Record                The data for the fixed portion of the SMBIOS record. The format of the record is
                                determined by EFI_SMBIOS_TABLE_HEADER.Type. The size of the formatted area is defined
                                by EFI_SMBIOS_TABLE_HEADER.Length and either followed by a double-null (0x0000) or
                                a set of null terminated strings and a null.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added due to lack of system resources.

**/
EFI_STATUS
AddSmbiosRecord (
  IN EFI_SMBIOS_PROTOCOL        *Smbios,
  OUT EFI_SMBIOS_HANDLE         *SmbiosHandle,
  IN EFI_SMBIOS_TABLE_HEADER    *Record
  )
{
  *SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  return Smbios->Add (
                   Smbios,
                   NULL,
                   SmbiosHandle,
                   Record
                   );
}

/**
  Description:
  Standard EFI driver point.  This driver parses the mMiscSubclassDataTable
  structure and reports any generated data to the DataHub.

  @param[in]  ImageHandle     Handle for the image of this driver
  @param[in]  SystemTable     Pointer to the EFI System Table

  @retval     EFI_SUCCESS     The data was successfully reported to the Data Hub.
**/
EFI_STATUS
EFIAPI
SmbiosDriverEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  UINTN                Index;
  EFI_STATUS           EfiStatus;
  EFI_SMBIOS_PROTOCOL  *Smbios;

  EfiStatus = gBS->LocateProtocol(&gEfiSmbiosProtocolGuid, NULL, (VOID**)&Smbios);
  if (EFI_ERROR(EfiStatus)) {
    DEBUG((EFI_D_ERROR, "Could not locate SMBIOS protocol.  %r\n", EfiStatus));
    return EfiStatus;
  }

  mHiiHandle = HiiAddPackages (
                 &gEfiCallerIdGuid,
                 ImageHandle,
                 SmbiosTableStrings,
                 NULL
                 );
  ASSERT (mHiiHandle != NULL);

  for (Index = 0; Index < mSmbiosDataTableEntries; ++Index) {
    //
    // If the entry have a function pointer, just log the data.
    //
    if (mSmbiosDataTable[Index].Function != NULL) {
      EfiStatus = (*mSmbiosDataTable[Index].Function)(
        mSmbiosDataTable[Index].RecordData,
        Smbios
        );

      if (EFI_ERROR(EfiStatus)) {
        DEBUG((EFI_D_ERROR, "Misc smbios store error.  Index=%d, ReturnStatus=%r\n", Index, EfiStatus));
        return EfiStatus;
      }
    }
  }
  return EfiStatus;
}

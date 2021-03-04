/**@file

  Copyright (c) 2006 - 2011, Intel Corporation. All rights reserved.<BR>
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
#ifndef _SMBIOSTABLE_H_
#define _SMBIOSTABLE_H_

#include <FrameworkDxe.h>
#include <IndustryStandard/SmBios.h>
#include <Protocol/Smbios.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/HiiLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>

#define SMBIOS_TYPE_INACTIVE         0x007E
#define SMBIOS_TYPE_END_OF_TABLE     0x007F
#define SMBIOS_STRING_MAX_LENGTH     64
#define SMBIOS_HANDLE_RESERVED_BEGIN 0xFF00

#define LOONGSON_CPU_NAME_REG_ADDR   (PHYS_TO_UNCACHED(0x1fe00000)|0x0020)
//
// Data Table extern definitions.
//
#define MISC_SMBIOS_TABLE_EXTERNS(NAME1, NAME2, NAME3) \
extern NAME1 NAME2 ## Data; \
extern EFI_MISC_SMBIOS_DATA_FUNCTION NAME3 ## Function

// Data Table entries
//
#define MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(NAME1, NAME2) \
{ \
  & NAME1 ## Data, \
  & NAME2 ## Function \
}

//
// Global definition macros.
//
#define MISC_SMBIOS_TABLE_DATA(NAME1, NAME2) \
  NAME1 NAME2 ## Data

#define MISC_SMBIOS_TABLE_FUNCTION(NAME2) \
  EFI_STATUS EFIAPI NAME2 ## Function( \
  IN  VOID                  *RecordData, \
  IN  EFI_SMBIOS_PROTOCOL   *Smbios \
  )

//
//table structure
//
typedef struct {
  UINT8                 AnchorString[4];
  UINT8                 EntryPointStructureChecksum;
  UINT8                 EntryPointLength;
  UINT8                 MajorVersion;
  UINT8                 MinorVersion;
  UINT16                MaxStructureSize;
  UINT8                 EntryPointRevision;
  UINT8                 FormattedArea[5];
  UINT8                 IntermediateAnchorString[5];
  UINT8                 IntermediateChecksum;
  UINT16                TableLength;
  UINT32                TableAddress;
  UINT16                NumberOfSmbiosStructures;
  UINT8                 SmbiosBcdRevision;
} SMBIOS_TABLE_STRUCTURE;

//
//table header
//
typedef struct {
  UINT8                 Type;
  UINT8                 Length;
  UINT16                Handle;
} SMBIOS_TABLE_HEADER;

///
/// Memory Array Mapped Address (Type 19).
///
/// This structure provides the address mapping for a Physical Memory Array.
/// One structure is present for each contiguous address range described.
///
typedef struct {
  SMBIOS_STRUCTURE      Hdr;
  UINT32                StartingAddress;
  UINT32                EndingAddress;
  UINT16                MemoryArrayHandle;
  UINT8                 PartitionWidth;
  //
  // Add for smbios 2.7
  //
  UINT64                ExtendedStartingAddress;
  UINT64                ExtendedEndingAddress;
} EFI_MISC_MEMORY_ARRAY_MAPPED_DATA;

//
//type 28
//
typedef struct {
  SMBIOS_TABLE_HEADER   Hdr;
  SMBIOS_TABLE_STRING   Description;
  UINT8                 LocationAndStatus;
  UINT16                MaximumValue;
  UINT16                MinimumValue;
  UINT16                Resolution;
  UINT16                Tolerance;
  UINT16                Accuracy;
  UINT32                OEMDefined;
  UINT16                NominalValue;
}EFI_MISC_TEMPERATURE_DATA ;


///
/// System Boot Information (Type 32).
///
/// The client system firmware, e.g. BIOS, communicates the System Boot Status to the
/// client's Pre-boot Execution Environment (PXE) boot image or OS-present management
/// application via this structure. When used in the PXE environment, for example,
/// this code identifies the reason the PXE was initiated and can be used by boot-image
/// software to further automate an enterprise's PXE sessions.  For example, an enterprise
/// could choose to automatically download a hardware-diagnostic image to a client whose
/// reason code indicated either a firmware- or operating system-detected hardware failure.
///
typedef struct {
  SMBIOS_STRUCTURE                        Hdr;
  UINT8                                   Reserved[6];
  UINT8                                   BootStatus;     ///< The enumeration value from MISC_BOOT_INFORMATION_STATUS_DATA_TYPE.
}EFI_MISC_BOOT_INFORMATION_STATUS_DATA ;

//
//type 127
//
typedef struct {
  SMBIOS_TABLE_HEADER   Hdr;
} EFI_MISC_ENDTABLE_DATA;

//
// Data table entry update function.
//
typedef EFI_STATUS (EFIAPI EFI_MISC_SMBIOS_DATA_FUNCTION) (
  IN  VOID                 *RecordData,
  IN  EFI_SMBIOS_PROTOCOL  *Smbios
  );

//
// Data table entry definition.
//
typedef struct {
  //
  // intermediat input data for SMBIOS record
  //
  VOID                              *RecordData;
  EFI_MISC_SMBIOS_DATA_FUNCTION     *Function;
} EFI_MISC_SMBIOS_DATA_TABLE;

//
// Data Table
//
extern SMBIOS_TABLE_TYPE0  MiscBiosInfoData;                                //0
extern SMBIOS_TABLE_TYPE1  MiscSystemInfoData;                              //1
extern SMBIOS_TABLE_TYPE2  MiscBoardInfoData;                               //2
extern SMBIOS_TABLE_TYPE3  MiscChassisInfoData;                             //3
extern SMBIOS_TABLE_TYPE4  MiscProcessorInfoData;                           //4
extern SMBIOS_TABLE_TYPE7  MiscCacheInfoData;                               //7
extern SMBIOS_TABLE_TYPE9  MiscSystemSlotDesignationData;                   //9
extern SMBIOS_TABLE_TYPE11 MiscOemStringData;                               //11
extern SMBIOS_TABLE_TYPE16 MiscPhysicalStoreInfoData;                       //16
extern SMBIOS_TABLE_TYPE17 MiscStoreDeviceInfoData;                         //17
extern EFI_MISC_MEMORY_ARRAY_MAPPED_DATA  MiscMemMappInfoData;              //19
extern EFI_MISC_TEMPERATURE_DATA MiscTemperatureInfoData;                   //28
extern EFI_MISC_BOOT_INFORMATION_STATUS_DATA MiscBootInformationStatusData; //32
extern SMBIOS_TABLE_TYPE39 MiscPowerSupplyInfoData;                         //39

//
// Function Table
//
extern EFI_STATUS EFIAPI MiscBiosInfoFunction(
  IN  VOID *RecordData,
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
);
extern EFI_STATUS EFIAPI MiscSystemInfoFunction(
  IN  VOID *RecordData,
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
);
extern EFI_STATUS EFIAPI MiscBoardInfoFunction(
  IN  VOID *RecordData,
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
);
extern EFI_STATUS EFIAPI MiscChassisInfoFunction(
  IN  VOID *RecordData,
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
);
extern EFI_STATUS EFIAPI MiscProcessorInfoFunction(
  IN  VOID *RecordData,
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
);
extern EFI_STATUS EFIAPI MiscPhysicalStoreInfoFunction(
  IN  VOID *RecordData,
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
);
extern EFI_STATUS EFIAPI MiscStoreDeviceInfoFunction(
  IN  VOID *RecordData,
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
);
extern EFI_STATUS EFIAPI MiscSystemSlotDesignationFunction(
  IN  VOID *RecordData,
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
);
extern EFI_STATUS EFIAPI MiscTemperatureInfoFunction(
  IN  VOID *RecordData,
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
);
extern EFI_STATUS EFIAPI MiscOemStringFunction(
  IN  VOID *RecordData,
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
);
extern EFI_STATUS EFIAPI MiscBootInformationStatusFunction(
  IN  VOID *RecordData,
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
);
extern EFI_STATUS EFIAPI MiscMemMappInfoFunction(
  IN  VOID *RecordData,
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
);
extern EFI_STATUS EFIAPI MiscCacheInfoFunction(
  IN  VOID *RecordData,
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
);
extern EFI_STATUS EFIAPI MiscPowerSupplyInfoFunction(       //Type 39
  IN  VOID *RecordData,
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
);

//
// Data Table Array Entries
//
extern UINTN                        mSmbiosDataTableEntries;
extern EFI_HII_HANDLE               mHiiHandle;

//must be same with BaseName
extern UINT8 SmbiosTableStrings[];

// Type 9 PCI device
typedef struct {
  UINTN SegmentId;
  UINTN BusId;
  UINTN DeviceId;
  UINTN FunctionId;
} PCI_DEVICE_ID;

EFI_STATUS
AddSmbiosRecord (
  IN EFI_SMBIOS_PROTOCOL        *Smbios,
  OUT EFI_SMBIOS_HANDLE         *SmbiosHandle,
  IN EFI_SMBIOS_TABLE_HEADER    *Record
  );
#endif

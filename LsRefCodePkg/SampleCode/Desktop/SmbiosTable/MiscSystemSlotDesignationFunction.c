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

#include <Protocol/PciRootBridgeIo.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Protocol/PciIo.h>
#include "SmbiosTable.h"

//
// Static (possibly build generated) type 9.
//
MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE9, MiscSystemSlotDesignation) = {
  {
    EFI_SMBIOS_TYPE_SYSTEM_SLOTS,
    sizeof(SMBIOS_TABLE_TYPE9),
    0x900
  }, 0
};

VOID
GetSlotDesignationString(
  UINT8                   SlotId,
  UINT8                   SocketNum,
  STRING_REF              *TokenToGet
  )
{
  if (SocketNum == 1) { /* Loongson 3A5000-7A1000 single way CRB */
    switch (SlotId) {
      case 0:    /* Ls7A1000 F1 X4, Dev 0xD */
        *TokenToGet = STRING_TOKEN(STR_MISC_SYSTEM_SLOT_PCIEX4);
        break;

      case 1:    /* Ls7A1000 G1 X8, Dev 0x11 */
        *TokenToGet = STRING_TOKEN(STR_MISC_SYSTEM_SLOT_PCIEX8_1);
        break;

      case 2:    /* Ls7A1000 G0 X8, Dev 0xF */
        *TokenToGet = STRING_TOKEN(STR_MISC_SYSTEM_SLOT_PCIEX8_2);
        break;

      case 3:    /* Ls7A1000 H X8, Dev 0x13 */
        *TokenToGet = STRING_TOKEN(STR_MISC_SYSTEM_SLOT_PCIEX8_3);
        break;

      default:
        break;
    }
  } else if (SocketNum == 2) { /* Loongson 3AB5000-7A1000 Dual CPU server CRB */
    switch (SlotId) {
      case 0:    /* Ls7A1000 H X8, Dev 0x13 */
        *TokenToGet = STRING_TOKEN(STR_MISC_SYSTEM_SLOT_PCIEX8_1);
        break;

      case 1:    /* Ls7A1000 G0 X8, Dev 0xF */
        *TokenToGet = STRING_TOKEN(STR_MISC_SYSTEM_SLOT_PCIEX8_2);
        break;

      case 2:    /* Ls7A1000 G1-PEX8632 PE2 X8, Dev 0x11 */
        *TokenToGet = STRING_TOKEN(STR_MISC_SYSTEM_SLOT_PCIEX8_6);
        break;

      case 3:    /* Ls7A1000 G1-PEX8632 PE1 X8, Dev 0x11 */
        *TokenToGet = STRING_TOKEN(STR_MISC_SYSTEM_SLOT_PCIEX8_3);
        break;

      case 4:    /* Ls7A1000 G1-PEX8632 PE3 X8, Dev 0x11 */
        *TokenToGet = STRING_TOKEN(STR_MISC_SYSTEM_SLOT_PCIEX8_9);
        break;

      default:
        break;
    }
  }
}

EFI_STATUS
Type9SlotDesignationInitialize (
  SMBIOS_TABLE_TYPE9              *SmbiosRecord,
  PCI_DEVICE_ID                   **PciDeviceId,
  UINT8                           SocketNum,
  UINT8                           SlotId
  )
{
  UINT8      i = 0;
  UINT8      FunId = 0;

  SmbiosRecord->SegmentGroupNum = 0xFFFF;
  SmbiosRecord->BusNum = 0xFF;
  SmbiosRecord->DevFuncNum = 0xFF;
  SmbiosRecord->CurrentUsage = 0x3; /* Available */

  if (SocketNum == 1) { /* Loongson 3A5000-7A1000 single way CRB */
    switch (SlotId) {
      case 0:    /* Ls7A1000 F1 X4, Dev 0xD */
        FunId = 0xD;
        SmbiosRecord->SlotType = 0xAE;        /* PCIe Gen2 X4 */
        SmbiosRecord->SlotDataBusWidth = 0xA; /* X4 */
        break;

      case 1:    /* Ls7A1000 G1 X8, Dev 0x11 */
        FunId = 0x11;
        SmbiosRecord->SlotType = 0xAF;        /* PCIe Gen2 X8 */
        SmbiosRecord->SlotDataBusWidth = 0xB; /* X8 */
        break;

      case 2:    /* Ls7A1000 G0 X8, Dev 0xF */
        FunId = 0xF;
        SmbiosRecord->SlotType = 0xAF;        /* PCIe Gen2 X8 */
        SmbiosRecord->SlotDataBusWidth = 0xB; /* X8 */
        break;

      case 3:    /* Ls7A1000 H X8, Dev 0x13 */
        FunId = 0x13;
        SmbiosRecord->SlotType = 0xAF;        /* PCIe Gen2 X8 */
        SmbiosRecord->SlotDataBusWidth = 0xB; /* X8 */
        break;

      default:
        break;
    }
    SmbiosRecord->SlotID = SlotId;
  } else if (SocketNum == 2) { /* Loongson 3AB5000-7A1000 Dual CPU server CRB */
    switch (SlotId) {
      case 0:    /* Ls7A1000 H X8, Dev 0x13 */
        FunId = 0x13;
        SmbiosRecord->SlotID = 0x1;
        break;

      case 1:    /* Ls7A1000 G0 X8, Dev 0xF */
        FunId = 0xF;
        SmbiosRecord->SlotID = 0x2;
        break;

      case 2:    /* Ls7A1000 G1-PEX8632 PE2 X8, Dev 0x11 */
        FunId = 0xFF; /* Not support by now */
        SmbiosRecord->SlotID = 0x6;
        break;

      case 3:    /* Ls7A1000 G1-PEX8632 PE1 X8, Dev 0x11 */
        FunId = 0xFF; /* Not support by now */
        SmbiosRecord->SlotID = 0x8;
        break;

      case 4:    /* Ls7A1000 G1-PEX8632 PE3 X8, Dev 0x11 */
        FunId = 0xFF; /* Not support by now */
        SmbiosRecord->SlotID = 0x9;
        break;

      default:
        break;
    }
    SmbiosRecord->SlotType = 0xAF;        /* PCIe Gen2 X8 */
    SmbiosRecord->SlotDataBusWidth = 0xB; /* X8 */
  }

  do {
    if ((FunId != 0xFF) && (PciDeviceId[i]->DeviceId == FunId) && (PciDeviceId[i + 1]->BusId != 0)) {
      /* Bus, deivce, funciton numbers initialize, to get the main bus device sub bus device */
      SmbiosRecord->SegmentGroupNum = PciDeviceId[i + 1]->SegmentId;
      SmbiosRecord->BusNum = PciDeviceId[i + 1]->BusId;
      SmbiosRecord->DevFuncNum = ((PciDeviceId[i + 1]->DeviceId) << 3) |
                                ((PciDeviceId[i + 1]->DeviceId) & 7);
      SmbiosRecord->CurrentUsage = 0x4; /* In use */
    }
    i++;
  } while (PciDeviceId[i + 1] != NULL);

  SmbiosRecord->SlotDesignation = 1;
  SmbiosRecord->SlotCharacteristics1.Provides33Volts = 1;
  SmbiosRecord->SlotLength= 0x3;  /* Short */

  return EFI_SUCCESS;
}

EFI_STATUS
DetectPcieSlot (
  IN  UINT8          SocketNum,
  OUT UINT8          *DeviceNum,
  OUT PCI_DEVICE_ID  ***PciDeviceMap
  )
{
  EFI_STATUS                       Status;
  UINT8                            i;
  UINTN                            NumberOfHandles;
  EFI_HANDLE                       *pHandleBuffer;
  EFI_PCI_IO_PROTOCOL              *pPciIoProtocol;
  UINTN                            SegmentId;
  UINTN                            BusId;
  UINTN                            DeviceId;
  UINTN                            FunctionId;
  PCI_DEVICE_ID                    **PciDeviceId;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &pHandleBuffer
                  );
  PciDeviceId = AllocateZeroPool(sizeof(PCI_DEVICE_ID *) * (NumberOfHandles + 1));
  for (i = 0; i < NumberOfHandles; i++) {
    PciDeviceId[i] = AllocateZeroPool(sizeof(PCI_DEVICE_ID));
    Status = gBS->HandleProtocol (
                    pHandleBuffer[i],
                    &gEfiPciIoProtocolGuid,
                    (VOID *)&pPciIoProtocol
                    );
    if (EFI_ERROR(Status)) {
     continue;
    }

    Status = pPciIoProtocol->GetLocation (
                               pPciIoProtocol,
                               &SegmentId,
                               &BusId,
                               &DeviceId,
                               &FunctionId
                               );
    PciDeviceId[i]->SegmentId = SegmentId;
    PciDeviceId[i]->BusId = BusId;
    PciDeviceId[i]->DeviceId = DeviceId;
    PciDeviceId[i]->FunctionId = FunctionId;
  }
  *PciDeviceMap = PciDeviceId;
  *DeviceNum = i;

  return EFI_SUCCESS;
}

/**
  This function makes boot time changes to the contents of the
  MiscBiosVendor (Type 9).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
MISC_SMBIOS_TABLE_FUNCTION (
  MiscSystemSlotDesignation
  )
{
  EFI_STATUS                      Status;
  EFI_SMBIOS_HANDLE               SmbiosHandle;
  UINT8                           SocketNum;
  UINT8                           DeviceNum;
  UINT8                           MaxSlotNum;
  UINT8                           SlotId;
  UINT8                           i;
  UINTN                           SlotDesignationLen;
  UINTN                           TotalSize;
  CHAR16                          *SlotDesignation;
  CHAR8                           *OptionalStrStart;
  STRING_REF                      TokenToGet;
  SMBIOS_TABLE_TYPE9              *SmbiosRecord;
  PCI_DEVICE_ID                   **PciDeviceId = NULL;

  if (RecordData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

#if (TOT_NODE_NUM == 2)
  SocketNum = 2;
  MaxSlotNum = 5;
#else
  SocketNum = 1;
  MaxSlotNum = 4;
#endif

  Status = DetectPcieSlot(SocketNum, &DeviceNum, &PciDeviceId);
  ASSERT_EFI_ERROR (Status);

  for (SlotId = 0; SlotId < MaxSlotNum; SlotId++) {
    GetSlotDesignationString(SlotId, SocketNum, &TokenToGet);
    SlotDesignation = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
    SlotDesignationLen = StrLen(SlotDesignation);

    if (SlotDesignationLen > SMBIOS_STRING_MAX_LENGTH) {
      return EFI_UNSUPPORTED;
    }

    TotalSize = sizeof(SMBIOS_TABLE_TYPE9) + (SlotDesignationLen + 1);
    SmbiosRecord = AllocateZeroPool(TotalSize);
    CopyMem(SmbiosRecord, RecordData, sizeof (SMBIOS_TABLE_TYPE9));

    Status = Type9SlotDesignationInitialize (SmbiosRecord, PciDeviceId, SocketNum, SlotId);

    OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
    UnicodeStrToAsciiStr(SlotDesignation, OptionalStrStart);
    Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER*) SmbiosRecord);
    ASSERT_EFI_ERROR (Status);
    FreePool(SmbiosRecord);
  }

  for (i = 0; i < DeviceNum; i++) {
    FreePool(PciDeviceId[i]);
  }
  FreePool(PciDeviceId);

  return Status;
}

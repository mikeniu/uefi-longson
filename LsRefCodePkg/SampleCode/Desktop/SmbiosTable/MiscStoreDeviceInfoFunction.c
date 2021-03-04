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
#include <PiPei.h>
#include <Library/MemConfig.h>
#include <Library/PrintLib.h>
#include <McConfig.h>
#include "SmbiosTable.h"
#include "Lib.h"
//
// Static (possibly build generated) Boot info (type 17).
//
MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE17, MiscStoreDeviceInfo) = {
  {
    EFI_SMBIOS_TYPE_MEMORY_DEVICE ,
    sizeof(SMBIOS_TABLE_TYPE17),
    0x1100
  }, 0
};

VOID
GetMemManufacturer (
  UINT64                  NodeId,
  UINT64                  McId,
  UINT64                  DimmId,
  CHAR16                  *Manu,
  LS_MEM_INFO_HOB         *MemInfoHobData
  )
{
  UINT8 Value = 0;
  UINT8 Manufacturer[DDR4_MODULE_MANUFACTURER_LEN] = {0};

  ASSERT(Manu != NULL && MemInfoHobData != NULL);

  Value = MemInfoHobData->MemInfo.DdrType;
  if(Value == DDR4){
    Value = MemInfoHobData->MemInfo.Ddr4Data[NodeId][McId][DimmId].Ddr4DimmMap;
    if (Value == 1) {
      CopyMem(Manufacturer, MemInfoHobData->MemInfo.Ddr4Data[NodeId][McId][DimmId].Ddr4Manufacturer, sizeof(UINT8) * DDR4_MODULE_MANUFACTURER_LEN);
      UnicodeSPrint(Manu, sizeof(CHAR16) * (DDR4_MODULE_MANUFACTURER_LEN * 2 + 1), L"%.2x%.2x", \
          *Manufacturer, *(Manufacturer+1));
      DbgPrint(DEBUG_INFO,"DDR4 Manu:%s\n", Manu);
    }
  }else {
    Value = MemInfoHobData->MemInfo.Ddr3Data[NodeId][McId][DimmId].Ddr3DimmMap;
    if (Value == 1) {
      CopyMem(Manufacturer, MemInfoHobData->MemInfo.Ddr3Data[NodeId][McId][DimmId].Ddr3Manufacturer, sizeof(UINT8) * DDR3_MODULE_MANUFACTURER_LEN);
      UnicodeSPrint(Manu, sizeof(CHAR16) * (DDR3_MODULE_MANUFACTURER_LEN * 2 + 1), L"%.2x%.2x", \
          *Manufacturer, *(Manufacturer+1));
      DbgPrint(DEBUG_INFO,"DDR3 Manu:%s\n", Manu);
    }
  }
}

VOID
GetMemPartNum (
  UINT64                  NodeId,
  UINT64                  McId,
  UINT64                  DimmId,
  CHAR8                   *PartNumber,
  LS_MEM_INFO_HOB         *MemInfoHobData
  )
{
  UINT8 Value = 0;
  UINT8 DDR4PartNum[DDR4_MODULE_PART_NUMBER_LEN] = {0};
  UINT8 DDR3PartNum[DDR3_MODULE_PART_NUMBER_LEN] = {0};
  CHAR8 TmpPartNumber[2] = {0};
  UINT8 i = 0;

  ASSERT(PartNumber != NULL && MemInfoHobData != NULL);

  Value = MemInfoHobData->MemInfo.DdrType;
  if (Value == DDR3) {
    Value = MemInfoHobData->MemInfo.Ddr3Data[NodeId][McId][DimmId].Ddr3DimmMap;
    if (Value == 1) {
      CopyMem(DDR3PartNum, MemInfoHobData->MemInfo.Ddr3Data[NodeId][McId][DimmId].Ddr3ModulePartNumber, sizeof(UINT8) * DDR3_MODULE_PART_NUMBER_LEN);

      for (i = 0; i < DDR4_MODULE_PART_NUMBER_LEN; i++) {
        DbgPrint(DEBUG_INFO,"DDR3 PartNum[%d]>%c:0x%x\n", i, *(DDR3PartNum + i), *(DDR3PartNum + i));
        if ((0x20 == *(DDR3PartNum + i)) || (0 == *(DDR3PartNum + i))) {
          break;
        }
        ZeroMem(TmpPartNumber, sizeof(TmpPartNumber));
        AsciiSPrint(TmpPartNumber, sizeof(CHAR8) + 1, "%c", *(DDR3PartNum+i));
        *(PartNumber + i) = TmpPartNumber[0];
      }
    }
  } else {
    Value = MemInfoHobData->MemInfo.Ddr4Data[NodeId][McId][DimmId].Ddr4DimmMap;
    if (Value == 1) {
      CopyMem(DDR4PartNum, MemInfoHobData->MemInfo.Ddr4Data[NodeId][McId][DimmId].Ddr4ModulePartNumber, sizeof(UINT8) * DDR4_MODULE_PART_NUMBER_LEN);
      for (i = 0; i < DDR4_MODULE_PART_NUMBER_LEN; i++) {
        DbgPrint(DEBUG_INFO,"DDR4 PartNum[%d]:%c\n", i, *(DDR4PartNum + i));
        if ((0x20 == *(DDR4PartNum + i)) || (0 == *(DDR4PartNum + i))) {
          break;
        }
        ZeroMem(TmpPartNumber, sizeof(TmpPartNumber));
        AsciiSPrint(TmpPartNumber, sizeof(CHAR8) + 1, "%c", *(DDR4PartNum+i));
        *(PartNumber + i) = TmpPartNumber[0];
      }
    }
  }
}

VOID
GetMemSerialNum (
  UINT64                  NodeId,
  UINT64                  McId,
  UINT64                  DimmId,
  CHAR16                  *Serial,
  LS_MEM_INFO_HOB         *MemInfoHobData
  )
{
  UINT8 Value = 0;
  UINT8 SerialNum[DDR4_MODULE_SERIAL_NUMBER_LEN] = {0};

  ASSERT(Serial != NULL && MemInfoHobData != NULL);

  Value = MemInfoHobData->MemInfo.DdrType;
  if (Value == DDR3) {
    Value = MemInfoHobData->MemInfo.Ddr3Data[NodeId][McId][DimmId].Ddr3DimmMap;
    if (Value == 1) {
      CopyMem(SerialNum, MemInfoHobData->MemInfo.Ddr3Data[NodeId][McId][DimmId].Ddr3ModuleSerialNumber, sizeof(UINT8) * DDR3_MODULE_SERIAL_NUMBER_LEN);
      UnicodeSPrint(Serial, sizeof(CHAR16) * (DDR3_MODULE_SERIAL_NUMBER_LEN * 2 + 1), L"%.2x%.2x%.2x%.2x", \
          *SerialNum, *(SerialNum+1), *(SerialNum+2), *(SerialNum+3));
      DbgPrint(DEBUG_INFO,"DDR3 Serial>%s\n", Serial);
    }
  } else {
    Value = MemInfoHobData->MemInfo.Ddr4Data[NodeId][McId][DimmId].Ddr4DimmMap;
    if (Value == 1) {
      CopyMem(SerialNum, MemInfoHobData->MemInfo.Ddr4Data[NodeId][McId][DimmId].Ddr4ModuleSerialNumber, sizeof(UINT8) * DDR4_MODULE_SERIAL_NUMBER_LEN);
      UnicodeSPrint(Serial, sizeof(CHAR16) * (DDR4_MODULE_SERIAL_NUMBER_LEN * 2 + 1), L"%.2x%.2x%.2x%.2x", \
          *SerialNum, *(SerialNum+1), *(SerialNum+2), *(SerialNum+3));
      DbgPrint(DEBUG_INFO,"DDR4 Serial>%s\n", Serial);
    }
  }
}

VOID
GetSpdDimmSize (
  UINT64 *Size
  )
{
  switch (*Size & 0xF)
  {
    case 0x0:
      *Size = 0x10000000;
      break;
    case 0x1:
      *Size = 0x20000000;
      break;
    case 0x2:
      *Size = 0x40000000;
      break;
    case 0x3:
      *Size = 0x80000000;//2*1024*1024*1024;
      break;
    case 0x4:
      *Size = 0x100000000;//4*1024*1024*1024;
      break;
    case 0x5:
      *Size = 0x200000000;//8*1024*1024*1024;
      break;
    case 0x6:
      *Size = 0x400000000;//16*1024*1024*1024;
      break;
    case 0x7:
      *Size = 0x800000000;//32*1024*1024*1024;
      break;
    case 0x8:
      *Size = 0x300000000;//12*1024*1024*1024;
      break;
    case 0x9:
      *Size = 0x600000000;//24*1024*1024*1024;
      break;
  }
}

UINT16
GetVoltage (
  UINT64                  NodeId,
  UINT64                  McId,
  UINT64                  DimmId,
  LS_MEM_INFO_HOB         *MemInfoHobData
  )
{
  UINT16 Voltage = 0;
  UINT8 Value = 0;

  Value = MemInfoHobData->MemInfo.DdrType;
  if (Value == DDR3) {
    Value = MemInfoHobData->MemInfo.Ddr3Data[NodeId][McId][DimmId].Ddr3DimmMap;
    if (Value == 1) {
      Voltage = MemInfoHobData->MemInfo.Ddr3Data[NodeId][McId][DimmId].Ddr3SdramVoltage;
      if (Voltage & 0x3) {
        Voltage = 1.2;       /*VDD:1.2V*/
      }
    }
  } else {
    Value = MemInfoHobData->MemInfo.Ddr4Data[NodeId][McId][DimmId].Ddr4DimmMap;
    if (Value == 1) {
      Voltage = MemInfoHobData->MemInfo.Ddr4Data[NodeId][McId][DimmId].Ddr4SdramVoltage;
      if (!(Voltage & 0x1)) {
        Voltage = 1.5;       /*VDD:1.5V*/
      } else if (Voltage & 0x2) {
        Voltage = 1.35;       /*VDD:1.5V*/
      } else if (Voltage & 0x4) {
        Voltage = 1.2;       /*VDD:1.2V*/
      }
    }
  }
  DbgPrint(DEBUG_INFO,"Voltage:0x%x\n", Voltage);

  return Voltage;
}

UINT16
GetMemDevTypeDetailField(
  UINT64                  NodeId,
  UINT64                  McId,
  UINT64                  DimmId,
  LS_MEM_INFO_HOB         *MemInfoHobData
  )
{
  UINT8 Field = 0;
  UINT8 Value = 0;
  UINT16 Ret = 0;

  Value = MemInfoHobData->MemInfo.DdrType;
  if (Value == DDR4) {
    Value = MemInfoHobData->MemInfo.Ddr4Data[NodeId][McId][DimmId].Ddr4DimmMap;
    if (Value == 1) {
      Field = MemInfoHobData->MemInfo.Ddr4Data[NodeId][McId][DimmId].Ddr4ModuleType;
      switch (Field)
      {
        /*Registered*/
        case 1:
        case 5:
        case 8:
          Ret = (1 << 13);
          break;

          /*Unbuffered*/
        case 2:
        case 3:
        case 6:
        case 9:
        case 12:
        case 13:
          Ret = (1 << 14);
          break;

          /*Reserved*/
        case 4:
        case 7:
        case 10:
        case 11:
        case 14:
        case 15:
          Ret = (1 << 0);
          break;
      }
      DbgPrint(DEBUG_INFO,"Field:0x%x, Ret:0x%x\n", Field, Ret);
    }
  } else {
    Value = MemInfoHobData->MemInfo.Ddr3Data[NodeId][McId][DimmId].Ddr3DimmMap;
    if (Value == 1) {
      Field = MemInfoHobData->MemInfo.Ddr3Data[NodeId][McId][DimmId].Ddr3ModuleType;
      switch (Field)
      {
        /*Registered*/
        case 1:
        case 5:
        case 8:
          Ret = (1 << 13);
          break;

          /*Unbuffered*/
        case 2:
        case 3:
        case 6:
        case 9:
          Ret = (1 << 14);
          break;

          /*Reserved*/
        case 4:
        case 7:
        case 10:
        case 11:
          Ret = (1 << 0);
          break;
      }
      DbgPrint(DEBUG_INFO,"Field:0x%x Ret:0x%x\n",Field, Ret);
    } else {
      DbgPrint(DEBUG_INFO,"GetMemDevTypeDetailField error.\n");
    }
  }

  return Ret;
}

UINT16
GetDimmSize (
  UINT64                  NodeId,
  UINT64                  McId,
  UINT64                  DimmId,
  LS_MEM_INFO_HOB         *MemInfoHobData
  )
{

  UINT64 MemSize = 0;
  UINT64 SdramCapacity = 0;
  UINT8  PrimaryBusWidth = 0;
  UINT8  SdramWidth = 0;
  UINT8  LogicalRanksPerDimm = 0;
  UINT8  Value = 0;

    Value = MemInfoHobData->MemInfo.DdrType;
    if (Value == DDR3) {
      Value = MemInfoHobData->MemInfo.Ddr3Data[NodeId][McId][DimmId].Ddr3DimmMap;
      if (Value == 1) {
        SdramCapacity = MemInfoHobData->MemInfo.Ddr3Data[NodeId][McId][DimmId].Ddr3SdramBankCapacity;
        GetSpdDimmSize(&SdramCapacity);

        PrimaryBusWidth = MemInfoHobData->MemInfo.Ddr3Data[NodeId][McId][DimmId].Ddr3ModuleMemPrimaryBusWidth;
        SdramWidth = MemInfoHobData->MemInfo.Ddr3Data[NodeId][McId][DimmId].Ddr3ModuleORGDeviceWidth;
        LogicalRanksPerDimm = MemInfoHobData->MemInfo.Ddr3Data[NodeId][McId][DimmId].Ddr3ModuleORGNumberRanks;
        MemSize += SdramCapacity / 8 * PrimaryBusWidth / SdramWidth * LogicalRanksPerDimm;
      }
    } else if (Value == DDR4) {
      Value = MemInfoHobData->MemInfo.Ddr4Data[NodeId][McId][DimmId].Ddr4DimmMap;
      if (Value == 1) {
        SdramCapacity = MemInfoHobData->MemInfo.Ddr4Data[NodeId][McId][DimmId].Ddr4SdramBankCapacity;
        GetSpdDimmSize(&SdramCapacity);

        PrimaryBusWidth = MemInfoHobData->MemInfo.Ddr4Data[NodeId][McId][DimmId].Ddr4ModuleMemPrimaryBusWidth;
        SdramWidth = MemInfoHobData->MemInfo.Ddr4Data[NodeId][McId][DimmId].Ddr4ModuleORGDeviceWidth;
        LogicalRanksPerDimm = MemInfoHobData->MemInfo.Ddr4Data[NodeId][McId][DimmId].Ddr4ModuleORGNumberRanks;
        MemSize += SdramCapacity / 8 * PrimaryBusWidth / SdramWidth * LogicalRanksPerDimm;
      }
  }
  DbgPrint(DEBUG_INFO,"MemSize=0x%x\n", MemSize);

  return MemSize/(1024*1024);
}

UINT8
GetDataWidth (
  UINT64                  NodeId,
  UINT64                  McId,
  UINT64                  DimmId,
  LS_MEM_INFO_HOB         *MemInfoHobData
  )
{
  UINT8 Value = 0;
  UINT8 ModuleWidth = 0;

  Value = MemInfoHobData->MemInfo.DdrType;
  if (Value == DDR3) {
    Value = MemInfoHobData->MemInfo.Ddr3Data[NodeId][McId][DimmId].Ddr3DimmMap;
    if (Value == 1) {
      ModuleWidth = MemInfoHobData->MemInfo.Ddr3Data[NodeId][McId][DimmId].Ddr3ModuleMemPrimaryBusWidth;
      DbgPrint(DEBUG_INFO,"DDR4 ModuleWidth:%d\n", ModuleWidth);
    }
  } else {
    Value = MemInfoHobData->MemInfo.Ddr4Data[NodeId][McId][DimmId].Ddr4DimmMap;
    if (Value == 1) {
      ModuleWidth = MemInfoHobData->MemInfo.Ddr4Data[NodeId][McId][DimmId].Ddr4ModuleMemPrimaryBusWidth;
      DbgPrint(DEBUG_INFO,"DDR4 ModuleWidth:%d\n", ModuleWidth);
    }

  }
  return ModuleWidth;
}

UINT8
GetTotalWidth(
  UINT64                  NodeId,
  UINT64                  McId,
  UINT64                  DimmId,
  LS_MEM_INFO_HOB         *MemInfoHobData
  )
{
  UINT8 DataWidth = 0;
  UINT8 Value = 0;

  Value = MemInfoHobData->MemInfo.DdrType;
  if(Value == DDR4){
    Value = MemInfoHobData->MemInfo.Ddr4Data[NodeId][McId][DimmId].Ddr4DimmMap;
    if (Value == 1) {
      DataWidth = MemInfoHobData->MemInfo.Ddr4Data[NodeId][McId][DimmId].Ddr4ModuleMemBusWidth;
    }
  } else {
    Value = MemInfoHobData->MemInfo.Ddr3Data[NodeId][McId][DimmId].Ddr3DimmMap;
    if (Value == 1) {
      DataWidth = MemInfoHobData->MemInfo.Ddr3Data[NodeId][McId][DimmId].Ddr3ModuleMemBusWidth;
    }
  }

  DbgPrint(DEBUG_INFO,"DataWidth:%d\n", DataWidth);

  return DataWidth;
}

EFI_STATUS
GetDimmStr(UINT8 DimmNum,CHAR16 **DimmStr,UINT8 *DimmLen)
{
  STRING_REF              TokenToGet;
  CHAR16                  *Dimm;
  UINT8                   DimmStrLen;

  if(!DimmStr){
    return EFI_INVALID_PARAMETER;
  }

  switch(DimmNum)
  {
    case 0:
      TokenToGet = STRING_TOKEN (STR_MISC_STOREDEVICE_DIMM1STR);
      break;
    case 1:
      TokenToGet = STRING_TOKEN (STR_MISC_STOREDEVICE_DIMM2STR);
      break;
    case 2:
      TokenToGet = STRING_TOKEN (STR_MISC_STOREDEVICE_DIMM3STR);
      break;
    case 3:
      TokenToGet = STRING_TOKEN (STR_MISC_STOREDEVICE_DIMM4STR);
      break;
    case 4:
      TokenToGet = STRING_TOKEN (STR_MISC_STOREDEVICE_DIMM5STR);
      break;
    case 5:
      TokenToGet = STRING_TOKEN (STR_MISC_STOREDEVICE_DIMM6STR);
      break;
    case 6:
      TokenToGet = STRING_TOKEN (STR_MISC_STOREDEVICE_DIMM7STR);
      break;
    case 7:
      TokenToGet = STRING_TOKEN (STR_MISC_STOREDEVICE_DIMM8STR);
      break;
    default:
      return EFI_INVALID_PARAMETER;
  }

  Dimm = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  DimmStrLen = StrLen(Dimm);
  if (DimmStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }
  *DimmLen = DimmStrLen;
  *DimmStr = Dimm;

  return EFI_SUCCESS;
}

UINT8
GetMemoryType(UINT8 DdrType)
{
  switch (DdrType)
  {
    case 0x08:
      return 0x13;
    case 0x0b:
      return 0x18;
    case 0x0c:
      return 0x1A;
    default:
      return 0;
  }
}

EFI_STATUS
Type17ProcessorTableInitialize (
  UINT64                  NodeId,
  UINT64                  McId,
  UINT64                  DimmId,
  SMBIOS_TABLE_TYPE17     *SmbiosRecord,
  LS_MEM_INFO_HOB         *MemInfoHobData
  )
{
  UINT16 TypeDetail = 0;

  ASSERT(SmbiosRecord != NULL && MemInfoHobData != NULL);

  SmbiosRecord->MemoryArrayHandle = 0;
  SmbiosRecord->MemoryErrorInformationHandle = 0xfffe;
  SmbiosRecord->FormFactor = MemoryFormFactorDimm;
  SmbiosRecord->DeviceSet = 0;
  SmbiosRecord->DeviceLocator = 1;
  SmbiosRecord->BankLocator = 2;
  SmbiosRecord->TypeDetail.Reserved = 0;
  SmbiosRecord->TypeDetail.Other = 0;
  SmbiosRecord->TypeDetail.Unknown = 0;
  SmbiosRecord->TypeDetail.FastPaged = 0;
  SmbiosRecord->TypeDetail.StaticColumn = 0;
  SmbiosRecord->TypeDetail.PseudoStatic = 0;
  SmbiosRecord->TypeDetail.Rambus = 0;
  SmbiosRecord->TypeDetail.Synchronous = 0;
  SmbiosRecord->TypeDetail.Cmos = 0;
  SmbiosRecord->TypeDetail.Edo = 0;
  SmbiosRecord->TypeDetail.WindowDram = 0;
  SmbiosRecord->TypeDetail.CacheDram = 0;
  SmbiosRecord->TypeDetail.Nonvolatile = 0;
  SmbiosRecord->TypeDetail.Registered = 0;
  SmbiosRecord->TypeDetail.Unbuffered = 0;
  SmbiosRecord->TypeDetail.LrDimm = 0;
  SmbiosRecord->Manufacturer = 3;
  SmbiosRecord->SerialNumber = 4;
  SmbiosRecord->AssetTag = 5;
  SmbiosRecord->PartNumber = 6;
  // Add for smbios 2.6
  SmbiosRecord->Attributes = 0;
  // Add for smbios 2.7
  SmbiosRecord->ExtendedSize = 0;
  SmbiosRecord->ConfiguredMemoryClockSpeed = CLK_DDR/1000;
  // Add for smbios 2.8.
  SmbiosRecord->MinimumVoltage = 0;
  SmbiosRecord->MaximumVoltage = 0;

  SmbiosRecord->Speed = MemInfoHobData->MemInfo.Freq;

  SmbiosRecord->MemoryType = GetMemoryType(MemInfoHobData->MemInfo.DdrType);
  SmbiosRecord->Size = GetDimmSize(NodeId, McId, DimmId, MemInfoHobData);
  SmbiosRecord->TotalWidth = GetDataWidth(NodeId, McId, DimmId, MemInfoHobData); /*DRAM_MODE_BUS_WIDTH_BASE :bit 0~2*/
  SmbiosRecord->DataWidth = GetTotalWidth(NodeId, McId, DimmId, MemInfoHobData); /*DRAM_MODE_BUS_WIDTH_BASE :bit 0~4*/
  SmbiosRecord->ConfiguredVoltage = GetVoltage(NodeId, McId, DimmId, MemInfoHobData);
  TypeDetail = GetMemDevTypeDetailField(NodeId, McId, DimmId, MemInfoHobData);
  if (TypeDetail & (1 << 13)) {
    SmbiosRecord->TypeDetail.Registered = 1;
  } else if (TypeDetail & (1 << 14)) {
    SmbiosRecord->TypeDetail.Unbuffered = 1;
  } else {
    SmbiosRecord->TypeDetail.Reserved = 1;
  }

  return EFI_SUCCESS;
}

/**
  This function makes boot time changes to the contents of the
  MiscBiosVendor (Type 17).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

 **/
MISC_SMBIOS_TABLE_FUNCTION(MiscStoreDeviceInfo)
{
  SMBIOS_TABLE_TYPE17       *SmbiosRecord;
  LS_CPU_INFO_HOB           CpuInfoHobData;
  LS_MEM_INFO_HOB           MemInfoHobData;
  EFI_SMBIOS_HANDLE         SmbiosHandle;
  EFI_STATUS                Status = EFI_SUCCESS;
  STRING_REF                TokenToGet;
  CHAR8                     *OptionalStrStart;
  CHAR16                    *AssetTag;
  CHAR16                    *BankStr;
  CHAR16                    *DimmStr;
  UINTN                     ManufactureStrLen = 0;
  UINTN                     SerialNumStrLen = 0;
  UINTN                     AssetTagStrLen;
  UINTN                     PartNumStrLen = 0;
  UINTN                     BankStrLen = 0;
  UINT8                     DimmStrLen = 0;
  CHAR16                    *PartNum;
  UINT8                     MemType = 0;
  UINT8                     NodeId = 0;
  UINT8                     McId = 0;
  UINT8                     DimmId = 0;

  //
  // First check for invalid parameters.
  //
  if (RecordData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_STOREDEVICE_ASSETTAG);
  AssetTag = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  AssetTagStrLen = StrLen(AssetTag);
  if (AssetTagStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_STOREDEVICE_BANKSTR);
  BankStr = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  BankStrLen = StrLen(BankStr);
  if (BankStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }
  TokenToGet = STRING_TOKEN (STR_MISC_STOREDEVICE_PARTNUMBER);
  PartNum = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  PartNumStrLen = StrLen(PartNum);
  if (PartNumStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  ManufactureStrLen = sizeof(CHAR8) * (DDR4_MODULE_MANUFACTURER_LEN * 2);
  SerialNumStrLen = sizeof(CHAR8) * (DDR4_MODULE_SERIAL_NUMBER_LEN * 2);

  GetCpuInfoHob (&CpuInfoHobData);
  GetMemInfoHob (&MemInfoHobData);

  for (NodeId = 0; NodeId < CpuInfoHobData.NodeNum; NodeId++) {
    for (McId = 0; McId < MCMAX; McId++) {
      for (DimmId = 0; DimmId < DIMMMAX; DimmId++) {
        MemType = MemInfoHobData.MemInfo.DdrType;
        if (DDR3 == MemType) {
          PartNumStrLen = DDR3_MODULE_PART_NUMBER_LEN;
        } else if (DDR4 == MemType) {
          PartNumStrLen = DDR4_MODULE_PART_NUMBER_LEN;
        } else {
          continue;
        }

        CHAR8 *PartNum = AllocatePool(PartNumStrLen);
        ZeroMem(PartNum, PartNumStrLen + 1);

        GetMemPartNum(NodeId, McId, DimmId, PartNum, &MemInfoHobData);
        /*PartNum maybe not full, it will be filled with 0x20.  Unused digits are coded as ASCII blanks (0x20).*/
        PartNumStrLen = AsciiStrLen(PartNum);

        CHAR16 *Manufacturer = AllocatePool(sizeof(CHAR16) * (DDR4_MODULE_MANUFACTURER_LEN * 2) + 1);
        ZeroMem(Manufacturer, ManufactureStrLen + 1);
        GetMemManufacturer(NodeId, McId, DimmId, Manufacturer, &MemInfoHobData);

        CHAR16 *SerialNum = AllocatePool(sizeof(CHAR16) * (DDR4_MODULE_SERIAL_NUMBER_LEN * 2) + 1);
        ZeroMem(SerialNum, SerialNumStrLen + 1);

        GetMemSerialNum(NodeId, McId, DimmId, SerialNum, &MemInfoHobData);

        UINT8 DimmNum = NodeId * DIMMMAX * MCMAX + McId * DIMMMAX + DimmId;
        GetDimmStr(DimmNum, &DimmStr, &DimmStrLen);
        DbgPrint(DEBUG_INFO,"DimmStr:%s\n",DimmStr);

        UINT64 TotalSize = sizeof(SMBIOS_TABLE_TYPE17) \
                           +(DimmStrLen+1) \
                           +(BankStrLen+1) \
                           +(ManufactureStrLen+1) \
                           +(SerialNumStrLen+1) \
                           +(AssetTagStrLen+1) \
                           +(PartNumStrLen+1)+1;

        SmbiosRecord = AllocatePool(TotalSize);
        ZeroMem(SmbiosRecord, TotalSize);
        CopyMem(SmbiosRecord, RecordData, sizeof(SMBIOS_TABLE_TYPE17));

        Status = Type17ProcessorTableInitialize(NodeId, McId, DimmId, SmbiosRecord, &MemInfoHobData);

        OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
        UnicodeStrToAsciiStr(DimmStr, OptionalStrStart);/*1*/
        UnicodeStrToAsciiStr(BankStr, OptionalStrStart + DimmStrLen+1);/*2*/
        UnicodeStrToAsciiStr(Manufacturer, OptionalStrStart + DimmStrLen+1 + BankStrLen+1);/*3*/
        UnicodeStrToAsciiStr(SerialNum, OptionalStrStart + DimmStrLen+1 + BankStrLen+1 + ManufactureStrLen + 1);/*4*/
        UnicodeStrToAsciiStr(AssetTag, OptionalStrStart + DimmStrLen+1 + BankStrLen+1+ ManufactureStrLen + 1 + SerialNumStrLen + 1);/*5*/
        AsciiStrnCpy(OptionalStrStart + DimmStrLen+1 + BankStrLen+1+ ManufactureStrLen + 1 + SerialNumStrLen + 1+AssetTagStrLen+1, PartNum, PartNumStrLen);/*6*/
        DbgPrint(DEBUG_INFO,"%a() (type:%d) (size:0x%x)\n",__FUNCTION__,SmbiosRecord->Hdr.Type,TotalSize);
        Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);

        FreePool(SmbiosRecord);
        FreePool(Manufacturer);
        FreePool(SerialNum);
        FreePool(PartNum);
      }
    }
  }

  return Status;
}

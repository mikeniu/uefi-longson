/** @file
0  BIOS vendor information boot time changes.
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
#include <Io.h>
#include <Ppi/Ls3aCpu/Ls3aPlatformTablePpi.h>

//
// Static (possibly build generated) Boot info (type 4).
//
MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE4, MiscProcessorInfo) = {
  {
    EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION,
    sizeof(SMBIOS_TABLE_TYPE4),
    0x400
  }, 0
};

/* Max support four sockets(CPUs) now */
EFI_STATUS
GetSockets (
  CHAR16   **Socket,
  UINTN    *SocketStrLen,
  UINTN    SocketNum
  )
{
  STRING_REF                 TokenToGet;
  CHAR16                     *CpuName;
  UINTN                      CpuStrLen;

  if((!Socket )||(!SocketStrLen))
    return EFI_INVALID_PARAMETER;

  switch(SocketNum)
  {
    case 0:
      TokenToGet = STRING_TOKEN (STR_MISC_PROCESSOR_SOCKET0);
      CpuName = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
      CpuStrLen = StrLen(CpuName);
      break;

      case 1:
      TokenToGet = STRING_TOKEN (STR_MISC_PROCESSOR_SOCKET1);
      CpuName = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
      CpuStrLen = StrLen(CpuName);
      break;

    case 2:
      TokenToGet = STRING_TOKEN (STR_MISC_PROCESSOR_SOCKET2);
      CpuName = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
      CpuStrLen = StrLen(CpuName);
      break;

    case 3:
      TokenToGet = STRING_TOKEN (STR_MISC_PROCESSOR_SOCKET3);
      CpuName = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
      CpuStrLen = StrLen(CpuName);
      break;

    default:
      return EFI_INVALID_PARAMETER;
  }

  if (CpuStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  *Socket = CpuName;
  *SocketStrLen = CpuStrLen;

  return EFI_SUCCESS;
}

EFI_STATUS
GetProcessorVersion (
  CHAR16    **ProVer,
  UINTN     *ProVerStrLen
  )
{
  STRING_REF                 TokenToGet;
  CHAR16                     *PvStr = NULL;
  UINTN                      PvStrLen = 0;

  CHAR16* ChipStr;
  ChipStr = AllocatePool(sizeof (CHAR16)*8);
  ZeroMem(ChipStr,sizeof(CHAR16)*8);
  AsciiStrToUnicodeStr((CHAR8 *)(&(Read64(LOONGSON_CPU_NAME_REG_ADDR))),ChipStr);

  if((!ProVer)||(!ProVerStrLen))
    return EFI_INVALID_PARAMETER;

  if (!StrCmp(ChipStr,L"3A5000")) {
#if (TOT_NODE_NUM == 2)
      TokenToGet = STRING_TOKEN (STR_MISC_PROCESSOR_3B5000_VERSION);
#else
      TokenToGet = STRING_TOKEN (STR_MISC_PROCESSOR_3A5000_VERSION);
#endif
      PvStr = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
      PvStrLen = StrLen(PvStr);
  } else {
    DbgPrint(DEBUG_INFO,"Error:Did not match the chip!!!\n");
    ASSERT(FALSE);
  }

  if (PvStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  *ProVer = PvStr;
  *ProVerStrLen = PvStrLen;

  return EFI_SUCCESS;
}

EFI_STATUS
Type4ProcessorTableInitialize (
  SMBIOS_TABLE_TYPE4      *SmbiosRecord,
  LS_CPU_INFO_HOB          *CpuInfoHobData
  )
{
  UINTN                Prid;
  CHAR16*              ChipStr;

  ChipStr = AllocatePool(sizeof (CHAR16)*8);
  ZeroMem(ChipStr,sizeof(CHAR16)*8);
  AsciiStrToUnicodeStr((CHAR8 *)(&(Read64(LOONGSON_CPU_NAME_REG_ADDR))),ChipStr);

  if(!SmbiosRecord)
    return EFI_INVALID_PARAMETER;

  Prid = CpuInfoHobData->CpuInfo.Prid;
  SmbiosRecord->CurrentSpeed = CpuInfoHobData->CpuInfo.Freq;/*MHz*/

  SmbiosRecord->Socket = 1;                                                      /* SMBIOS_TABLE_STRING       Socket */
  SmbiosRecord->ProcessorType = CentralProcessor;                                /* Central Processor */
  SmbiosRecord->ProcessorFamily = ProcessorFamilyUnknown;                        /* LoongArch Family */
  SmbiosRecord->ProcessorManufacture = 2;                                        /* SMBIOS_TABLE_STRING       ProcessorManufacture */
  SmbiosRecord->ProcessorVersion= 3;                                             /* SMBIOS_TABLE_STRING       ProcessorVersion */
  SmbiosRecord->Status = 0x41;                                                   /* CPU Socket Populated & CPU Enabled */
  SmbiosRecord->ProcessorUpgrade = ProcessorUpgradeNone;                         /* None */
  SmbiosRecord->L1CacheHandle = 0xFFFF;                                          /* Not provided */
  SmbiosRecord->L2CacheHandle = 0xFFFF;                                          /* Not provided */
  SmbiosRecord->L3CacheHandle = 0xFFFF;                                          /* Not provided */
  SmbiosRecord->SerialNumber = 4;                                                /* SMBIOS_TABLE_STRING       SerialNumber */
  SmbiosRecord->AssetTag = 5;                                                    /* SMBIOS_TABLE_STRING       AssetTag */
  SmbiosRecord->PartNumber = 6;                                                  /* SMBIOS_TABLE_STRING       PartNumber */
  SmbiosRecord->CoreCount = 4;
  SmbiosRecord->ThreadCount = 4;
  SmbiosRecord->EnabledCoreCount = 4;
  SmbiosRecord->ProcessorCharacteristics = 0x1C;                                 /* 64-bit Capable & Mulit-core & Hardware Thread*/
  SmbiosRecord->ProcessorId.Signature.ProcessorSteppingId = (Prid >> 8) & 0xF;   /* 7.5.3.1 Processor ID */
  SmbiosRecord->ProcessorId.Signature.ProcessorModel      = (Prid >> 12) & 0xF;  /* 7.5.3.1 Processor ID */
  SmbiosRecord->ProcessorId.Signature.ProcessorFamily     = Prid & 0xF;          /* 7.5.3.1 Processor ID */
  SmbiosRecord->ProcessorId.Signature.ProcessorType       = (Prid >> 4) & 0xF;   /* 7.5.3.1 Processor ID */

  if (!StrCmp(ChipStr,L"3A5000")) {
      SmbiosRecord->MaxSpeed = 2000;
      SmbiosRecord->ExternalClock = 25;
      /* 7.5.4 Set processor voltage as 1.2V, use legacy mode */
      SmbiosRecord->Voltage.ProcessorVoltageCapability5V        = 0;
      SmbiosRecord->Voltage.ProcessorVoltageCapability3_3V      = 0;
      SmbiosRecord->Voltage.ProcessorVoltageCapability2_9V      = 1;
      SmbiosRecord->Voltage.ProcessorVoltageCapabilityReserved  = 1;
      SmbiosRecord->Voltage.ProcessorVoltageReserved            = 0;
      SmbiosRecord->Voltage.ProcessorVoltageIndicateLegacy      = 1;
  } else {
    DbgPrint(DEBUG_INFO,"Error:Did not match the chip!!!\n");
    ASSERT(FALSE);
  }

  return EFI_SUCCESS;
}

/**
  This function makes boot time changes to the contents of the
  MiscBiosVendor (Type 4).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
MISC_SMBIOS_TABLE_FUNCTION (
  MiscProcessorInfo
  )
{

  SMBIOS_TABLE_TYPE4        *SmbiosRecord = NULL;
  EFI_STATUS                 Status = EFI_SUCCESS;
  EFI_SMBIOS_HANDLE          SmbiosHandle;
  LS_CPU_INFO_HOB            CpuInfoHobData;
  CHAR8                      *OptionalStrStart ;
  CHAR16                     *Socket;
  CHAR16                     *Manufacture ;
  CHAR16                     *ProcessorVersion ;
  CHAR16                     *SerialNum ;
  CHAR16                     *AsetTag ;
  CHAR16                     *PartNum ;
  UINTN                      SocketStrLen;
  UINTN                      ManufactureStrLen;
  UINTN                      ProcessorVersionStrLen;
  UINTN                      SerialNumStrLen;
  UINTN                      PartNumStrLen;
  UINTN                      AssetTagStrLen;
  UINTN                      TotalSize;
  UINT64                     CpuNum, Loop;
  STRING_REF                 TokenToGet;

  if (RecordData == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  TokenToGet = STRING_TOKEN (STR_MISC_PROCESSOR_MANUFACTURE);
  Manufacture = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  ManufactureStrLen = StrLen(Manufacture);
  if (ManufactureStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_PROCESSOR_SERIALNUMBER);
  SerialNum = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  SerialNumStrLen = StrLen(SerialNum);
  if (SerialNumStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_PROCESSOR_ASSETTAG);
  AsetTag = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  AssetTagStrLen = StrLen(AsetTag);
  if (AssetTagStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_PROCESSOR_PARTNUMBER);
  PartNum = HiiGetPackageString(&gEfiCallerIdGuid, TokenToGet, NULL);
  PartNumStrLen = StrLen(PartNum);
  if (PartNumStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  Status = GetCpuInfoHob (&CpuInfoHobData);
  ASSERT_EFI_ERROR (Status);

  CpuNum = CpuInfoHobData.CpuInfo.CpuNum;

  Status = GetProcessorVersion(&ProcessorVersion, &ProcessorVersionStrLen);
  ASSERT_EFI_ERROR (Status);

  for(Loop = 0; Loop < CpuNum; Loop++) {
    Status = GetSockets((CHAR16 **)&Socket, &SocketStrLen, Loop);
    ASSERT_EFI_ERROR (Status);

    TotalSize = sizeof(SMBIOS_TABLE_TYPE4) + (SocketStrLen + 1) + (ManufactureStrLen + 1)
                      + (ProcessorVersionStrLen + 1) + (SerialNumStrLen + 1) + (AssetTagStrLen + 1)
                      + (PartNumStrLen+1)+1;

    SmbiosRecord = AllocateZeroPool(TotalSize);
    CopyMem(SmbiosRecord, RecordData, sizeof(SMBIOS_TABLE_TYPE4));

    Status = Type4ProcessorTableInitialize(SmbiosRecord, &CpuInfoHobData);
    ASSERT_EFI_ERROR (Status);

    OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
    UnicodeStrToAsciiStr(Socket,            OptionalStrStart);
    UnicodeStrToAsciiStr(Manufacture,       OptionalStrStart + SocketStrLen + 1);
    UnicodeStrToAsciiStr(ProcessorVersion,  OptionalStrStart + SocketStrLen + 1 + ManufactureStrLen + 1);
    UnicodeStrToAsciiStr(SerialNum,         OptionalStrStart + SocketStrLen + 1 + ManufactureStrLen + 1 + ProcessorVersionStrLen + 1);
    UnicodeStrToAsciiStr(AsetTag,           OptionalStrStart + SocketStrLen + 1 + ManufactureStrLen + 1 + ProcessorVersionStrLen + 1 + SerialNumStrLen + 1);
    UnicodeStrToAsciiStr(PartNum,           OptionalStrStart + SocketStrLen + 1 + ManufactureStrLen + 1 + ProcessorVersionStrLen + 1 + SerialNumStrLen + 1 + AssetTagStrLen + 1);

    DbgPrint(DEBUG_INFO,"%a() (type:%d) (size:0x%x)\n",__FUNCTION__,SmbiosRecord->Hdr.Type,TotalSize);

    Status = AddSmbiosRecord (Smbios, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);
    FreePool(SmbiosRecord);
  }

  return Status;
}

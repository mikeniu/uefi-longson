/** @file
  This file contains Srat Talbe initialized work.

  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
  All intellectual property rights(Copyright, Patent and Trademark) reserved.

  Any violations of copyright or other intellectual property rights of the Loongson
  Technology Corporation Limited will be held accountable in accordance with the law,
  if you (or any of your subsidiaries, corporate affiliates or agents) initiate directly
  or indirectly any Intellectual Property Assertion or Intellectual Property Litigation:
  (i) against Loongson Technology Corporation Limited or any of its subsidiaries or corporate affiliates,
  (ii) against any party if such Intellectual Property Assertion or Intellectual Property Litigation arises
  in whole or in part from any software, technology, product or service of Loongson Technology Corporation Limited
  or any of its subsidiaries or corporate affiliates, or (iii) against any party relating to the Software.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION).

**/

#include "AcpiPlatform.h"
#include <LsHobTypes.h>
#include <Library/HobLib.h>

EFI_STATUS
GetMemInfoHob (
  IN OUT LS_MEM_INFO_HOB   *MemInfoData
  )
{
  EFI_HOB_GUID_TYPE    *MemInfoHob;
  LS_MEM_INFO_HOB      *MemInfoHobData;

  ASSERT(MemInfoData != NULL);

  MemInfoHob = GetFirstGuidHob (&gLsMemInfoHobGuid);

  MemInfoHobData = (LS_MEM_INFO_HOB *)(MemInfoHob + 1);

  CopyMem (MemInfoData, MemInfoHobData, sizeof(LS_MEM_INFO_HOB));

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
InstallAcpiSratTable (
  IN   EFI_ACPI_TABLE_PROTOCOL       *AcpiProtocol
  )
{
  EFI_STATUS                                                 Status;
  UINT8                                                      TempNode;
  UINT8                                                      TempCore;
  UINT8                                                      TotalNodeCount;
  UINT8                                                      MemValidNode;
  UINTN                                                      NodeMemSize;
  UINTN                                                      SratTableSize;
  UINTN                                                      TableHandle;
  EFI_ACPI_3_0_SYSTEM_RESOURCE_AFFINITY_TABLE_HEADER         *Srat;
  EFI_ACPI_3_0_PROCESSOR_LOCAL_APIC_SAPIC_AFFINITY_STRUCTURE *ProcStruct;
  EFI_ACPI_3_0_MEMORY_AFFINITY_STRUCTURE                     *MemStruct;
  LS_MEM_INFO_HOB                                            MemInfoHobData;

  GetMemInfoHob (&MemInfoHobData);

  TableHandle    = 0;
  TotalNodeCount = TOT_NODE_NUM;
  MemValidNode   = 0;

  for(TempNode = 0; TempNode < TotalNodeCount; TempNode++){
    if(MemInfoHobData.MemInfo.NodeMemorySize[TempNode] != 0)
      MemValidNode++;
  }
  ASSERT(MemValidNode != 0);

  SratTableSize = sizeof(*Srat) + TotalNodeCount * CORE_PER_NODE * sizeof(*ProcStruct) +
                    (MemValidNode + 1) * sizeof(*MemStruct); //Node0 including two structure

  Srat = AllocatePool (SratTableSize);
  if (Srat == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Srat->Header.Signature = EFI_ACPI_3_0_SYSTEM_RESOURCE_AFFINITY_TABLE_SIGNATURE;
  Srat->Header.Length = (UINT32)SratTableSize;
  Srat->Header.Revision = EFI_ACPI_3_0_SYSTEM_RESOURCE_AFFINITY_TABLE_REVISION;
  Srat->Header.Checksum = 0;
  Srat->Header.OemId[0] = 'L';
  Srat->Header.OemId[1] = 'O';
  Srat->Header.OemId[2] = 'O';
  Srat->Header.OemId[3] = 'N';
  Srat->Header.OemId[4] = 'G';
  Srat->Header.OemId[5] = 'S';
  Srat->Header.OemTableId = EFI_ACPI_OEM_TABLE_ID;
  Srat->Header.OemRevision = EFI_ACPI_OEM_REVISION;
  Srat->Header.CreatorId = EFI_ACPI_CREATOR_ID;
  Srat->Header.CreatorRevision = EFI_ACPI_CREATOR_REVISION;
  Srat->Reserved1 = 0x00000001;
  Srat->Reserved2 = RESERVED64;

  ProcStruct = (EFI_ACPI_3_0_PROCESSOR_LOCAL_APIC_SAPIC_AFFINITY_STRUCTURE *)(Srat + 1);
  for(TempNode = 0; TempNode < TotalNodeCount; TempNode++){
    for(TempCore = 0; TempCore < CORE_PER_NODE; TempCore++){
      ProcStruct->Type = PROC_TYPE;
      ProcStruct->Length = PROC_LENGTH;
      ProcStruct->ProximityDomain7To0 = TempNode;
      ProcStruct->ApicId = TempNode * 4 + TempCore;
      ProcStruct->Flags = PROC_FLAGS;
      ProcStruct->LocalSapicEid = PROC_SAPIC_EID;
      ProcStruct->ProximityDomain31To8[0] = PROC_PROX_DOMAIN_31TO8;
      ProcStruct->ProximityDomain31To8[1] = PROC_PROX_DOMAIN_31TO8;
      ProcStruct->ProximityDomain31To8[2] = PROC_PROX_DOMAIN_31TO8;
      ProcStruct->Reserved[0] = RESERVED8;
      ProcStruct->Reserved[1] = RESERVED8;
      ProcStruct->Reserved[2] = RESERVED8;
      ProcStruct->Reserved[3] = RESERVED8;
      ProcStruct++;
    }
  }

  NodeMemSize = MemInfoHobData.MemInfo.NodeMemorySize[0];
  ASSERT(NodeMemSize != 0);

  MemStruct = (EFI_ACPI_3_0_MEMORY_AFFINITY_STRUCTURE *)ProcStruct;
  MemStruct->Type = MEM_TYPE;
  MemStruct->Length = MEM_LENGTH;
  MemStruct->ProximityDomain = 0;
  MemStruct->Reserved1 = RESERVED16;
  MemStruct->AddressBaseLow = 0x00000000;
  MemStruct->AddressBaseHigh = 0x00000000;
  MemStruct->LengthLow = 0x10000000;
  MemStruct->LengthHigh = 0x00000000;
  MemStruct->Reserved2 = RESERVED32;
  MemStruct->Flags = MEM_FLAGS;
  MemStruct->Reserved3 = RESERVED64;
  MemStruct++;

  MemStruct->Type = MEM_TYPE;
  MemStruct->Length = MEM_LENGTH;
  MemStruct->ProximityDomain = 0;
  MemStruct->Reserved1 = RESERVED16;
  MemStruct->AddressBaseLow = 0x90000000;
  MemStruct->AddressBaseHigh = 0x00000000;
  MemStruct->LengthLow = (NodeMemSize - 0x10000000) & 0xffffffff;
  MemStruct->LengthHigh = ((NodeMemSize - 0x10000000) >> 32) & 0xffffffff;
  MemStruct->Reserved2 = RESERVED32;
  MemStruct->Flags = MEM_FLAGS;
  MemStruct->Reserved3 = RESERVED64;
  MemStruct++;

  if(TotalNodeCount > 1){
    for(TempNode = 1; TempNode < TotalNodeCount; TempNode++){
      NodeMemSize = MemInfoHobData.MemInfo.NodeMemorySize[TempNode];
      if(NodeMemSize != 0){
        MemStruct->Type = MEM_TYPE;
        MemStruct->Length = MEM_LENGTH;
        MemStruct->ProximityDomain = TempNode;
        MemStruct->Reserved1 = RESERVED16;
        MemStruct->AddressBaseLow = 0x80000000;
        MemStruct->AddressBaseHigh = 0x00000000 | (TempNode << 12);
        MemStruct->LengthLow = NodeMemSize & 0xffffffff;
        MemStruct->LengthHigh = (NodeMemSize >> 32) & 0xffffffff;
        MemStruct->Reserved2 = RESERVED32;
        MemStruct->Flags = MEM_FLAGS;
        MemStruct->Reserved3 = RESERVED64;
        MemStruct++;
      }
    }
  }

  AcpiPlatformChecksum ((UINT8*)Srat, Srat->Header.Length);
  Status = AcpiProtocol->InstallAcpiTable (AcpiProtocol, Srat, SratTableSize, &TableHandle);

  FreePool (Srat);

  return Status;
}


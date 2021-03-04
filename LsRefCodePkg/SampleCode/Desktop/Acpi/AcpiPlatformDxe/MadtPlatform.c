/** @file
  This file contains Madt Talbe initialized work.

  Copyright (c) 2013-2015 Intel Corporation.
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

//
// Statements that include other files
//

#include "AcpiPlatform.h"

//
// Init Multiple APIC Description Table
//
EFI_STATUS
InstallMadtTable (
  EFI_ACPI_TABLE_PROTOCOL *AcpiProtocol
  )
{
  EFI_ACPI_2_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER   *Madt;
  EFI_ACPI_2_0_COREINTC_STRUCTURE                       *CoreIntc;
  EFI_ACPI_2_0_LIOINTC_STRUCTURE                        *LioIntc;
  EFI_ACPI_2_0_EIOINTC_STRUCTURE                        *EIOIntc;
  EFI_ACPI_2_0_PCHINTC_STRUCTURE                        *PCHIntc;
  EFI_ACPI_2_0_MSINTC_STRUCTURE                         *MSIntc;
  EFI_ACPI_2_0_LPCINTC_STRUCTURE                        *LPCIntc;



  EFI_MP_SERVICES_PROTOCOL                            *MpService;
  EFI_PROCESSOR_INFORMATION                           ProcessorInfoBuffer;
  UINTN                                               NumberOfCPUs;
  UINTN                                               NumberOfEnabledCPUs;
  UINTN                                               TableHandle;

  UINT32      MadtSize;
  UINTN       Index;
  EFI_STATUS  Status;

  TableHandle = 0;

  //
  // Find the MP Protocol. This is an MP platform, so MP protocol must be
  // there.
  //
  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  (VOID **)&MpService
                  );
  ASSERT_EFI_ERROR (Status);
  //
  // Determine the number of processors
  //
  MpService->GetNumberOfProcessors (
              MpService,
              &NumberOfCPUs,
              &NumberOfEnabledCPUs
              );
  DEBUG ((EFI_D_INFO, "%a: Find %d processors in system.\n", __func__, NumberOfCPUs));
  ASSERT (NumberOfCPUs <= 64 && NumberOfCPUs > 0);

  MadtSize = 1                     * sizeof (*Madt) +
             NumberOfCPUs          * sizeof (*CoreIntc) +
             1                     * sizeof (*LioIntc) +
			 1                     * sizeof (*EIOIntc) +
             1                     * sizeof (*PCHIntc) +
             1                     * sizeof (*MSIntc) +
             1                     * sizeof (*LPCIntc);

  Madt = (EFI_ACPI_2_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER *)AllocateZeroPool (MadtSize);
  ASSERT (Madt != NULL);
  //
  // Initialize MADT Header information
  //
  Madt->Header.Signature    = EFI_ACPI_2_0_MULTIPLE_SAPIC_DESCRIPTION_TABLE_SIGNATURE;
  Madt->Header.Length       = MadtSize;
  Madt->Header.Revision     = EFI_ACPI_2_0_MULTIPLE_APIC_DESCRIPTION_TABLE_REVISION;
  Madt->Header.OemTableId   = EFI_ACPI_OEM_TABLE_ID;
  Madt->Header.OemRevision  = EFI_ACPI_OEM_MADT_REVISION;
  Madt->Header.CreatorId    = EFI_ACPI_CREATOR_ID;
  Madt->LocalApicAddress    = PcdGet32 (PcdCpuLocalApicAddress);
  Madt->Flags               = EFI_ACPI_2_0_MULTIPLE_APIC_FLAGS;
  CopyMem (Madt->Header.OemId, EFI_ACPI_OEM_ID, 6);

  CoreIntc = (EFI_ACPI_2_0_COREINTC_STRUCTURE *) (Madt + 1);

  //
  // Initialization of CoreIntc
  //
  for (Index = 0;Index < NumberOfCPUs; Index++) {
    Status = MpService->GetProcessorInfo (
                          MpService,
                          Index,
                          &ProcessorInfoBuffer
                          );
    if (!EFI_ERROR (Status)) {
      CoreIntc[Index].Type             = EFI_ACPI_2_0_COREINTC;
      CoreIntc[Index].Length           = sizeof (EFI_ACPI_2_0_COREINTC_STRUCTURE);
      CoreIntc[Index].ProcessorId      = (UINT8)(Index + 1);
      CoreIntc[Index].CoreId           = ProcessorInfoBuffer.ProcessorId;
      DEBUG ((EFI_D_INFO, "lapic[%d].AcpiId = 0x%x==\n", Index, ProcessorInfoBuffer.ProcessorId));
      if (ProcessorInfoBuffer.StatusFlag & PROCESSOR_ENABLED_BIT) {
        CoreIntc[Index].Flags          = 1;
      } else {
        CoreIntc[Index].Flags          = 0;
      }
      CoreIntc[Index].Version          = ACPI_MADT_COREINTC_VERSION_V1;
    }
  }

  //
  // Initialization of LioIntc.
  //
  LioIntc = (EFI_ACPI_2_0_LIOINTC_STRUCTURE *) (&CoreIntc[Index]);
  LioIntc->Type          = EFI_ACPI_2_0_LIOINTC;
  LioIntc->Length        = sizeof (EFI_ACPI_2_0_LIOINTC_STRUCTURE);
  LioIntc->Address       = PcdGet32 (PcdCpuLocalApicAddress);
  LioIntc->Size          = 0x80;
  LioIntc->Cascade[0]    = 0x02;
  LioIntc->Cascade[1]    = 0x03;
  LioIntc->CascadeMap[0] = 0x00FFFFFE;
  LioIntc->CascadeMap[1] = 0xFF000000;
  LioIntc->Version       = ACPI_MADT_LIOINTC_VERSION_V1;
  LioIntc ++;
  
  //
  // Initialization of Extend I/O Interrupt Controller.
  //
  EIOIntc = (EFI_ACPI_2_0_EIOINTC_STRUCTURE *) LioIntc;
  EIOIntc->Type          = EFI_ACPI_2_0_EIOINTC;
  EIOIntc->Length        = sizeof (EFI_ACPI_2_0_EIOINTC_STRUCTURE);
  EIOIntc->Cascade       = 0x03;
  EIOIntc->Version       = ACPI_MADT_EIOINTC_VERSION_V1;
  EIOIntc++;

  //
  // Initialization of PCHIntc.
  //
  PCHIntc = (EFI_ACPI_2_0_PCHINTC_STRUCTURE *) EIOIntc;
  PCHIntc->Type          = EFI_ACPI_2_0_PCHINTC;
  PCHIntc->Length        = sizeof (EFI_ACPI_2_0_PCHINTC_STRUCTURE);
  PCHIntc->Address       = 0x0000000010000000;
  PCHIntc->Size          = 0x400;
  PCHIntc->Id            = 0;
  PCHIntc->GsiBase       = 0x40;
  PCHIntc->Version       = ACPI_MADT_PCHINTC_VERSION_V1;
  PCHIntc ++;

  //
  // Initialization of MSIntc.
  //
  MSIntc = (EFI_ACPI_2_0_MSINTC_STRUCTURE *) PCHIntc;
  MSIntc->Type          = EFI_ACPI_2_0_MSINTC;
  MSIntc->Length        = sizeof (EFI_ACPI_2_0_MSINTC_STRUCTURE);
  MSIntc->MsgAddress    = 0x000000002FF00000;
  MSIntc->Start         = 0x40;
  MSIntc->Count         = 0xC0;
  MSIntc->Version       = ACPI_MADT_MSINTC_VERSION_V1;
  MSIntc ++;

  //
  // Initialization of LPCIntc.
  //
  LPCIntc = (EFI_ACPI_2_0_LPCINTC_STRUCTURE *) MSIntc;
  LPCIntc->Type          = EFI_ACPI_2_0_LPCINTC;
  LPCIntc->Length        = sizeof (EFI_ACPI_2_0_LPCINTC_STRUCTURE);
  LPCIntc->Address       = 0x0000000010002000;
  LPCIntc->Size          = 0x1000;
  LPCIntc->Cascade       = 0x13;
  LPCIntc->Version       = ACPI_MADT_LPCINTC_VERSION_V1;
  LPCIntc ++;
  
  Madt->Header.Checksum = 0;
  AcpiPlatformChecksum ((UINT8*)Madt, Madt->Header.Length);

  Status = AcpiProtocol->InstallAcpiTable (AcpiProtocol, Madt, MadtSize, &TableHandle);

  FreePool (Madt);

  return Status;
}

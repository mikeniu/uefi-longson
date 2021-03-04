/*
## @file
#
#  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
#  All intellectual property rights(Copyright, Patent and Trademark) reserved.
#
#  Any violations of copyright or other intellectual property rights of the Loongson Technology
#  Corporation Limited will be held accountable in accordance with the law,
#  if you (or any of your subsidiaries, corporate affiliates or agents) initiate
#  directly or indirectly any Intellectual Property Assertion or Intellectual Property Litigation:
#  (i) against Loongson Technology Corporation Limited or any of its subsidiaries or corporate affiliates,
#  (ii) against any party if such Intellectual Property Assertion or Intellectual Property Litigation arises
#  in whole or in part from any software, technology, product or service of Loongson Technology Corporation
#  Limited or any of its subsidiaries or corporate affiliates, or (iii) against any party relating to the Software.
#
#  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR
#  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION).
#
#
##
*/

#include <Ppi/HdaVerbTablePpi.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include "HdaVerbTables.h"

extern UINT32 *gHdaVerbTable;
extern UINT64 gHdaVerbTableSize;

/**
 Entry point function for the PEIM, install loongson platform 7a device ppi.

 @param FileHandle      Handle of the file being invoked.
 @param PeiServices     Describes the list of possible PEI Services.

 @return EFI_SUCCESS    If we installed our PPI successfully.

**/
EFI_STATUS
EFIAPI
HdaVerbTableInitEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                  Status;
  HDA_VERB_TABLE_PPI          *HdaVerbTablePpi;
  EFI_PEI_PPI_DESCRIPTOR      *HdaVerbTablePpiDesc;
  UINT32                      *HdaVerbTable;

  //
  // Allocate descriptor and PPI structures
  //
  HdaVerbTablePpiDesc = (EFI_PEI_PPI_DESCRIPTOR *)AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  ASSERT (HdaVerbTablePpiDesc != NULL);
  if (HdaVerbTablePpiDesc == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  HdaVerbTablePpi = (HDA_VERB_TABLE_PPI *)AllocateZeroPool (sizeof (HDA_VERB_TABLE_PPI));
  ASSERT (HdaVerbTablePpi != NULL);
  if (HdaVerbTablePpi == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  HdaVerbTable = (UINT32 *)AllocateZeroPool (gHdaVerbTableSize * sizeof (UINT32));
  ASSERT (HdaVerbTable != NULL);
  if (HdaVerbTable == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (HdaVerbTable, gHdaVerbTable, gHdaVerbTableSize * sizeof (UINT32));

  HdaVerbTablePpi->HdaVerbTable = HdaVerbTable;
  HdaVerbTablePpi->Size         = gHdaVerbTableSize;

  //
  // Initialize the PPI
  //
  HdaVerbTablePpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  HdaVerbTablePpiDesc->Guid  = &gEfiHdaVerbTablePpiGuid;
  HdaVerbTablePpiDesc->Ppi   = HdaVerbTablePpi;

  // Install HDA_VERB_TABLE_PPI
  //
  Status = PeiServicesInstallPpi (HdaVerbTablePpiDesc);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

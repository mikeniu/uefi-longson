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

#include <Ppi/Ls3aCpu/Ls3aPlatformTablePpi.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/Ls7aRegDef.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/BaseMemoryLib.h>

void mm_feature_init(OUT DDR_CTRL *mm_ctrl);

/**
 Entry point function for the PEIM, install loongson platform 7a device ppi.

 @param FileHandle      Handle of the file being invoked.
 @param PeiServices     Describes the list of possible PEI Services.

 @return EFI_SUCCESS    If we installed our PPI successfully.

**/
EFI_STATUS
EFIAPI
PeiLs3aPlatformTableInitEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                       Status;
  LS3A_PLATFORM_TABLE_PPI          *Ls3aPtPpi;
  EFI_PEI_PPI_DESCRIPTOR           *Ls3aPtPpiDesc;
  DVFS_CFG_TABLE                   *DvfsCfg;
  CPU_INFO_TABLE                   *CpuInfo;
  MEM_INFO_TABLE                   *MemInfo;
  DDR_CTRL                         *DdrCtrl;
  DEVICE_MANAGER_CONFIGURATION     *Configuration;
  UINTN                            BufferSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *PeiGetVariableName = NULL;
  CHAR16                           VariableName[] = L"DevMgrData";
  //
  // Allocate descriptor and PPI structures
  //
  Ls3aPtPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  ASSERT (Ls3aPtPpiDesc != NULL);
  if (Ls3aPtPpiDesc == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Ls3aPtPpi = (LS3A_PLATFORM_TABLE_PPI *) AllocateZeroPool (sizeof (LS3A_PLATFORM_TABLE_PPI));
  ASSERT (Ls3aPtPpi != NULL);
  if (Ls3aPtPpi == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  DvfsCfg = (DVFS_CFG_TABLE *) AllocateZeroPool (sizeof (DVFS_CFG_TABLE));
  ASSERT (DvfsCfg != NULL);
  if (DvfsCfg == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CpuInfo = (CPU_INFO_TABLE *) AllocateZeroPool (sizeof (CPU_INFO_TABLE));
  ASSERT (CpuInfo != NULL);
  if (CpuInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  MemInfo = (MEM_INFO_TABLE *) AllocateZeroPool (sizeof (MEM_INFO_TABLE));
  ASSERT (MemInfo != NULL);
  if (MemInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  DdrCtrl = (DDR_CTRL *) AllocateZeroPool (sizeof (DDR_CTRL));
  ASSERT (DdrCtrl != NULL);
  if (DdrCtrl == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Initialize the PPI
  //
  Ls3aPtPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  Ls3aPtPpiDesc->Guid  = &gEfiLs3aPlatformTablePpiGuid;
  Ls3aPtPpiDesc->Ppi   = Ls3aPtPpi;

  Ls3aPtPpi->DvfsCfg = DvfsCfg;
  Ls3aPtPpi->CpuInfo = CpuInfo;
  Ls3aPtPpi->MemInfo = MemInfo;
  Ls3aPtPpi->DdrCtrl = DdrCtrl;

  //
  // Initialize the Ls3aPt information
  //
  Ls3aPtPpi->Signature         = LS3A_PLATFORM_TABLE_SIGNATURE;
  Ls3aPtPpi->Rev               = 0x00;
  Ls3aPtPpi->VariableValidFlag = FALSE;
  Ls3aPtPpi->NodeNum           = 1;

  //
  //GetVariable of DVFS
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **)&PeiGetVariableName
             );
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  Configuration = AllocateZeroPool(sizeof(DEVICE_MANAGER_CONFIGURATION));

  if (Configuration == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  BufferSize = sizeof (DEVICE_MANAGER_CONFIGURATION);

  Status = PeiGetVariableName->GetVariable (
                                 PeiGetVariableName,
                                 VariableName,
                                 &gDeviceListFormSetGuid,
                                 NULL,
                                 &BufferSize,
                                 Configuration
                                 );

  if (EFI_ERROR(Status)) {
    //
    // Initialize the DVFS Configuration
    //
    DvfsCfg->DvfsEnableFlag = LS3A_DEV_DISABLE;
  } else {
    DvfsCfg->DvfsEnableFlag = Configuration->DVFS;
  }
  //
  // Initialize DDR_CTRL
  //
  mm_feature_init (Ls3aPtPpi->DdrCtrl);
  //
  // Install Ls3aPt PPI
  //
  Status = PeiServicesInstallPpi (Ls3aPtPpiDesc);
  ASSERT_EFI_ERROR (Status);

  FreePool(Configuration);

  return Status;
}

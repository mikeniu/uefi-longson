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

#include <Ppi/Ls7aChipset/Ls7aPlatformTablePpi.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/Ls7aRegDef.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/BaseMemoryLib.h>

/**
 Entry point function for the PEIM, install loongson platform 7a device ppi.

 @param FileHandle      Handle of the file being invoked.
 @param PeiServices     Describes the list of possible PEI Services.

 @return EFI_SUCCESS    If we installed our PPI successfully.

**/
EFI_STATUS
EFIAPI
PeiLs7aPlatformTableInitEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                        Status;
  LS7A_PLATFORM_TABLE_PPI           *Ls7aPtPpi;
  EFI_PEI_PPI_DESCRIPTOR            *Ls7aPtPpiDesc;
  LS7A_PLL_CFG_TABLE                *PllCfg;
  LS7A_HT_CFG_TABLE                 *HtCfg;
  LS7A_PCIE_CFG_TABLE               *PcieCfg;
  LS7A_SATA_CFG_TABLE               *SataCfg;
  LS7A_USB_CFG_TABLE                *UsbCfg;
  LS7A_GMAC_CFG_TABLE               *GmacCfg;
  LS7A_GMEM_CFG_TABLE               *GmemCfg;
  LS7A_WAKEUP_CFG_TABLE             *WakeUpCfg;
  LS7A_SMARTFAN_CFG_TABLE           *SmartFanCfg;
  LS7A_DEVICE_MANAGER_CONFIGURATION      *Configuration;
  UINTN                             BufferSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *PeiGetVariableName = NULL;
  CHAR16                            VariableName[] = L"DevMgrData";

  //
  // Allocate descriptor and PPI structures
  //
  Ls7aPtPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  ASSERT (Ls7aPtPpiDesc != NULL);
  if (Ls7aPtPpiDesc == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Ls7aPtPpi = (LS7A_PLATFORM_TABLE_PPI *) AllocateZeroPool (sizeof (LS7A_PLATFORM_TABLE_PPI));
  ASSERT (Ls7aPtPpi != NULL);
  if (Ls7aPtPpi == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  PllCfg = (LS7A_PLL_CFG_TABLE *) AllocateZeroPool (sizeof (LS7A_PLL_CFG_TABLE));
  ASSERT (PllCfg != NULL);
  if (PllCfg == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  HtCfg = (LS7A_HT_CFG_TABLE *) AllocateZeroPool (sizeof (LS7A_HT_CFG_TABLE));
  ASSERT (HtCfg != NULL);
  if (HtCfg == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  PcieCfg = (LS7A_PCIE_CFG_TABLE *) AllocateZeroPool (sizeof (LS7A_PCIE_CFG_TABLE));
  ASSERT (PcieCfg != NULL);
  if (PcieCfg == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SataCfg = (LS7A_SATA_CFG_TABLE *) AllocateZeroPool (sizeof (LS7A_SATA_CFG_TABLE));
  ASSERT (SataCfg != NULL);
  if (SataCfg == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  UsbCfg = (LS7A_USB_CFG_TABLE *) AllocateZeroPool (sizeof (LS7A_USB_CFG_TABLE));
  ASSERT (UsbCfg != NULL);
  if (UsbCfg == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  GmacCfg = (LS7A_GMAC_CFG_TABLE *) AllocateZeroPool (sizeof (LS7A_GMAC_CFG_TABLE));
  ASSERT (GmacCfg != NULL);
  if (GmacCfg == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  GmemCfg = (LS7A_GMEM_CFG_TABLE *) AllocateZeroPool (sizeof (LS7A_GMEM_CFG_TABLE));
  ASSERT (GmemCfg != NULL);
  if (GmemCfg == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  WakeUpCfg = (LS7A_WAKEUP_CFG_TABLE *) AllocateZeroPool (sizeof (LS7A_WAKEUP_CFG_TABLE));
  ASSERT (WakeUpCfg != NULL);
  if (WakeUpCfg == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SmartFanCfg = (LS7A_SMARTFAN_CFG_TABLE *) AllocateZeroPool (sizeof (LS7A_SMARTFAN_CFG_TABLE));
  ASSERT (SmartFanCfg != NULL);
  if (SmartFanCfg == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Initialize the PPI
  //
  Ls7aPtPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  Ls7aPtPpiDesc->Guid  = &gEfiLs7aPlatformTablePpiGuid;
  Ls7aPtPpiDesc->Ppi   = Ls7aPtPpi;

  Ls7aPtPpi->PllCfg      = PllCfg;
  Ls7aPtPpi->HtCfg       = HtCfg;
  Ls7aPtPpi->PcieCfg     = PcieCfg;
  Ls7aPtPpi->SataCfg     = SataCfg;
  Ls7aPtPpi->UsbCfg      = UsbCfg;
  Ls7aPtPpi->GmacCfg     = GmacCfg;
  Ls7aPtPpi->GmemCfg     = GmemCfg;
  Ls7aPtPpi->WakeUpCfg   = WakeUpCfg;
  Ls7aPtPpi->SmartFanCfg = SmartFanCfg;

  //
  // Initialize the Ls7aPt information
  //
  Ls7aPtPpi->Signature         = LS7A_PLATFORM_TABLE_SIGNATURE;
  Ls7aPtPpi->Rev               = 0x00;
  Ls7aPtPpi->VariableValidFlag = FALSE;
  //
  // Initialize the PLL Configuration
  //
  PllCfg->Pll0Val.PllValue = LS7A_PLL_VALUE(80, 8, 16, 12);
  PllCfg->Pll0Val.DivRef   = 0x4;

  PllCfg->Pll1Val.PllValue = LS7A_PLL_VALUE(127, 8, 6, 12);
  PllCfg->Pll1Val.DivRef   = 0x4;

  PllCfg->Pll2Val.PllValue = LS7A_PLL_VALUE(96, 72, 6, 100);
  PllCfg->Pll2Val.DivRef   = 0x4;

  PllCfg->Pll3Val.PllValue = LS7A_PLL_VALUE(104, 68, 68, 68);
  PllCfg->Pll3Val.DivRef   = 0x4;

  PllCfg->Pll4Val.PllValue = LS7A_PLL_VALUE(104, 68, 68, 68);
  PllCfg->Pll4Val.DivRef   = 0x4;
  //
  // Initialize the HT Configuration
  //
  HtCfg->LinkNodeNum = 1;
  //
  // Initialize the PCIE Configuration
  //
  PcieCfg->RefClk        = LS7A_PCIE_INSIDE_REF_CLK;
  PcieCfg->PcieX8CalEnableFlag = LS7A_DEV_ENABLE;

  PcieCfg->F0.DrivingForce.OffSet0 = 0xc2492331;
  PcieCfg->F0.DrivingForce.OffSet4 = 0xff3ff0a8;
  PcieCfg->F0.DrivingForce.OffSet8 = 0x27fff;

  PcieCfg->F1.DrivingForce.OffSet0 = 0xc2492331;
  PcieCfg->F1.DrivingForce.OffSet4 = 0xff3ff0a8;
  PcieCfg->F1.DrivingForce.OffSet8 = 0x27fff;

  PcieCfg->H.DrivingForce.OffSet0  = 0xc2492331;
  PcieCfg->H.DrivingForce.OffSet4  = 0xff3ff0a8;
  PcieCfg->H.DrivingForce.OffSet8  = 0x27fff;

  PcieCfg->G0.DrivingForce.OffSet0 = 0xc2492331;
  PcieCfg->G0.DrivingForce.OffSet4 = 0xff3ff0a8;
  PcieCfg->G0.DrivingForce.OffSet8 = 0x27fff;

  PcieCfg->G1.DrivingForce.OffSet0 = 0xc2492331;
  PcieCfg->G1.DrivingForce.OffSet4 = 0xff3ff0a8;
  PcieCfg->G1.DrivingForce.OffSet8 = 0x27fff;

  PcieCfg->F0.EnableFlag   = LS7A_DEV_ENABLE;
  PcieCfg->F0.Mode         = LS7A_PCIE_ADAPT_MODE;
  PcieCfg->F0.Clock        = LS7A_PCIE_PORT_WORK_AS_DEFAULT;
  PcieCfg->F0.Inversion    = LS7A_PCIE_PORT_INVERSION_OFF;

  PcieCfg->F1.EnableFlag   = LS7A_DEV_ENABLE;
  PcieCfg->F1.Mode         = LS7A_PCIE_ADAPT_MODE;
  PcieCfg->F1.Clock        = LS7A_PCIE_PORT_WORK_AS_DEFAULT;
  PcieCfg->F1.Inversion    = LS7A_PCIE_PORT_INVERSION_OFF;

  PcieCfg->H.EnableFlag    = LS7A_DEV_ENABLE;
  PcieCfg->H.Mode          = LS7A_PCIE_ADAPT_MODE;
  PcieCfg->H.Clock         = LS7A_PCIE_PORT_WORK_AS_DEFAULT;
  PcieCfg->H.Inversion     = LS7A_PCIE_PORT_INVERSION_OFF;

  PcieCfg->G0.EnableFlag   = LS7A_DEV_ENABLE;
  PcieCfg->G0.Mode         = LS7A_PCIE_ADAPT_MODE;
  PcieCfg->G0.Clock        = LS7A_PCIE_PORT_WORK_AS_DEFAULT;
  PcieCfg->G0.Inversion    = LS7A_PCIE_PORT_INVERSION_OFF;

  PcieCfg->G1.EnableFlag   = LS7A_DEV_ENABLE;
  PcieCfg->G1.Mode         = LS7A_PCIE_ADAPT_MODE;
  PcieCfg->G1.Clock        = LS7A_PCIE_PORT_WORK_AS_DEFAULT;
  PcieCfg->G1.Inversion    = LS7A_PCIE_PORT_INVERSION_OFF;

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **)&PeiGetVariableName
             );
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  Configuration = AllocateZeroPool(sizeof(LS7A_DEVICE_MANAGER_CONFIGURATION));
  if(Configuration == NULL){
    return EFI_OUT_OF_RESOURCES;
  }

  BufferSize = sizeof (LS7A_DEVICE_MANAGER_CONFIGURATION);
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
    // Default configuration
    //
    Configuration->SataController0     = TRUE;
    Configuration->SataController1     = TRUE;
    Configuration->SataController2     = TRUE;
    Configuration->UsbController0      = TRUE;
    Configuration->UsbController1      = TRUE;
    Configuration->EthernetController0 = TRUE;
    Configuration->EthernetController1 = TRUE;
    Configuration->WakeOnLine          = TRUE;
    Configuration->PcieWake            = FALSE;
    Configuration->UsbWake             = FALSE;
  } else {
    /* Special Treatment for USB0, SATA0, Gmac0 */
    if (Configuration->SataController0 == FALSE) {
      Configuration->SataController1 = FALSE;
      Configuration->SataController2 = FALSE;
    }
    if (Configuration->UsbController0 == FALSE) {
      Configuration->UsbController1 = FALSE;
    }
    if (Configuration->EthernetController0 == FALSE) {
      Configuration->EthernetController1 = FALSE;
    }
  }

  //
  //Initialize Sata2.0 Configuration
  //
  SataCfg->Sata0.RefClk                = LS7A_SATA_INSIDE_REF_CLK;
  SataCfg->Sata0.EnableFlag            = Configuration->SataController0;
  SataCfg->Sata0.DrivingForce.OvrdFlag = LS7A_DEV_DISABLE;
  SataCfg->Sata0.DrivingForce.Ost8Val  = 0x403f1002;

  SataCfg->Sata1.RefClk                = LS7A_SATA_INSIDE_REF_CLK;
  SataCfg->Sata1.EnableFlag            = Configuration->SataController1;
  SataCfg->Sata1.DrivingForce.OvrdFlag = LS7A_DEV_DISABLE;
  SataCfg->Sata1.DrivingForce.Ost8Val  = 0x403f1002;

  SataCfg->Sata2.RefClk                = LS7A_SATA_INSIDE_REF_CLK;
  SataCfg->Sata2.EnableFlag            = Configuration->SataController2;
  SataCfg->Sata2.DrivingForce.OvrdFlag = LS7A_DEV_DISABLE;
  SataCfg->Sata2.DrivingForce.Ost8Val  = 0x403f1002;

  //
  //Initialize USB2.0 Configuration
  //
  UsbCfg->RefClk              = LS7A_USB_INSIDE_REF_CLK;

  UsbCfg->Usb0.EnableFlag     = Configuration->UsbController0;
  UsbCfg->Usb0.Port0.OvrdFlag = LS7A_DEV_DISABLE;
  UsbCfg->Usb0.Port0.Value    = 0x0;
  UsbCfg->Usb0.Port1.OvrdFlag = LS7A_DEV_DISABLE;
  UsbCfg->Usb0.Port1.Value    = 0x0;
  UsbCfg->Usb0.Port2.OvrdFlag = LS7A_DEV_DISABLE;
  UsbCfg->Usb0.Port2.Value    = 0x0;

  UsbCfg->Usb1.EnableFlag     = Configuration->UsbController1;
  UsbCfg->Usb1.Port0.OvrdFlag = LS7A_DEV_DISABLE;
  UsbCfg->Usb1.Port0.Value    = 0x0;
  UsbCfg->Usb1.Port1.OvrdFlag = LS7A_DEV_DISABLE;
  UsbCfg->Usb1.Port1.Value    = 0x0;
  UsbCfg->Usb1.Port2.OvrdFlag = LS7A_DEV_DISABLE;
  UsbCfg->Usb1.Port2.Value    = 0x0;

  GmacCfg->Gmac0.EnableFlag = Configuration->EthernetController0;
  GmacCfg->Gmac1.EnableFlag = Configuration->EthernetController1;

  WakeUpCfg->WakeGmacEnableFlag = Configuration->WakeOnLine;
  WakeUpCfg->WakePcieEnableFlag = Configuration->PcieWake;
  WakeUpCfg->WakeUsbEnableFlag  = Configuration->UsbWake;

  //
  // Initialize the GMEM Configuration
  //
  GmemCfg->Lpc.EnableFlag      = LS7A_DEV_ENABLE;
  GmemCfg->GraphicsEnableFlag  = LS7A_DEV_ENABLE;
  GmemCfg->GmemCfgEnableFlag   = LS7A_DEV_ENABLE;
  //
  // Initialize the WakeupFunc Configuration
  //
  WakeUpCfg->WakeRtcEnableFlag  = LS7A_DEV_DISABLE;

  //
  // Initialize the SmartFan Configuration
  //
  SmartFanCfg->MinRpm = 5000;
  SmartFanCfg->MaxRpm = 10000;

  //
  // Install Ls7aPt PPI
  //

  Status = PeiServicesInstallPpi (Ls7aPtPpiDesc);
  ASSERT_EFI_ERROR (Status);

  FreePool(Configuration);

  return Status;
}

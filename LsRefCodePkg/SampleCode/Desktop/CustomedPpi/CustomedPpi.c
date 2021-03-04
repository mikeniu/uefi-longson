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

#include "CustomedPpi.h"

/**
 Entry point function for the PEIM, install loongson platform 7a device ppi.

 @param FileHandle      Handle of the file being invoked.
 @param PeiServices     Describes the list of possible PEI Services.

 @return EFI_SUCCESS    If we installed our PPI successfully.

**/
EFI_STATUS
EFIAPI
CustomedCfgInitEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                  Status;
  LS_RC_CORE_PPI              *LsRcCorePpi = NULL;

  Status = PeiServicesLocatePpi (
              &gEfiLsRcCorePpiGuid,
              0,
              NULL,
              (VOID **)&LsRcCorePpi
              );
  ASSERT_EFI_ERROR (Status);

 //Register BeepFunc
  RegBeepFunc(LsRcCorePpi->CustomedCfgPpi);
  DbgPrint (DEBUG_INFO, "Beep Function Register Success.\n");

  //Register WatchDog
  RegWdtFunc(LsRcCorePpi->CustomedCfgPpi);
  DbgPrint (DEBUG_INFO, "WatchDog Function Register Success.\n");

  //Register Dc Phy Init
  RegDcPhyCfgFunc(LsRcCorePpi->CustomedCfgPpi);
  DbgPrint (DEBUG_INFO, "DcPhyInit Function Register Success.\n");

  //Register Volatge Ops
  RegVoltageOps(LsRcCorePpi->CustomedCfgPpi);
  DbgPrint (DEBUG_INFO, "Voltage Function Register Success.\n");

  //Register SmartFan Ops
  RegSmartFanOps(LsRcCorePpi->CustomedCfgPpi);
  DbgPrint (DEBUG_INFO, "SmartFan Function Register Success.\n");

  return Status;
}

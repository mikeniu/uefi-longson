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

#include <Library/PeiServicesLib.h>
#include <Library/PeiLsPtPtrLib.h>
#include <Io.h>
#include <Library/Ls7aRegDef.h>
#include <Ppi/Ls7aChipset/Ls7aPlatformTablePpi.h>
#include <Library/DebugLib.h>
#include <Ppi/RcCorePpi.h>

EFI_STATUS
Ls7aPwm (
  IN LS7A_SMARTFAN_CFG_TABLE *Parameter
  )
{
  UINT32 X, Y;

  X = Parameter->MinRpm;
  Y = Parameter->MaxRpm;

  Readl (LS7A_PWM0_CTRL) &= ~1;
  Writel (LS7A_PWM0_LOW,X);
  Writel (LS7A_PWM0_FULL,Y);
  Readl (LS7A_PWM0_CTRL) |= 1;

  Readl (LS7A_PWM1_CTRL) &= ~1;
  Writel (LS7A_PWM1_LOW,X);
  Writel (LS7A_PWM1_FULL,Y);
  Readl (LS7A_PWM1_CTRL) |= 1;

  //
  // as now, the 7A Fan control circuit(PWM2) has problem, keep it constant to avoid wearing fan.
  //
  /*
    readl (LS7A_PWM2_CTRL) &= ~1;
    outl (LS7A_PWM2_LOW,x);
    outl (LS7A_PWM2_FULL,y);
    readl (LS7A_PWM2_CTRL) |= 1;
  */
  Readl (LS7A_PWM2_LOW) = 0;
  Readl (LS7A_PWM2_FULL) = 100;
  Readl (LS7A_PWM2_CTRL) = 1;

  Readl (LS7A_PWM3_CTRL) &= ~1;
  Writel (LS7A_PWM3_LOW,X);
  Writel (LS7A_PWM3_FULL,Y);
  Readl (LS7A_PWM3_CTRL) |= 1;
  return EFI_SUCCESS;
}

extern LS7A_PLATFORM_TABLE_PPI  *gLs7aPt;
LS7A_SMARTFAN_CFG_TABLE  SmartFanParameter;
EFI_STATUS
EFIAPI
SmartFanInit (
  UINT8 Percent
  )
{
  EFI_STATUS Status;

  //CHECK_LS7APT (gLs7aPt);


  SmartFanParameter.MinRpm = 100 * Percent;
  SmartFanParameter.MaxRpm = 10000;
  DbgPrint (EFI_D_INFO, "SmartFan Init, MinRpm:%d ,MaxRpm;%d \n",\
          SmartFanParameter.MinRpm,SmartFanParameter.MaxRpm);

  Status = Ls7aPwm(&SmartFanParameter);

  return Status;
}

LS_SMARTFAN_OPS  LsSmartFanOps;
EFI_STATUS
RegSmartFanOps (
    LS_CUSTOMER_CFG_PPI   *CustomedCfgPpi
    )
{
  //EFI_STATUS Status;

  LsSmartFanOps.SmartFanInit   =  SmartFanInit;

  CustomedCfgPpi->SmartFanOps  = (LS_SMARTFAN_OPS*)&LsSmartFanOps;

  return EFI_SUCCESS;
}


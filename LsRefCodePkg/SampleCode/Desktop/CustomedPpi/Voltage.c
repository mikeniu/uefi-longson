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
#include <Cpu.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Ppi/RcCorePpi.h>
#include <Library/LsI2cLib.h>
#include <Library/TimerLib.h>

#define CPU_I2C0_REG_BASE  PHYS_TO_UNCACHED(0x1fe00120)
#define I2C_CTL_CLK        100000         //BASE_CLK (Unit Khz)
#define I2C_BUS_RATE       42             //Unit Kbps

EFI_STATUS
CpuVddnVoltageCtrl (
  IN UINT64 NodeNum,
  IN UINT16 Vddn
  )
{
  UINT64 I2cRegBaseAddr, NodeId;
  UINT16 Val16;

  for (NodeId = 0; NodeId < NodeNum; NodeId++) {
    I2cRegBaseAddr = CPU_I2C0_REG_BASE | (NodeId << 44);
    I2cInitSetFreq (I2cRegBaseAddr, I2C_CTL_CLK, I2C_BUS_RATE, ALGORITHM_3A); //Lock Freq

    Val16 = 0x210;
    I2cCtlWrite (I2cRegBaseAddr, MPS_ADDR, 0x5e, 0x2, &Val16);

    Val16 = Vddn;
    I2cCtlWrite (I2cRegBaseAddr, MPS_ADDR, 0x21, 0x2, &Val16);
  }

  MicroSecondDelay (500);

  return EFI_SUCCESS;
}

LS_VOLTAGE_OPS  LsVoltageOps;

EFI_STATUS
RegVoltageOps (
    LS_CUSTOMER_CFG_PPI   *CustomedCfgPpi
    )
{
  //EFI_STATUS Status;

  LsVoltageOps.CpuVddnCtl       =  CpuVddnVoltageCtrl;

  CustomedCfgPpi->VoltageOps  = (LS_VOLTAGE_OPS*)&LsVoltageOps;

  return EFI_SUCCESS;
}


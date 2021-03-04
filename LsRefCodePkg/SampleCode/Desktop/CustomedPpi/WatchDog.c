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

//WatchDog
 /* Loongson3A have 16 GPIOs,
 *  Loongson3A5000 have 32 GPIOs,
 * GPIO5,GPIO4,GPIO3 & GPIO14 are used in driver watchdog chip(MAX6369)
 *
 * GPIO14 should keep high/low to start watchdog counter.
 *
 * GPIO5, GPIO4 & GPIO3 are link to watchdong pin SET2, SET1, SET0.
 * SET2, SET1, SET0 determine watchdog timing characteristics, the timing table as follow:
 *
 * [SET2, SET1, SET0]     watchdog timeout period
 *
 *   [0, 0, 0]			  1 ms
 *   [0, 0, 1]			 10 ms
 *   [0, 1, 0]			 30 ms
 *   [0, 1, 1]			Disable
 *   [1, 0, 0]			100 ms
 *   [1, 0, 1]			   1 s
 *   [1, 1, 0]			  10 s
 *   [1, 1, 1]			  60 s
 */

#include <Cpu.h>
#include <Library/TimerLib.h>
#include <Ppi/RcCorePpi.h>
#include <Library/CpuGpioLib.h>

#define GPIO3                         3
#define GPIO4                         4
#define GPIO5                         5
#define GPIO6                         6
#define GPIO13                        13
#define GPIO14                        14

#define LOONGSON3A_GPIO_BASE          PHYS_TO_UNCACHED(0x1fe00500)

EFI_STATUS
EnableCpuWatchdog (
  VOID
  )
{
#if (TOT_NODE_NUM == 1)
  GpioSetOutLow(LOONGSON3A_GPIO_BASE,GPIO13);
  GpioSetOutHigh(LOONGSON3A_GPIO_BASE,GPIO14);

  /* [SET2, SET1, SET0] ---  [1, 0, 1]  ---  1s */
  GpioSetOutHigh(LOONGSON3A_GPIO_BASE,GPIO5);
  GpioSetOutHigh(LOONGSON3A_GPIO_BASE,GPIO4);
  GpioSetOutLow(LOONGSON3A_GPIO_BASE,GPIO6);

  /* start watchdog counter */
  GpioSetOutLow(LOONGSON3A_GPIO_BASE,GPIO14);
  MicroSecondDelay(0x100);
  GpioSetOutHigh(LOONGSON3A_GPIO_BASE,GPIO13);
#elif (TOT_NODE_NUM == 2)
  GpioSetOutHigh(LOONGSON3A_GPIO_BASE,GPIO6);
  GpioSetOutHigh(LOONGSON3A_GPIO_BASE,GPIO4);
  GpioSetOutLow(LOONGSON3A_GPIO_BASE,GPIO3);
  GpioSetOutLow(LOONGSON3A_GPIO_BASE,GPIO5);
  MicroSecondDelay(0x100);
#elif (TOT_NODE_NUM == 4)
  GpioSetOutLow(LOONGSON3A_GPIO_BASE,GPIO6);
  GpioSetOutHigh(LOONGSON3A_GPIO_BASE,GPIO14);
  GpioSetOutHigh(LOONGSON3A_GPIO_BASE,GPIO5);
  GpioSetOutLow(LOONGSON3A_GPIO_BASE,GPIO4);
  GpioSetOutHigh(LOONGSON3A_GPIO_BASE,GPIO6);

  GpioSetOutLow(LOONGSON3A_GPIO_BASE,GPIO14);
  MicroSecondDelay(0x100);
  GpioSetOutHigh(LOONGSON3A_GPIO_BASE,GPIO6);
#endif
  return EFI_SUCCESS;
}

VOID
DisableCpuWatchdog (
  VOID
  )
{
#if (TOT_NODE_NUM == 1)
/* WatchDog Close for chip MAX6369*/
  GpioSetOutLow (LOONGSON3A_GPIO_BASE,GPIO5);
  GpioSetOutHigh(LOONGSON3A_GPIO_BASE,GPIO4);
  GpioSetOutHigh(LOONGSON3A_GPIO_BASE,GPIO6);
  GpioSetOutLow (LOONGSON3A_GPIO_BASE,GPIO13);
#elif (TOT_NODE_NUM == 2)
  GpioSetOutHigh(LOONGSON3A_GPIO_BASE,GPIO6);
  GpioSetOutHigh(LOONGSON3A_GPIO_BASE,GPIO5);
  GpioSetOutLow(LOONGSON3A_GPIO_BASE,GPIO3);
  GpioSetOutLow(LOONGSON3A_GPIO_BASE,GPIO4);
#elif (TOT_NODE_NUM == 4)
  GpioSetOutLow (LOONGSON3A_GPIO_BASE,GPIO5);
  GpioSetOutHigh(LOONGSON3A_GPIO_BASE,GPIO6);
  GpioSetOutHigh(LOONGSON3A_GPIO_BASE,GPIO4);
  GpioSetOutLow (LOONGSON3A_GPIO_BASE,GPIO6);
#endif
  return EFI_SUCCESS;
}

LS_WDT_OPS  LsWdtOps;
EFI_STATUS
RegWdtFunc (
    LS_CUSTOMER_CFG_PPI   *CustomedCfgPpi
    )
{
  //EFI_STATUS Status;

  LsWdtOps.WdtEnable       =  EnableCpuWatchdog;
  LsWdtOps.WdtDisable      =  DisableCpuWatchdog;

  CustomedCfgPpi->WdtOps   = (LS_WDT_OPS*)&LsWdtOps;

  return EFI_SUCCESS;
}

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

#include <Library/TimerLib.h>
#include <Library/Ls7aGpio.h>
#include <Ppi/RcCorePpi.h>
#include <Io.h>

#define GPIO0                          0
//#define PWM_FREQ                       2000 //2Khz
UINT32
EFIAPI
GetFreq (
  VOID
  );

UINTN
EFIAPI
CsrReadTime (
  VOID
  );

VOID
GpioSimulatePwm (
    UINT16  GpioNum,
    UINT32  PwmFreq,  //unit hz
    UINT32  KeepMs    //unit ms
    )
{
    UINTN  Count, Ticks, Start, End;
    UINT64 FreqCoverTime = 1000000 / PwmFreq / 2; // 1000000 = 1Mhz

    Count = GetFreq ();
    Count = Count * KeepMs / 1000;
    Start = CsrReadTime();
    End = Start + Count;

    do {
        Ls7aGpioSetVal (GpioNum,0);
        MicroSecondDelay (FreqCoverTime);
        Ls7aGpioSetVal (GpioNum,1);
        MicroSecondDelay (FreqCoverTime);
        Ticks =  CsrReadTime();
    } while (Ticks < End);
}

EFI_STATUS
BeepPwmOn (
  UINT32   KeepMs,
  UINT32   PwmFreq
  )
{
  GpioSimulatePwm (GPIO0,PwmFreq,KeepMs);
  return EFI_SUCCESS;
}

EFI_STATUS
BeepOn (
  VOID
  )
{
  Ls7aGpioSetVal (GPIO0,0); // Low is open
  return EFI_SUCCESS;
}

EFI_STATUS
BeepOff (
  VOID
  )
{
  Ls7aGpioSetVal (GPIO0,1); // High is close
  return EFI_SUCCESS;
}

LS_BEEP_OPS  LsBeepOps;
EFI_STATUS
RegBeepFunc (
    LS_CUSTOMER_CFG_PPI   *CustomedCfgPpi
    )
{
  LsBeepOps.BeepOn        =  BeepOn;
  LsBeepOps.BeepOff       =  BeepOff;
  LsBeepOps.BeepPwmOn     =  BeepPwmOn;

  CustomedCfgPpi->BeepOps = (LS_BEEP_OPS*)&LsBeepOps;

  return EFI_SUCCESS;
}

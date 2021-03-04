/** @file

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

THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION).

**/
#include <Cpu.h>
#include <Library/ShellLib.h>
#include "UefiShellSignalTestCommandsLib.h"


STATIC CONST SHELL_PARAM_ITEM ParamList[] = {
  {L"-m", TypeValue},
  {L"-p", TypeValue},
  {NULL, TypeMax}
};

SHELL_STATUS
EFIAPI
ShellCommandRunUsbtest (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS          Status;
  LIST_ENTRY          *Package;
  CHAR16              *ProblemParam;
  SHELL_STATUS        ShellStatus;
  CONST CHAR16        *ValueStr;
  UINTN               TestMode;
  UINTN                Port;
  UINTN               Cntl;
  UINTN               TmpVal;
  UINT64              BaseAddr;

  ShellStatus         = SHELL_SUCCESS;
  Status              = EFI_SUCCESS;
  TestMode            = 0;
  Port                = -1;
  BaseAddr            = 0;

  //
  // parse the command line
  //
  Status = ShellCommandLineParse (ParamList, &Package, &ProblemParam, TRUE);

  if (EFI_ERROR(Status)) {
    if (Status == EFI_VOLUME_CORRUPTED && ProblemParam != NULL) {
      ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM), gShellSignalTestHiiHandle, L"usbtest", ProblemParam);
      FreePool(ProblemParam);
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    } else {
      ASSERT(FALSE);
    }
  } else {
    DEBUG((EFI_D_ERROR, "[debug]ShellCommandLineGetCount(Package)=%d \n",ShellCommandLineGetCount(Package)));
    DEBUG((EFI_D_ERROR, "[debug]ShellCommandLineGetFlag -m = %d  \n",ShellCommandLineGetFlag (Package, L"-m") ));
    DEBUG((EFI_D_ERROR, "[debug]ShellCommandLineGetValue -m = %s  \n",ShellCommandLineGetValue (Package, L"-m") ));

    if (ShellCommandLineGetCount(Package) != 1) {
      ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_INVALID) , gShellSignalTestHiiHandle, L"usbtest");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    } else if ((ShellCommandLineGetFlag (Package, L"-m") == 0) ||
        (ShellCommandLineGetFlag (Package, L"-m") && ShellCommandLineGetValue (Package, L"-m") == NULL )){
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_NO_VALUE), gShellSignalTestHiiHandle, L"usbtest", L"-m");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    } else if ((ShellCommandLineGetFlag (Package, L"-p") == 0) ||
        (ShellCommandLineGetFlag (Package, L"-p") && ShellCommandLineGetValue (Package, L"-p") == NULL)){
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_NO_VALUE), gShellSignalTestHiiHandle, L"usbtest", L"-p");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    }

    ValueStr = ShellCommandLineGetValue(Package, L"-m");
    if (ValueStr != NULL) {
      TestMode = ShellStrToUintn (ValueStr);
      DEBUG((EFI_D_INFO, "[debug]TestMode=%d\n", TestMode));
    }
    if ((TestMode != 1) && (TestMode != 2) && (TestMode != 3) && (TestMode != 4) && (TestMode != 5)) {
      DEBUG((EFI_D_ERROR, "Error: test mode exceed valid value[1,5] \n"));
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM_VAL), gShellSignalTestHiiHandle, L"usbtest", ValueStr, L"-m, test mode exceed valid value[1,5]");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    }

    DEBUG((EFI_D_INFO, "[debug]ShellCommandLineGetFlag -p = %d  \n",ShellCommandLineGetFlag (Package, L"-p") ));
    DEBUG((EFI_D_INFO, "[debug]ShellCommandLineGetValue -p = %s  \n",ShellCommandLineGetValue (Package, L"-p") ));
    ValueStr = ShellCommandLineGetValue(Package, L"-p");
    if (ValueStr != NULL) {
      Port = ShellStrToUintn (ValueStr);
      DEBUG((EFI_D_ERROR, "[debug]Port=%d\n", Port));
    }
    if (Port < 3 ){
      Cntl = 0;
    } else if (Port < 6){
      Cntl = 1;
      Port -= 3;
    } else {
      DEBUG((EFI_D_ERROR, "Error: port number exceed valid value(5) \n"));
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM_VAL), gShellSignalTestHiiHandle, L"usbtest", ValueStr, L"-p, port number exceed valid value(5) ");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    }
    DEBUG((EFI_D_INFO, "[debug]: USB cntl %d port %d\n", Cntl, Port));

    //get cntl base address, func 1: EHCI
    BaseAddr = *(volatile UINT32 *)(PHYS_TO_UNCACHED(0xefdfe000000) | (0 << 16) | ((4 + Cntl) << 11) | (1 << 8) | 0x10);
    DEBUG((EFI_D_INFO, "[debug] =1= base: 0x%llx\n", BaseAddr));
    BaseAddr &= 0xfffffff0;
    DEBUG((EFI_D_INFO, "[debug] =2= base: 0x%llx\n", BaseAddr));
    //BaseAddr |= 0xffffffff80000000;
    BaseAddr |= PHYS_TO_UNCACHED(0xe0000000000);
    DEBUG((EFI_D_INFO, "[debug] =3= base: 0x%llx\n", BaseAddr));

    BaseAddr += 0x10; //HC operational register base
    DEBUG((EFI_D_INFO, "[debug]: USB operational register base is: 0x%llx\n", BaseAddr));

    //reset USB to stop last test
    //  (1) Stop the  Host Controller
    TmpVal = Pt32(BaseAddr);
    Pt32(BaseAddr) = TmpVal & ~0x1;

    //  (2) Check if the host controller is in the Halted state.
    while((Pt32(BaseAddr + 4) & 0x1000) != 0x1000);

    TmpVal = Pt32(BaseAddr);
    Pt32(BaseAddr) = TmpVal | 0x2;

    //make CF from 0 to 1
    Pt32(BaseAddr + 0x40) = 0;
    Pt32(BaseAddr + 0x40) = 1;

    //disable ASE and PSE
    TmpVal = Pt32(BaseAddr);
    Pt32(BaseAddr) = TmpVal & ~0x30;

    //suspend port
    TmpVal = Pt32(BaseAddr + 0x44 + Port * 4);
    Pt32(BaseAddr + 0x44 + Port * 4) = TmpVal | 0x84;

    //stop USB and wait HCHalted
    TmpVal = Pt32(BaseAddr);
    Pt32(BaseAddr) = TmpVal & ~0x1;

    while((Pt32(BaseAddr + 4) & 0x1000) != 0x1000);

    //set test mode
    TmpVal = Pt32(BaseAddr + 0x44 + Port * 4);
    Pt32(BaseAddr + 0x44 + Port * 4) = (TmpVal & (~(0xf << 16))) | (TestMode << 16);

    if (TestMode == 5){
        TmpVal = Pt32(BaseAddr);
        Pt32(BaseAddr) = TmpVal | 0x1;
    }
    DEBUG((EFI_D_INFO, "USB test ready and start...\n"));
  }

Done:
  if (Package != NULL) {
    ShellCommandLineFreeVarList (Package);
  }
  return ShellStatus;

}

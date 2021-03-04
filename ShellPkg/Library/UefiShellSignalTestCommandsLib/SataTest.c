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
  {L"-g", TypeValue},
  {NULL, TypeMax}
};

SHELL_STATUS
EFIAPI
ShellCommandRunSatatest (
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
  UINTN               Port;
  UINTN               Gen;
  UINTN               Cntl;
  UINTN               TmpVal;
  UINT64              BaseAddr;

  ShellStatus         = SHELL_SUCCESS;
  Status              = EFI_SUCCESS;
  TestMode            = 0;
  Port                = -1;
  BaseAddr            = 0;
  Gen                 = 0;

  //
  // parse the command line
  //
  Status = ShellCommandLineParse (ParamList, &Package, &ProblemParam, TRUE);

  if (EFI_ERROR(Status)) {
    if (Status == EFI_VOLUME_CORRUPTED && ProblemParam != NULL) {
      ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM), gShellSignalTestHiiHandle, L"satatest", ProblemParam);
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
      ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_INVALID) , gShellSignalTestHiiHandle, L"satatest");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    } else if((ShellCommandLineGetFlag (Package, L"-m") == 0) ||
        (ShellCommandLineGetFlag (Package, L"-m") && ShellCommandLineGetValue (Package, L"-m") == NULL )){
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_NO_VALUE), gShellSignalTestHiiHandle, L"satatest", L"-m");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    } else if ((ShellCommandLineGetFlag (Package, L"-p") == 0) ||
        (ShellCommandLineGetFlag (Package, L"-p") && ShellCommandLineGetValue (Package, L"-p") == NULL )){
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_NO_VALUE), gShellSignalTestHiiHandle, L"satatest", L"-p");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    } else if ((ShellCommandLineGetFlag (Package, L"-g") == 0) ||
        (ShellCommandLineGetFlag (Package, L"-g") && ShellCommandLineGetValue (Package, L"-g") == NULL )){
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_NO_VALUE), gShellSignalTestHiiHandle, L"satatest", L"-g");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    }

    ValueStr = ShellCommandLineGetValue(Package, L"-m");
    if (ValueStr != NULL) {
      TestMode = ShellStrToUintn (ValueStr);
      DEBUG((EFI_D_INFO, "[debug]TestMode=%d\n", TestMode));
    }
    if ((TestMode != 0) && (TestMode != 1) &&
        (TestMode != 2) && (TestMode != 3) &&
        (TestMode != 4) && (TestMode != 5) &&
        (TestMode != 6) && (TestMode != 7) &&
        (TestMode != 8)) {
      DEBUG((EFI_D_ERROR, "Error: test mode exceed valid value[0,8] \n"));
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM_VAL), gShellSignalTestHiiHandle, L"satatest", ValueStr, L"-m, test mode exceed valid value[0,8]");
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

    if ((Port != 0) && (Port != 1) && (Port != 2)){
      DEBUG((EFI_D_ERROR, "Error: port number exceed valid value[0,2] \n"));
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM_VAL), gShellSignalTestHiiHandle, L"satatest", ValueStr, L"-p, port number exceed valid value[0,2]");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    }
    Cntl = Port;
    DEBUG((EFI_D_INFO, "[debug]: Sata cntl %d port %d\n", Cntl, Port));


    DEBUG((EFI_D_INFO, "[debug]ShellCommandLineGetFlag -g = %d  \n",ShellCommandLineGetFlag (Package, L"-g") ));
    DEBUG((EFI_D_INFO, "[debug]ShellCommandLineGetValue -g = %s  \n",ShellCommandLineGetValue (Package, L"-g") ));

    ValueStr = ShellCommandLineGetValue(Package, L"-g");
    if (ValueStr != NULL) {
      Gen = ShellStrToUintn (ValueStr);
      DEBUG((EFI_D_ERROR, "[debug]Gen=%d\n", Gen));
    }

    if ((Gen != 1) && (Gen != 2)){
      DEBUG((EFI_D_ERROR, "Error: Gen is not a valid value[1,2] \n"));
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM_VAL), gShellSignalTestHiiHandle, L"satatest", ValueStr, L"-g, Gen is not a valid value[1,2] ");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    }


    //get cntl base address
    BaseAddr = *(volatile UINT32 *)(PHYS_TO_UNCACHED(0xefdfe000000)| (0 << 16) | (8 << 11) | (Cntl << 8) | 0x10);
    BaseAddr &= 0xfffffff0;
    BaseAddr |= PHYS_TO_UNCACHED(0xe0000000000);

    DEBUG((EFI_D_INFO, "[debug]: request test: port: %d, gen: %d, test mode: %d\n", Port, Gen, TestMode));
    DEBUG((EFI_D_INFO, "[debug]: SATA ctrl register BaseAddr is: 0x%llx\n", BaseAddr));

    //TODO PHY cfg, GEN sel
    //if (Gen == 1)
    //    pt8(BaseAddr + 0x8012) = 0x0;
    //else if (Gen == 2)
    //    pt8(BaseAddr + 0x8012) = 0x9;
    //pt8(BaseAddr + 0x8010) = 0x1;

    //set port to 0
    Pt32(BaseAddr + 0xf4) = 0x0;
    //
    TmpVal = Pt32(BaseAddr + 0xa4);
    //transmit only
    Pt32(BaseAddr + 0xa4) = ((1 << 18) | (TmpVal & ~0xf & ~(0x7 << 8)) | TestMode);
  }


Done:
  if (Package != NULL) {
    ShellCommandLineFreeVarList (Package);
  }
  return ShellStatus;

}

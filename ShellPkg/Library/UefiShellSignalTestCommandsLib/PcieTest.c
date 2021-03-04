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
  {L"-sp", TypeValue},
  {L"-g", TypeValue},
  {NULL, TypeMax}
};

SHELL_STATUS
EFIAPI
ShellCommandRunPcietest (
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
  UINTN               DevNum;
  UINTN               Bar0Low;
  UINTN               Bar0High;
  UINT64              Header;
  UINT64              Bar0;
  UINTN               SubPort;

  ShellStatus         = SHELL_SUCCESS;
  Status              = EFI_SUCCESS;
  TestMode            = 0;
  Port                = -1;
  Gen                 = 0;
  SubPort             = -1;

  //
  // parse the command line
  //
  Status = ShellCommandLineParse (ParamList, &Package, &ProblemParam, TRUE);

  if (EFI_ERROR(Status)) {
    if (Status == EFI_VOLUME_CORRUPTED && ProblemParam != NULL) {
      ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM), gShellSignalTestHiiHandle, L"pcietest", ProblemParam);
      FreePool(ProblemParam);
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    } else {
      ASSERT(FALSE);
    }
  } else {
    DEBUG((EFI_D_INFO, "[debug]ShellCommandLineGetCount(Package)=%d \n",ShellCommandLineGetCount(Package)));
    DEBUG((EFI_D_INFO, "[debug]ShellCommandLineGetFlag -m = %d  \n",ShellCommandLineGetFlag (Package, L"-m") ));
    DEBUG((EFI_D_INFO, "[debug]ShellCommandLineGetValue -m = %s  \n",ShellCommandLineGetValue (Package, L"-m") ));

    if (ShellCommandLineGetCount(Package) != 1) {
      ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_INVALID) , gShellSignalTestHiiHandle, L"pcietest");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    } else if ( (ShellCommandLineGetFlag (Package, L"-p") == 0 ) ||
      (ShellCommandLineGetFlag (Package, L"-p") && ShellCommandLineGetValue (Package, L"-p") == NULL)){
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_NO_VALUE), gShellSignalTestHiiHandle, L"pcietest", L"-p");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    } else if ((ShellCommandLineGetFlag (Package, L"-g") == 0 ) ||
      (ShellCommandLineGetFlag (Package, L"-g") && ShellCommandLineGetValue (Package, L"-g") == NULL)){
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_NO_VALUE), gShellSignalTestHiiHandle, L"pcietest", L"-g");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    } else if ((ShellCommandLineGetFlag (Package, L"-sp") == 0 ) ||
      (ShellCommandLineGetFlag (Package, L"-sp") && ShellCommandLineGetValue (Package, L"-sp") == NULL)){
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_NO_VALUE), gShellSignalTestHiiHandle, L"pcietest", L"-sp");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    }

    DEBUG((EFI_D_INFO, "[debug]ShellCommandLineGetFlag -g = %d  \n",ShellCommandLineGetFlag (Package, L"-g") ));
    DEBUG((EFI_D_INFO, "[debug]ShellCommandLineGetValue -g = %s  \n",ShellCommandLineGetValue (Package, L"-g") ));

    ValueStr = ShellCommandLineGetValue(Package, L"-g");
    if (ValueStr != NULL) {
      Gen = ShellStrToUintn (ValueStr);
      DEBUG((EFI_D_ERROR, "[debug]Gen=%d\n", Gen));
    }

    if ((Gen != 1) && (Gen != 2)){
      DEBUG((EFI_D_ERROR, "Error: Gen is not a valid value{1,2} \n"));
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM_VAL), gShellSignalTestHiiHandle, L"pcietest", ValueStr, L"-g, Gen is not a valid value[1,2]");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    } else if (Gen == 2){
      if ( (ShellCommandLineGetFlag (Package, L"-m") == 0) ||
         (ShellCommandLineGetFlag (Package, L"-m") && ShellCommandLineGetValue (Package, L"-m") == NULL)) {
        ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_NO_VALUE), gShellSignalTestHiiHandle, L"pcietest", L"-m, You must set testmode for gen2");
        ShellStatus = SHELL_INVALID_PARAMETER;
        goto Done;
      }

      ValueStr = ShellCommandLineGetValue(Package, L"-m");
      if (ValueStr != NULL) {
        TestMode = ShellStrToUintn (ValueStr);
        DEBUG((EFI_D_INFO, "[debug]TestMode=%d\n", TestMode));
    }
      if ((TestMode != 1) && (TestMode != 2) &&
          (TestMode != 3) && (TestMode != 4) &&
          (TestMode != 5) && (TestMode != 6)) {
        DEBUG((EFI_D_ERROR, "Error: test mode exceed valid value[1,6] \n"));
        ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM_VAL), gShellSignalTestHiiHandle, L"pcietest", ValueStr, L"-m, Test mode exceed valid value{1,2,3,4,5,6} ");
        ShellStatus = SHELL_INVALID_PARAMETER;
        goto Done;
      }
    }

    DEBUG((EFI_D_INFO, "[debug]ShellCommandLineGetFlag -p = %d  \n",ShellCommandLineGetFlag (Package, L"-p") ));
    DEBUG((EFI_D_INFO, "[debug]ShellCommandLineGetValue -p = %s  \n",ShellCommandLineGetValue (Package, L"-p") ));
    ValueStr = ShellCommandLineGetValue(Package, L"-p");
    if (ValueStr != NULL) {
      Port = ShellStrToUintn (ValueStr);
      DEBUG((EFI_D_ERROR, "[debug]Port=%d\n", Port));
    }

    if ((Port != 0) && (Port != 1) && (Port != 2) &&
        (Port != 3) && (Port != 4)){
      DEBUG((EFI_D_ERROR, "Error: port number exceed valid value{0,1,2,3,4} \n"));
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM_VAL), gShellSignalTestHiiHandle, L"pcietest", ValueStr, L"-p ,Port number exceed valid value{0,1,2,3,4} ");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    }
    DEBUG((EFI_D_INFO, "[debug]ShellCommandLineGetFlag -sp = %d  \n",ShellCommandLineGetFlag (Package, L"-sp") ));
    DEBUG((EFI_D_INFO, "[debug]ShellCommandLineGetValue -sp = %s  \n",ShellCommandLineGetValue (Package, L"-sp") ));

    ValueStr = ShellCommandLineGetValue(Package, L"-sp");
    if (ValueStr != NULL) {
      SubPort = ShellStrToUintn (ValueStr);
      DEBUG((EFI_D_ERROR, "[debug]SubPort=%d\n", SubPort));
    }
    if ((SubPort != 0) && (SubPort != 1) && (SubPort != 2) && (SubPort != 3)) {
      DEBUG((EFI_D_ERROR, "Error: port number exceed valid value{0,1,2,3} \n"));
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM_VAL), gShellSignalTestHiiHandle, L"pcietest", ValueStr, L"-sp ,Port number exceed valid value{0,1,2,3} ");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    }
    if (Port != 0){
        if((SubPort == 2)||(SubPort == 3)){
        DEBUG((EFI_D_ERROR, "Error: port1,2,3,4 number exceed valid value{0,1} \n"));
        ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM_VAL), gShellSignalTestHiiHandle, L"pcietest", ValueStr, L"-sp ,Port1,2,3,4  exceed valid value{0,1} ");
        ShellStatus = SHELL_INVALID_PARAMETER;
        goto Done;
        }
    }
    //  port num: 0 -> f0
    //            -> [SubPort0] Bus 0:Device 9:Function 0 (F0 port0)
    //               [SubPort1] Bus 0:Device 10:Function 0 (F0 port1)
    //               [SubPort2] Bus 0:Device 11:Function 0 (F0 port2)
    //               [SubPort3] Bus 0:Device 12:Function 0 (F0 port3)
    //  port num: 1 -> f1
    //            -> [SubPort0]Bus 0:Device 13:Function 0 (F1 port0
    //            -> [SubPort1]Bus 0:Device 14:Function 0 (F1 port1)
    //  port num: 2 -> g0
    //            -> [SubPort0]Bus 0:Device 15:Function 0 (G0 port0)
    //            -> [SubPort1]Bus 0:Device 16:Function 0 (G0 port1)
    //  port num: 3 -> g1
    //            -> [SubPort0]Bus 0:Device 17:Function 0 (G0 port0)
    //            -> [SubPort1]Bus 0:Device 18:Function 0 (G0 port1)
    //  port num: 4 -> h
    //            -> [SubPort0]Bus 0:Device 19:Function 0 (H port0)
    //            -> [SubPort1]Bus 0:Device 20:Function 0 (H port1)
    DevNum =  Port  == 0 ? 9  :
              Port  == 1 ? 13 :
              Port  == 2 ? 15 :
              Port  == 3 ? 17 :
              Port  == 4 ? 19 :
                           9;

    Header = PCIE_CONF_TYPE0_ADDR | ( (DevNum + SubPort ) << 11 );

    if (Gen == 2) {
      // the low 4 bit must be 2.
      Pt32(Header + 0x7c) = 0x533c42;
    }

    Pt32(Header + (0x8 << 24) + 0x0c) = 0x2040f;

    Bar0Low  = (Pt32(Header + 0x10) & 0xffffffff0);
    Bar0High = (Pt32(Header + 0x14));
    Bar0 = (Bar0High << 32 | Bar0Low) + PHYS_TO_UNCACHED(0xe0000000000);
    DEBUG((EFI_D_INFO, "[debug]pcie Header = 0x%llx\n",Header));
    DEBUG((EFI_D_INFO, "[debug]pcie Bar0Low = 0x%x\n",Bar0Low));
    DEBUG((EFI_D_INFO, "[debug]pcie Bar0High = 0x%x\n",Bar0High));
    DEBUG((EFI_D_INFO, "[debug]pcie Bar0 = 0x%llx\n",Bar0));
    Pt32(Bar0) = 0xff204c;
    DEBUG((EFI_D_INFO, "[debug] 0x%x\n", Pt32(Bar0)));



    if (Gen == 0x1) {
        Pt32(Header + 0xa0) = 0xfc51;
        DEBUG((EFI_D_INFO, "[debug] Gen :%d  0x%x\n", Gen, Pt32(Header + 0xa0)));
    } else if (Gen == 0x2){
      switch (TestMode) {
        case 1:
            Pt32(Header + 0xa0) = 0xf052;
            DEBUG((EFI_D_INFO, "[debug] mode = %d  0x%x\n", TestMode, Pt32(Header + 0xa0)));
            break;
        case 2:
            Pt32(Header + 0xa0) =  0xf012;
            DEBUG((EFI_D_INFO, "[debug] mode = %d  0x%x\n", TestMode, Pt32(Header + 0xa0)));
            break;
        case 3:
            Pt32(Header + 0xa0) = 0xf452;
            DEBUG((EFI_D_INFO, "[debug] mode = %d  0x%x\n", TestMode, Pt32(Header + 0xa0)));
            break;
        case 4:
            Pt32(Header + 0xa0) = 0xf412;
            DEBUG((EFI_D_INFO, "[debug] mode = %d  0x%x\n", TestMode, Pt32(Header + 0xa0)));
            break;
        case 5:
            Pt32(Header + 0xa0) = 0xfc52;
            DEBUG((EFI_D_INFO, "[debug] mode = %d  0x%x\n", TestMode, Pt32(Header + 0xa0)));
            break;
        case 6:
            Pt32(Header + 0xa0) = 0xfc12;
            DEBUG((EFI_D_INFO, "[debug] mode = %d  0x%x\n", TestMode, Pt32(Header + 0xa0)));
            break;
        default:
            break;
     }
   }
    Pt32(Header + (0x7 << 24) + 0x08) = 0x7028004;
    DEBUG((EFI_D_INFO, "[debug] 0x%x \n", Pt32(Header + (0x7 << 24) + 0x08)));
  }

Done:
  if (Package != NULL) {
    ShellCommandLineFreeVarList (Package);
  }
  return ShellStatus;

}

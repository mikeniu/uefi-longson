
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
#include <Library/ShellLib.h>
#include "UefiShellSignalTestCommandsLib.h"


#define EFI_PCIE_PORT_F0 0
#define EFI_PCIE_PORT_F1 1
#define EFI_PCIE_PORT_G0 2
#define EFI_PCIE_PORT_G1 3
#define EFI_PCIE_PORT_H  4

#define EFI_PCIE_PORT_X1_MODE 1
#define EFI_PCIE_PORT_X4_MODE 4
#define EFI_PCIE_PORT_X8_MODE 8

STATIC CONST SHELL_PARAM_ITEM ParamList[] = {
  {L"-l"},            //List the Current Settings
  {L"-s"},            //Start Signal Test
  {L"-r"},            //ReConfig
  {L"-p", TypeValue}, //Port [0,4]   -> {F0,F1,G0,G1,H}
  {L"-c", TypeValue}, //Clock[0,1]   -> {0:OFF 1:ON}
  {L"-m", TypeValue}, //Mode {1,4,8} -> {1:X1 4:X4 8:X8}
  {NULL, TypeMax}
};

SHELL_STATUS
EFIAPI
ShellCommandRunPcieSwitch (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS          Status;
  LIST_ENTRY          *Package;
  CHAR16              *ProblemParam;
  SHELL_STATUS        ShellStatus;
  CONST CHAR16        *ValueStr;
  UINTN               Mode;
  UINTN               Port   = -1;
  UINTN               Clock  =  1;
  UINTN               ReConfigData = 1;
  UINTN               EnSiTestData = 1;

  CONST CHAR16        *VariableName;
  ShellStatus         = SHELL_SUCCESS;
  Status              = EFI_SUCCESS;

  Status = ShellCommandLineParse (ParamList, &Package, &ProblemParam, TRUE);
  EFI_RUNTIME_SERVICES    *gRT = NULL;
  gRT = SystemTable->RuntimeServices;
  UINTN Size = sizeof(UINTN);
  UINT32 Attributes = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS |EFI_VARIABLE_BOOTSERVICE_ACCESS;

  //
  // parse the command line
  //
  if (EFI_ERROR(Status)) {
    if (Status == EFI_VOLUME_CORRUPTED && ProblemParam != NULL) {
      ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM), gShellSignalTestHiiHandle, L"pcieswitch", ProblemParam);
      FreePool(ProblemParam);
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    } else {
      ASSERT(FALSE);
    }
  } else {

    DEBUG((EFI_D_INFO, "[DBG]ShellCommandLineGetCount(Package)=%d \n",ShellCommandLineGetCount(Package)));
    if (ShellCommandLineGetCount(Package) != 1) {
      ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_INVALID) , gShellSignalTestHiiHandle, L"pcieswitch");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    } else if ( (ShellCommandLineGetFlag (Package, L"-l") == 1 ) ) {
        if( (ShellCommandLineGetFlag (Package, L"-s") == 0 ) && (ShellCommandLineGetFlag (Package, L"-r") == 0 ) &&
        (ShellCommandLineGetFlag (Package, L"-m") == 0 ) && (ShellCommandLineGetFlag (Package, L"-c") == 0 ) &&
        (ShellCommandLineGetFlag (Package, L"-p") == 0 ) ){
        goto ListCurrentSettings;
        }else{
          ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_SINGLE_LI), gShellSignalTestHiiHandle);
          goto Done;
        }
    } else if ( (ShellCommandLineGetFlag (Package, L"-r") == 1 ) ) {
        if( (ShellCommandLineGetFlag (Package, L"-s") == 0 ) && (ShellCommandLineGetFlag (Package, L"-p") == 0 ) &&
        (ShellCommandLineGetFlag (Package, L"-m") == 0 ) && (ShellCommandLineGetFlag (Package, L"-c") == 0 ) ){
        goto ReConfig;
        }else{
          ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_SINGLE_RE), gShellSignalTestHiiHandle);
          goto Done;
        }
    } else if ( (ShellCommandLineGetFlag (Package, L"-s") == 1 ) ) {
        if( (ShellCommandLineGetFlag (Package, L"-p") == 0 ) &&
        (ShellCommandLineGetFlag (Package, L"-m") == 0 ) && (ShellCommandLineGetFlag (Package, L"-c") == 0 ) ){
        goto EnableSiTest;
        }else{
          ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_SINGLE_SI), gShellSignalTestHiiHandle);
          goto Done;
        }
    } else if ( (ShellCommandLineGetFlag (Package, L"-p") == 0 ) ||
      (ShellCommandLineGetFlag (Package, L"-p") && ShellCommandLineGetValue (Package, L"-p") == NULL)){
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_NO_VALUE), gShellSignalTestHiiHandle, L"pcieswitch", L"-p");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    } else if ((ShellCommandLineGetFlag (Package, L"-c") == 0 ) ||
      (ShellCommandLineGetFlag (Package, L"-c") && ShellCommandLineGetValue (Package, L"-c") == NULL)){
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_NO_VALUE), gShellSignalTestHiiHandle, L"pcieswitch", L"-c");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    } else if ((ShellCommandLineGetFlag (Package, L"-m") == 0 ) ||
      (ShellCommandLineGetFlag (Package, L"-m") && ShellCommandLineGetValue (Package, L"-c") == NULL)){
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_NO_VALUE), gShellSignalTestHiiHandle, L"pcieswitch", L"-m");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    }
    DEBUG((EFI_D_INFO, "[DBG]ShellCommandLineGetFlag -p = %d  \n",ShellCommandLineGetFlag (Package, L"-p") ));
    DEBUG((EFI_D_INFO, "[DBG]ShellCommandLineGetValue -p = %s  \n",ShellCommandLineGetValue (Package, L"-p") ));

    ValueStr = ShellCommandLineGetValue(Package, L"-p");
    if (ValueStr != NULL) {
      Port = ShellStrToUintn (ValueStr);
      DEBUG((EFI_D_ERROR, "[debug]Port = %d\n", Port));
    }
    if ((Port != 1) && (Port != 2) &&
        (Port != 3) && (Port != 4) &&
        (Port != 0)) {
      DEBUG((EFI_D_ERROR, "Error: pcie Port exceed valid value[0,4] \n"));
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM_VAL), gShellSignalTestHiiHandle, L"pcieswitch", ValueStr, L"-p, Pcie Port exceed valid value[0,4] ");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    }
    DEBUG((EFI_D_INFO, "[DBG]ShellCommandLineGetFlag -c = %d  \n",ShellCommandLineGetFlag (Package, L"-c") ));
    DEBUG((EFI_D_INFO, "[DBG]ShellCommandLineGetValue -c = %s  \n",ShellCommandLineGetValue (Package, L"-c") ));

    ValueStr = ShellCommandLineGetValue(Package, L"-c");
    if (ValueStr != NULL) {
      Clock = ShellStrToUintn (ValueStr);
      DEBUG((EFI_D_ERROR, "[DBG]Clock=%d\n", Clock));
    }

    if ((Clock != 0) && (Clock != 1)){
      DEBUG((EFI_D_ERROR, "Error: Clock is not a valid value[0,1] \n"));
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM_VAL), gShellSignalTestHiiHandle, L"pcieswitch", ValueStr, L"-g, Clock is not a valid value[0,1]");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    }

    DEBUG((EFI_D_INFO, "[DBG]ShellCommandLineGetFlag -m = %d  \n",ShellCommandLineGetFlag (Package, L"-m") ));
    DEBUG((EFI_D_INFO, "[DBG]ShellCommandLineGetValue -m = %s  \n",ShellCommandLineGetValue (Package, L"-m") ));

    ValueStr = ShellCommandLineGetValue(Package, L"-m");
    if (ValueStr != NULL) {
      Mode = ShellStrToUintn (ValueStr);
      DEBUG((EFI_D_ERROR, "[DBG]Mode=%d\n", Mode));
    }

    if ((Mode != 1) && (Mode != 4) && ((Port == 0)||(Port == 1)) )
    {
      DEBUG((EFI_D_ERROR, "Error: F0,F1 Only Enable X1,X4 Mode\n"));
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM_VAL), gShellSignalTestHiiHandle, L"pcieswitch", ValueStr, L"-m, F0,F1 Mode is not a valid value{1,4}");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    }
    if ((Mode != 4) && (Mode != 8) && ((Port == 2)||(Port == 3)||(Port == 4)) )
    {
      DEBUG((EFI_D_ERROR, "Error: G0,G1,H Only Enable X4,X8 Mode\n"));
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM_VAL), gShellSignalTestHiiHandle, L"pcieswitch", ValueStr, L"-m, G0,G1,H Mode is not a valid value{4,8}");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    }

    switch(Port)
    {
      case EFI_PCIE_PORT_F0 :
        VariableName = L"Pcie_F0_Clock";
        Status = gRT->SetVariable ((CHAR16*)VariableName, &gF0ClockVendorGuid, Attributes, Size, &Clock);
        (Mode == EFI_PCIE_PORT_X4_MODE)? (Mode = EFI_PCIE_PORT_X4_MODE) : (Mode = EFI_PCIE_PORT_X1_MODE);
        VariableName = L"Pcie_F0_Mode";
        Status = gRT->SetVariable((CHAR16*)VariableName, &gPcieF0ModeVendorGuid, Attributes, Size, &Mode);
        break;
      case EFI_PCIE_PORT_F1 :
        VariableName = L"Pcie_F1_Clock";
        Status = gRT->SetVariable ((CHAR16*)VariableName, &gF1ClockVendorGuid, Attributes, Size, &Clock);
        (Mode == EFI_PCIE_PORT_X4_MODE)? (Mode = EFI_PCIE_PORT_X4_MODE) : (Mode = EFI_PCIE_PORT_X1_MODE);
        VariableName = L"Pcie_F1_Mode";
        Status = gRT->SetVariable ((CHAR16*)VariableName, &gPcieF1ModeVendorGuid, Attributes, Size, &Mode);
        break;
      case EFI_PCIE_PORT_G0 :
        VariableName = L"Pcie_G0_Clock";
        Status = gRT->SetVariable ((CHAR16*)VariableName, &gG0ClockVendorGuid, Attributes, Size, &Clock);
        (Mode == EFI_PCIE_PORT_X8_MODE)? (Mode = EFI_PCIE_PORT_X8_MODE) : (Mode = EFI_PCIE_PORT_X4_MODE);
        VariableName = L"Pcie_G0_Mode";
        Status = gRT->SetVariable ((CHAR16*)VariableName, &gPcieG0ModeVendorGuid, Attributes, Size, &Mode);
        break;
      case EFI_PCIE_PORT_G1 :
        VariableName = L"Pcie_G1_Clock";
        Status = gRT->SetVariable ((CHAR16*)VariableName, &gG1ClockVendorGuid, Attributes, Size, &Clock);
        (Mode == EFI_PCIE_PORT_X8_MODE)? (Mode = EFI_PCIE_PORT_X8_MODE) : (Mode = EFI_PCIE_PORT_X4_MODE);
        VariableName = L"Pcie_G1_Mode";
        Status = gRT->SetVariable ((CHAR16*)VariableName, &gPcieG1ModeVendorGuid, Attributes, Size, &Mode);
        break;
      case EFI_PCIE_PORT_H  :
        VariableName = L"Pcie_H_Clock";
        Status = gRT->SetVariable ((CHAR16*)VariableName, &gHClockVendorGuid, Attributes, Size, &Clock);
        (Mode == EFI_PCIE_PORT_X8_MODE)? (Mode = EFI_PCIE_PORT_X8_MODE) : (Mode = EFI_PCIE_PORT_X4_MODE);
        VariableName = L"Pcie_H_Mode";
        Status = gRT->SetVariable ((CHAR16*)VariableName, &gPcieHModeVendorGuid, Attributes, Size, &Mode);
        break;
      default:
        break;
    }
    ASSERT_EFI_ERROR(Status);
    DEBUG((EFI_D_ERROR, "[DBG]Enable and Mode Chose Success! Now You Need Restart To Use It! Have a nice day! :) \n"));
    goto Done;
  }
ListCurrentSettings:
  VariableName = L"PcieReconfig";
  Status = gRT->GetVariable ((CHAR16*)VariableName, &gPcieReConfigVendorGuid, &Attributes, &Size, &ReConfigData);
  if(ReConfigData == 1){
    ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_INRECFG), gShellSignalTestHiiHandle);
    goto Done;
  } else {
    ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_NORECFG), gShellSignalTestHiiHandle);
  }
  VariableName = L"Enable_Si_Test";
  Status = gRT->GetVariable((CHAR16*)VariableName, &gEnableSiTestVendorGuid, &Attributes, &Size, &EnSiTestData);
  if(EnSiTestData == 0){
    ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_DISSI), gShellSignalTestHiiHandle);
    goto Done;
  } else {
    ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_ENSI), gShellSignalTestHiiHandle);
  }

  VariableName = L"Pcie_F0_Clock";
  Status = gRT->GetVariable ((CHAR16*)VariableName, &gF0ClockVendorGuid, &Attributes, &Size, &Clock);
  VariableName = L"Pcie_F0_Mode";
  Status = gRT->GetVariable ((CHAR16*)VariableName, &gPcieF0ModeVendorGuid, &Attributes, &Size, &Mode);
  (Clock == 0)? ( (Mode == 1)? ( ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_F0_OFF_M1), gShellSignalTestHiiHandle) )
                             : ( ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_F0_OFF_M4), gShellSignalTestHiiHandle) )
                )
              : ( (Mode == 1)? ( ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_F0_ON_M1), gShellSignalTestHiiHandle) )
                             : ( ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_F0_ON_M4), gShellSignalTestHiiHandle) )
                );

  VariableName = L"Pcie_F1_Clock";
  Status = gRT->GetVariable ((CHAR16*)VariableName, &gF1ClockVendorGuid, &Attributes, &Size, &Clock);
  VariableName = L"Pcie_F1_Mode";
  Status = gRT->GetVariable ((CHAR16*)VariableName, &gPcieF1ModeVendorGuid, &Attributes, &Size, &Mode);
  (Clock == 0)? ( (Mode == 1)? ( ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_F1_OFF_M1), gShellSignalTestHiiHandle) )
                             : ( ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_F1_OFF_M4), gShellSignalTestHiiHandle) )
                )
              : ( (Mode == 1)? ( ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_F1_ON_M1), gShellSignalTestHiiHandle) )
                             : ( ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_F1_ON_M4), gShellSignalTestHiiHandle) )
                );

  VariableName = L"Pcie_G0_Clock";
  Status = gRT->GetVariable ((CHAR16*)VariableName, &gG0ClockVendorGuid, &Attributes, &Size, &Clock);
  VariableName = L"Pcie_G0_Mode";
  Status = gRT->GetVariable ((CHAR16*)VariableName, &gPcieG0ModeVendorGuid, &Attributes, &Size, &Mode);
  (Clock == 0)? ( (Mode == 4)? ( ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_G0_OFF_M4), gShellSignalTestHiiHandle) )
                             : ( ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_G0_OFF_M8), gShellSignalTestHiiHandle) )
                )
              : ( (Mode == 4)? ( ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_G0_ON_M4), gShellSignalTestHiiHandle) )
                             : ( ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_G0_ON_M8), gShellSignalTestHiiHandle) )
                );

  VariableName = L"Pcie_G1_Clock";
  Status = gRT->GetVariable ((CHAR16*)VariableName, &gG1ClockVendorGuid, &Attributes, &Size, &Clock);
  VariableName = L"Pcie_G1_Mode";
  Status = gRT->GetVariable ((CHAR16*)VariableName, &gPcieG1ModeVendorGuid, &Attributes, &Size, &Mode);
  (Clock == 0)? ( (Mode == 4)? ( ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_G1_OFF_M4), gShellSignalTestHiiHandle) )
                             : ( ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_G1_OFF_M8), gShellSignalTestHiiHandle) )
                )
              : ( (Mode == 4)? ( ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_G1_ON_M4), gShellSignalTestHiiHandle) )
                             : ( ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_G1_ON_M8), gShellSignalTestHiiHandle) )
                );

  VariableName = L"Pcie_H_Clock";
  Status = gRT->GetVariable ((CHAR16*)VariableName, &gHClockVendorGuid, &Attributes, &Size, &Clock);
  VariableName = L"Pcie_H_Mode";
  Status = gRT->GetVariable ((CHAR16*)VariableName, &gPcieHModeVendorGuid, &Attributes, &Size, &Mode);
  (Clock == 0)? ( (Mode == 4)? ( ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_H_OFF_M4), gShellSignalTestHiiHandle) )
                             : ( ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_H_OFF_M8), gShellSignalTestHiiHandle) )
                )
              : ( (Mode == 4)? ( ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_H_ON_M4), gShellSignalTestHiiHandle) )
                             : ( ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_LIST_H_ON_M8), gShellSignalTestHiiHandle) )
                );
  goto Done;
EnableSiTest:
  VariableName = L"Enable_Si_Test";
  Status = gRT->SetVariable ((CHAR16*)VariableName, &gEnableSiTestVendorGuid, Attributes, Size, &EnSiTestData);
  ASSERT_EFI_ERROR(Status);
  DEBUG((EFI_D_ERROR, "[DBG]EnableSiTest Success! Now You Can Chose Mode and Clock To Use It! Have a nice day! :) \n"));
  ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_ENABLE_SI_TEST), gShellSignalTestHiiHandle,Mode);
  goto Done;
ReConfig:
  VariableName = L"PcieReconfig";
  Status = gRT->SetVariable ((CHAR16*)VariableName, &gPcieReConfigVendorGuid, Attributes, Size, &ReConfigData);
  ASSERT_EFI_ERROR(Status);
  DEBUG((EFI_D_ERROR, "[DBG]Pcie ReConfig Success! Now You Need Restart To Use It! Have a nice day! :) \n"));
  ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_RECONFIG_SETDOWN), gShellSignalTestHiiHandle);
Done:
  if (Package != NULL) {
    ShellCommandLineFreeVarList (Package);
  }

  return ShellStatus;
}

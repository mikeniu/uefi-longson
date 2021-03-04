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
#include "UefiShellSignalTestCommandsLib.h"

CONST CHAR16 gShellSignalTestFileName[] = L"ShellCommands";
EFI_HANDLE gShellSignalTestHiiHandle = NULL;

/**
  return the file name of the help text file if not using HII.

  @return The string pointer to the file name.
**/
CONST CHAR16*
EFIAPI
ShellCommandGetManFileNameSignalTest (
  VOID
  )
{
  return (gShellSignalTestFileName);
}

/**
  Constructor for the Shell Signal Test Commands library.

  Install the handlers for Signal Test UEFI Shell 2.0 profile commands.

  @param ImageHandle            The image handle of the process.
  @param SystemTable            The EFI System Table pointer.

  @retval EFI_SUCCESS           The shell command handlers were installed sucessfully.
  @retval EFI_UNSUPPORTED       The shell level required was not found.
**/
EFI_STATUS
EFIAPI
ShellSignalTestCommandsLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  gShellSignalTestHiiHandle = NULL;

  //
  // check our bit of the profiles mask
  //
//  if ((PcdGet8(PcdShellProfileMask) & BIT3) == 0) {
//    return (EFI_SUCCESS);
//  }

  gShellSignalTestHiiHandle = HiiAddPackages (&gShellSignalTestHiiGuid, gImageHandle, UefiShellSignalTestCommandsLibStrings, NULL);
  if (gShellSignalTestHiiHandle == NULL) {
    return (EFI_DEVICE_ERROR);
  }
  //
  // install our shell command handlers
  //
  ShellCommandRegisterCommandName(L"usbtest",   ShellCommandRunUsbtest  ,   ShellCommandGetManFileNameSignalTest, 0, L"signal test", TRUE , gShellSignalTestHiiHandle, STRING_TOKEN(STR_GET_HELP_USBTEST));
  ShellCommandRegisterCommandName(L"pcietest",  ShellCommandRunPcietest ,   ShellCommandGetManFileNameSignalTest, 0, L"signal test", TRUE , gShellSignalTestHiiHandle, STRING_TOKEN(STR_GET_HELP_PCIETEST));
  ShellCommandRegisterCommandName(L"satatest",  ShellCommandRunSatatest ,   ShellCommandGetManFileNameSignalTest, 0, L"signal test", TRUE , gShellSignalTestHiiHandle, STRING_TOKEN(STR_GET_HELP_SATATEST));
  ShellCommandRegisterCommandName(L"pcieswitch",ShellCommandRunPcieSwitch , ShellCommandGetManFileNameSignalTest, 0, L"signal test", TRUE , gShellSignalTestHiiHandle, STRING_TOKEN(STR_GET_HELP_PCIESWITCH));

  return (EFI_SUCCESS);
}

/**
  Destructor for the library.  free any resources.

  @param ImageHandle            The image handle of the process.
  @param SystemTable            The EFI System Table pointer.
**/
EFI_STATUS
EFIAPI
ShellSignalTestCommandsLibDestructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  if (gShellSignalTestHiiHandle != NULL) {
    HiiRemovePackages(gShellSignalTestHiiHandle);
  }
  return (EFI_SUCCESS);
}

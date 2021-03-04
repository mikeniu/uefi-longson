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
#include "UefiLoongsonTestCommandLib.h"

CONST CHAR16 gUefiLoongsonTestFileName[] = L"ShellCommand";
EFI_HANDLE gUefiLoongsonTestHiiHandle = NULL;

/**
  return the file name of the help text file if not using HII.

  @return The string pointer to the file name.
**/
CONST CHAR16*
EFIAPI
ShellCommandGetManFileNameLoongsonTest (
  VOID
  )
{
  return (gUefiLoongsonTestFileName);
}

/**
  Constructor for the Shell Vers Command library.

  Install the handlers for vers UEFI Shell command.

  @param ImageHandle         The image handle of the process.
  @param SysTable            The EFI System Table pointer.

  @retval EFI_SUCCESS           The shell command handlers were installed sucessfully.
  @retval EFI_UNSUPPORTED       The shell level required was not found.
**/
EFI_STATUS
EFIAPI
LoongsonTestCommandLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SysTable
  )
{
  gUefiLoongsonTestHiiHandle = NULL;

  gUefiLoongsonTestHiiHandle = HiiAddPackages (&gShellLoongsonTestHiiGuid, ImageHandle, UefiLoongsonTestCommandLibStrings, NULL);
  if (gUefiLoongsonTestHiiHandle == NULL) {
    return (EFI_DEVICE_ERROR);
  }
  //
  // install our shell command handlers
  //
  ShellCommandRegisterCommandName(L"rtcWakeup", ShellCommandRunRtcWakeUp, ShellCommandGetManFileNameLoongsonTest, 0, L"LoongsonTest", TRUE
      ,gUefiLoongsonTestHiiHandle, STRING_TOKEN(STR_LOONGSONTEST_HELP_WAKEUP));

  return (EFI_SUCCESS);
}

/**
  Destructor for the library.  free any resources.

  @param ImageHandle         The image handle of the process.
  @param SysTable            The EFI System Table pointer.
**/
EFI_STATUS
EFIAPI
LoongsonTestCommandLibDestructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SysTable
  )
{
  if (gUefiLoongsonTestHiiHandle != NULL) {
    HiiRemovePackages(gUefiLoongsonTestHiiHandle);
  }
  return (EFI_SUCCESS);
}

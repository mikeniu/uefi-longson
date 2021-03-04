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
#include "UefiVersCommandLib.h"

CONST CHAR16 gUefiVersFileName[] = L"ShellCommand";
EFI_HANDLE gUefiVersHiiHandle = NULL;

/**
  return the file name of the help text file if not using HII.

  @return The string pointer to the file name.
**/
CONST CHAR16*
EFIAPI
ShellCommandGetManFileNameVers (
  VOID
  )
{
  return (gUefiVersFileName);
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
VersCommandLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SysTable
  )
{
  gUefiVersHiiHandle = NULL;

  //
  // check our bit of the profiles mask
  //
  if ((PcdGet8(PcdShellProfileMask) & BIT4) == 0) {
    return (EFI_SUCCESS);
  }

  gUefiVersHiiHandle = HiiAddPackages (&gUefiVersHiiGuid, ImageHandle, UefiVersCommandLibStrings, NULL);
  if (gUefiVersHiiHandle == NULL) {
    return (EFI_DEVICE_ERROR);
  }
  //
  // install our shell command handlers
  //
  ShellCommandRegisterCommandName(L"vers", ShellCommandRunVers, ShellCommandGetManFileNameVers, 0, L"vers", TRUE , gUefiVersHiiHandle, STRING_TOKEN(STR_GET_HELP_VERS));

  return (EFI_SUCCESS);
}

/**
  Destructor for the library.  free any resources.

  @param ImageHandle         The image handle of the process.
  @param SysTable            The EFI System Table pointer.
**/
EFI_STATUS
EFIAPI
VersCommandLibDestructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SysTable
  )
{
  if (gUefiVersHiiHandle != NULL) {
    HiiRemovePackages(gUefiVersHiiHandle);
  }
  return (EFI_SUCCESS);
}

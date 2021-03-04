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

#include "UefiShellLevel2CommandsLib.h"
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <LoadRawFileProtocol.h>

STATIC CONST SHELL_PARAM_ITEM LoadParamList[] = {
  {L"-nc", TypeFlag},
  {NULL, TypeMax}
  };

VOID *
memset (
  OUT VOID    *Dest,
  IN  int     Char,
  IN  UINTN   Count
  )
{
  volatile UINT8  *Ptr;

  for (Ptr = Dest; Count > 0; Count--, Ptr++) {
    *Ptr = (UINT8) Char;
  }

  return Dest;
}
/**
  Function for 'run' command.

  @param[in] ImageHandle  Handle to the Image (NULL if Internal).
  @param[in] SystemTable  Pointer to the System Table (NULL if Internal).
**/
SHELL_STATUS
EFIAPI
ShellCommandRunImage (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_LOAD_RAW_FILE_PROTOCOL    *LoadRawFileHandle;
  EFI_DEVICE_PATH_PROTOCOL      *FilePath;
  EFI_DEVICE_PATH_PROTOCOL      *DevicePath;
  EFI_SHELL_FILE_INFO           *ListHead;
  EFI_SHELL_FILE_INFO           *Node;
  EFI_STATUS                    Status;
  SHELL_STATUS                  ShellStatus;
  LIST_ENTRY                    *Package;
  EFI_HANDLE                    *Handle;
  CHAR16                        *ProblemParam;
  CHAR16                        Parameters[200];
  VOID                          *FileBuffer;
  UINTN                         ParamCount;
  UINTN                         Filesize;
  UINTN                         Handlesize;
  UINTN                         Index;
  INTN                          Length;
  INTN                          Tmp_len;

  ListHead      = NULL;
  ProblemParam  = NULL;
  ShellStatus   = SHELL_SUCCESS;

  Handlesize    = 0;
  Length        = 0;
  Tmp_len       = 0;

  DEBUG((EFI_D_ERROR, "ShellCommandRunImage address is  0x%11p. \n",(VOID *)ShellCommandRunImage));
  memset((VOID*)Parameters, 0, 200);
  //
  // initialize the shell lib (we must be in non-auto-init...)
  //
  Status = ShellInitialize();
  ASSERT_EFI_ERROR(Status);
  Status = CommandInit();
  Status = gBS->LocateProtocol(&gEfiLoadRawFileProtocolGuid, NULL, (VOID **)&LoadRawFileHandle);
  if (EFI_ERROR(Status) || (LoadRawFileHandle == NULL))
  {
    DEBUG((EFI_D_ERROR, "cannot find LoadRawFile protocol. \n"));
    return SHELL_NOT_READY;
  }
  //
  // parse the command line
  //
  Status = ShellCommandLineParse (LoadParamList, &Package, &ProblemParam, TRUE);
  if (EFI_ERROR(Status)) {
    if (Status == EFI_VOLUME_CORRUPTED && ProblemParam != NULL) {
      DEBUG((EFI_D_ERROR, "EFI_VOLUME_CORRUPTED. \n"));
      ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM), gShellLevel2HiiHandle, L"load", ProblemParam);
      FreePool(ProblemParam);
      ShellStatus = SHELL_INVALID_PARAMETER;
    } else {
      ASSERT(FALSE);
    }
  } else {
    //
    // check for "-?"
    //
    if (ShellCommandLineGetFlag(Package, L"-?")) {
      DEBUG((EFI_D_ERROR, "string  L'-?' caputured!!! \n"));
      ASSERT(FALSE);
    } else if (ShellCommandLineGetRawValue(Package, 1) == NULL) {
      //
      // we didnt get a single file to load parameter
      //
      DEBUG((EFI_D_ERROR, "no paramter found. \n"));
      ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_TOO_FEW), gShellLevel2HiiHandle, L"load");
      ShellStatus = SHELL_INVALID_PARAMETER;
    } else {
      for ( ParamCount = 1
          ; ShellCommandLineGetRawValue(Package, ParamCount) != NULL
          ; ParamCount++)
          {
             DEBUG((EFI_D_ERROR, "ParamCount %d. \n", ParamCount));
             Status = ShellOpenFileMetaArg((CHAR16*)ShellCommandLineGetRawValue(Package, ParamCount), EFI_FILE_MODE_READ, &ListHead);
             if(!EFI_ERROR(Status))
             {
               Node = (EFI_SHELL_FILE_INFO *)GetFirstNode(&ListHead->Link);
               DEBUG((EFI_D_ERROR, "Node->FullName: %s. \n", Node->FullName));
             }
             if ((!EFI_ERROR(Status)) && (ParamCount == 1)) {
                // load kernal file first
                Node = (EFI_SHELL_FILE_INFO *)GetFirstNode(&ListHead->Link);
                FilePath = gEfiShellProtocol->GetDevicePathFromFilePath(Node->FullName);

                if (FilePath == NULL) {
                  ASSERT(FALSE);
                  return (SHELL_INVALID_PARAMETER);
                }
                Status = gBS->LocateHandleBuffer (ByProtocol, &gEfiSimpleFileSystemProtocolGuid, NULL, (UINTN *)&Handlesize, &Handle);
                for(Index = 0; Index < Handlesize; Index++){
                  Status = gBS->HandleProtocol (Handle[Index],&gEfiDevicePathProtocolGuid,(VOID *) &DevicePath);
                  if(!EFI_ERROR(Status))
                  {
                    Status = LoadRawFileHandle->LoadRawFile(LoadRawFileHandle, Handle[Index], FALSE, (CHAR16 *)Node->FullName, (VOID *)&FileBuffer, &Filesize);
                    DEBUG((EFI_D_ERROR, "LoadRawFileHandle->LoadRawFile. status is %r\n", Status));
                    if(!EFI_ERROR(Status))
                      break;
                  }
                }
                Status = ShellCloseFileMetaArg(&ListHead);
            } else {
                 if(ParamCount == 1){
                   ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_FILE_NF), gShellLevel2HiiHandle, L"run", (CHAR16*)ShellCommandLineGetRawValue(Package, ParamCount));
                   ShellStatus = SHELL_NOT_FOUND;
                 }else{
                   Tmp_len = StrLen((CHAR16*)ShellCommandLineGetRawValue(Package, ParamCount));
                   StrnCpy((Parameters + Length), (CHAR16*)ShellCommandLineGetRawValue(Package, ParamCount), Tmp_len);
                   Length += Tmp_len;
                   DEBUG((EFI_D_ERROR, "Length is %d\n", Length));
                   Parameters[Length] = ' ';
                   Length++;
                 }
                 Status = ShellCloseFileMetaArg(&ListHead);
             }
         } // end for
      //run kernal with params
      DEBUG((EFI_D_ERROR, "Parameters is %s\n", Parameters));
      Status = LoadRawFileHandle->RunRawFile(LoadRawFileHandle, FileBuffer, (CHAR16 *)Parameters);
      DEBUG((EFI_D_ERROR, "LoadRawFileHandle->RunRawFile. status is %r\n", Status));
    }
    //
    // free the command line package
    //
    ShellCommandLineFreeVarList (Package);
  }

  if (EFI_ERROR(Status) && ShellStatus == SHELL_SUCCESS) {
    ShellStatus = SHELL_ABORTED;
  }
  DEBUG((EFI_D_ERROR, "run over. \n"));
  return (ShellStatus);
}

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

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/ShellParameters.h>
#include <Protocol/Shell.h>
#include <Guid/FileInfo.h>
#include <Cpu.h>

UINTN  Argc;
CHAR16 **Argv;
EFI_SHELL_PROTOCOL      *mShellProtocolCap = NULL;

/**

  This function parse application ARG.

  @return Status
**/
EFI_STATUS
GetArg (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_SHELL_PARAMETERS_PROTOCOL *ShellParameters;

  Status = gBS->HandleProtocol (
                  gImageHandle,
                  &gEfiShellParametersProtocolGuid,
                  (VOID**)&ShellParameters
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Argc = ShellParameters->Argc;
  Argv = ShellParameters->Argv;
  return EFI_SUCCESS;
}

/**
  Get shell protocol.

  @return Pointer to shell protocol.
**/
EFI_SHELL_PROTOCOL *
GetShellProtocol (
  VOID
  )
{
  EFI_STATUS            Status;

  if (mShellProtocolCap == NULL) {
    Status = gBS->LocateProtocol (
                    &gEfiShellProtocolGuid,
                    NULL,
                    (VOID **) &mShellProtocolCap
                    );
    if (EFI_ERROR (Status)) {
      mShellProtocolCap = NULL;
    }
  }

  return mShellProtocolCap;
}

/**
  Read a file.

  @param[in]  FileName        The file to be read.
  @param[out] BufferSize      The file buffer size
  @param[out] Buffer          The file buffer

  @retval EFI_SUCCESS    Read file successfully
  @retval EFI_NOT_FOUND  Shell protocol or file not found
  @retval others         Read file failed
**/
EFI_STATUS
ReadFileToBuffer (
  IN  CHAR16                               *FileName,
  OUT UINTN                                *BufferSize,
  OUT VOID                                 **Buffer
  )
{
  EFI_STATUS                        Status;
  EFI_SHELL_PROTOCOL                *ShellProtocol;
  SHELL_FILE_HANDLE                 Handle;
  UINT64                            FileSize;
  UINTN                             TempBufferSize;
  VOID                              *TempBuffer;

  ShellProtocol = GetShellProtocol();
  if (ShellProtocol == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Open file by FileName.
  //
  Status = ShellProtocol->OpenFileByName (
                            FileName,
                            &Handle,
                            EFI_FILE_MODE_READ
                            );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Get the file size.
  //
  Status = ShellProtocol->GetFileSize (Handle, &FileSize);
  if (EFI_ERROR (Status)) {
    ShellProtocol->CloseFile (Handle);
    return Status;
  }

  TempBufferSize = (UINTN) FileSize;
  // TempBuffer = AllocateZeroPool (TempBufferSize);
  // TempBuffer =(VOID *)CACHE2UNCACHE(TempBuffer);
  TempBuffer =(VOID *)PHYS_TO_UNCACHED(0xf1100000);
  TempBuffer = ZeroMem (TempBuffer,TempBufferSize);

  if (TempBuffer == NULL) {
    ShellProtocol->CloseFile (Handle);
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Read the file data to the buffer
  //
  Status = ShellProtocol->ReadFile (
                            Handle,
                            &TempBufferSize,
                            TempBuffer
                            );
  if (EFI_ERROR (Status)) {
    ShellProtocol->CloseFile (Handle);
    return Status;
  }

  ShellProtocol->CloseFile (Handle);

  *BufferSize = TempBufferSize;
  *Buffer     = TempBuffer;
  return EFI_SUCCESS;
}

/**
  Write a file.

  @param[in] FileName        The file to be written.
  @param[in] BufferSize      The file buffer size
  @param[in] Buffer          The file buffer

  @retval EFI_SUCCESS    Write file successfully
  @retval EFI_NOT_FOUND  Shell protocol not found
  @retval others         Write file failed
**/
EFI_STATUS
WriteFileFromBuffer (
  IN  CHAR16                               *FileName,
  IN  UINTN                                BufferSize,
  IN  VOID                                 *Buffer
  )
{
  EFI_STATUS                        Status;
  EFI_SHELL_PROTOCOL                *ShellProtocol;
  SHELL_FILE_HANDLE                 Handle;
  EFI_FILE_INFO                     *FileInfo;
  UINTN                             TempBufferSize;

  ShellProtocol = GetShellProtocol();
  if (ShellProtocol == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Open file by FileName.
  //
  Status = ShellProtocol->OpenFileByName (
                            FileName,
                            &Handle,
                            EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE
                            );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Empty the file contents.
  //
  FileInfo = ShellProtocol->GetFileInfo (Handle);
  if (FileInfo == NULL) {
    ShellProtocol->CloseFile (Handle);
    return EFI_DEVICE_ERROR;
  }

  //
  // If the file size is already 0, then it has been empty.
  //
  if (FileInfo->FileSize != 0) {
    //
    // Set the file size to 0.
    //
    FileInfo->FileSize = 0;
    Status = ShellProtocol->SetFileInfo (Handle, FileInfo);
    if (EFI_ERROR (Status)) {
      FreePool (FileInfo);
      ShellProtocol->CloseFile (Handle);
      return Status;
    }
  }
  FreePool (FileInfo);

  //
  // Write the file data from the buffer
  //
  TempBufferSize = BufferSize;
  Status = ShellProtocol->WriteFile (
                            Handle,
                            &TempBufferSize,
                            Buffer
                            );
  if (EFI_ERROR (Status)) {
    ShellProtocol->CloseFile (Handle);
    return Status;
  }

  ShellProtocol->CloseFile (Handle);

  return EFI_SUCCESS;
}


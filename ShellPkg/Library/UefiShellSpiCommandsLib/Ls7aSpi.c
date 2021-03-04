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
#include "UefiShellSpiCommandsLib.h"
#include "XhciSpi.h"
#include <Library/ShellLib.h>
#include <Protocol/LsServiceDxe.h>
#include <Library/LoadFile.h>

STATIC CONST SHELL_PARAM_ITEM ParamList[] = {
  {L"-r", TypeFlag},
  {L"-e", TypeMax},
  {L"-w", TypeFlag},
  {L"-u", TypeValue},
  {L"-x", TypeValue},
  {L"-t", TypeValue},
  {L"-l", TypeFlag},
  {NULL, TypeMax}
};

SHELL_STATUS
EFIAPI
ShellCommandRunSpi (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS               Status;
  LIST_ENTRY               *Package;
  CHAR16                   *ProblemParam;
  SHELL_STATUS             ShellStatus;
  UINT8                    *Buffer;
  UINT8                    *TempBuffer;
  CONST CHAR16             *Lang;
  UINT32                   BufferSize;
  UINTN                    Offset;
  EFI_LS_SERVICE_PROTOCOL  *LsService = NULL;

  ShellStatus = SHELL_SUCCESS;
  Status      = EFI_SUCCESS;
  Buffer      = NULL;

  //
  // initialize the shell lib (we must be in non-auto-init...)
  //
  Status = ShellInitialize();
  ASSERT_EFI_ERROR(Status);

  Status = CommandInit();
  ASSERT_EFI_ERROR(Status);

  Status = gBS->LocateProtocol (
                  &gEfiLsServiceProtocolGuid,
                  NULL,
                  (void **)&LsService
                  );
  ASSERT_EFI_ERROR(Status);
  ASSERT(LsService != NULL);
  //
  // parse the command line
  //
  Status = ShellCommandLineParse (ParamList, &Package, &ProblemParam, TRUE);

  if (EFI_ERROR(Status)) {
    if (Status == EFI_VOLUME_CORRUPTED && ProblemParam != NULL) {
      ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM), gShellSpiHiiHandle, L"spi", ProblemParam);
      FreePool(ProblemParam);
      ShellStatus = SHELL_INVALID_PARAMETER;
    } else {
      ASSERT(FALSE);
    }
  } else {
    if (ShellCommandLineGetCount(Package) > 5) {
      ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_TOO_MANY), gShellSpiHiiHandle, L"spi");

      ShellCommandLineFreeVarList (Package);
      return (SHELL_INVALID_PARAMETER);
    }
    if (ShellCommandLineGetFlag(Package, L"-w")) {
      if (ShellCommandLineGetCount(Package) == 2) {
        Lang = ShellCommandLineGetRawValue(Package, 1);
        BufferSize = StrSize(Lang) / 2;
        Buffer = AllocateZeroPool(BufferSize / 3);
        TempBuffer = Buffer;
        for (Offset = 0;Offset < BufferSize;Offset += 3) {
          *TempBuffer = StrHexToUintn (Lang + Offset);
          ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_VALUE), gShellSpiHiiHandle, *(TempBuffer++));
        }
        LsService->ChipsetSpi.Erase (&LsService->ChipsetSpi, EfiDataWidthUint8, (BufferSize / 3), 0);
        LsService->ChipsetSpi.Write (&LsService->ChipsetSpi, EfiDataWidthUint8, (BufferSize / 3), 0, Buffer);
      } else if (ShellCommandLineGetCount(Package) == 3) {
        UINTN StartAddr =StrHexToUintn((CHAR16*)ShellCommandLineGetRawValue(Package, 1));
        CONST CHAR16 *Temp=ShellCommandLineGetRawValue(Package, 2);
        BufferSize = StrSize(Temp) / 2;
        Buffer = AllocateZeroPool(BufferSize / 3);
        TempBuffer = Buffer;
        for (Offset = 0;Offset < BufferSize;Offset += 3) {
          *TempBuffer = StrHexToUintn (Temp + Offset);
          ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_VALUE), gShellSpiHiiHandle, *(TempBuffer++));
        }
        if ((StartAddr+(BufferSize/3))>0x64) {
          ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_INVALID), gShellSpiHiiHandle, L"spi");
          ShellCommandLineFreeVarList (Package);
          if (Buffer != NULL) {
            FreePool (Buffer);
          }
          return (SHELL_INVALID_PARAMETER);
        }
        LsService->ChipsetSpi.Erase (&LsService->ChipsetSpi, EfiDataWidthUint8, (BufferSize / 3), StartAddr);
        LsService->ChipsetSpi.Write (&LsService->ChipsetSpi, EfiDataWidthUint8, (BufferSize / 3), StartAddr, Buffer);
      }
      else {
          ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_INVALID), gShellSpiHiiHandle, L"spi");
          ShellCommandLineFreeVarList (Package);
          return (SHELL_INVALID_PARAMETER);
      }

      if (Buffer != NULL) {
        FreePool (Buffer);
      }
    } else if (ShellCommandLineGetFlag(Package, L"-r")) {
      UINTN StartAddr=0,Size =0;
      UINT8 *Buf=NULL;
      UINTN Loop;
      if (ShellCommandLineGetCount(Package) != 3) {
        ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_INVALID), gShellSpiHiiHandle, L"spi");
        ShellCommandLineFreeVarList (Package);
        return (SHELL_INVALID_PARAMETER);
      }

      StartAddr = StrHexToUintn((CHAR16*)ShellCommandLineGetRawValue(Package, 1));
      Size = StrHexToUintn((CHAR16*)ShellCommandLineGetRawValue(Package, 2));
      if (Size > 1024*1024) {
        Print(L"%a: Size needs to be less than 1MB,Now Size 0x%x(%lldMB)\n", __func__,Size,(Size>>20));
        return (SHELL_BAD_BUFFER_SIZE);
      }

      Buf = AllocateZeroPool(Size);
      if (Buf == NULL) {
        DEBUG ((EFI_D_ERROR, "%a: failed to allocate Buf Size 0x%x\n", __func__,Size));
        return (SHELL_BAD_BUFFER_SIZE);
      }
      LsService->ChipsetSpi.Read (&LsService->ChipsetSpi, EfiDataWidthUint8, Size, StartAddr, Buf);
      for (Loop = 0;Loop < Size;Loop++){
        Print(L"%02x ",Buf[Loop]);
        if(((Loop+1)%16) == 0)
          Print(L"\n");
      }
      Print(L"\n");

      if (Buf != NULL) {
        FreePool (Buf);
      }
    } else if (ShellCommandLineGetFlag(Package, L"-u")) {

      EFI_SHELL_FILE_INFO           *ListHead;
      EFI_SHELL_FILE_INFO           *Node;
      EFI_HANDLE                    *Handle;
      VOID                          *FileBuffer;
      EFI_DEVICE_PATH_PROTOCOL      *DevicePath;
      UINTN Filesize;
      UINTN Handlesize;
      UINTN Index;
      UINTN Pages = 0;

      ListHead            = NULL;

      Lang = ShellCommandLineGetValue(Package, L"-u");

      Status = ShellOpenFileMetaArg((CHAR16*)Lang, EFI_FILE_MODE_READ, &ListHead);
      if (!EFI_ERROR(Status)) {
        Node = (EFI_SHELL_FILE_INFO *)GetFirstNode(&ListHead->Link);
        DEBUG((EFI_D_WARN, "Node->FullName: %s.  Handlesize = %d\n", Node->FullName,Handlesize));

        Status = gBS->LocateHandleBuffer (ByProtocol, &gEfiSimpleFileSystemProtocolGuid, NULL, (UINTN *)&Handlesize, &Handle);

        for(Index = 0; Index < Handlesize; Index++){
          Status = gBS->HandleProtocol (Handle[Index],&gEfiDevicePathProtocolGuid,(VOID *) &DevicePath);
          if(!EFI_ERROR(Status))
          {
            Status = LoadFile(Handle[Index], (CHAR16 *)Node->FullName, (VOID *)&FileBuffer, &Filesize);
            DEBUG((EFI_D_WARN, "Load File done.Filesize is = 0x%x\n",Filesize));
            if(!EFI_ERROR(Status))
              break;
          }
        }
        Status = ShellCloseFileMetaArg(&ListHead);
      } else {
        DEBUG((EFI_D_ERROR, "ShellOpenFileMetaArg error.\n"));
        return SHELL_LOAD_ERROR;
      }

      ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_STRING), gShellSpiHiiHandle, L"spi");
      LsService->CpuSpi.UpDate(&LsService->CpuSpi, EfiDataWidthUint8, Filesize, 0, FileBuffer);

     if (FileBuffer != NULL) {
        Pages = Filesize % 0x1000 == 0? Filesize / 0x1000 : Filesize / 0x1000 + 1;
        if ( Filesize < 0x1000){
          FreePool (FileBuffer);
        } else {
          FreePages (FileBuffer, Pages);
        }
        FileBuffer = NULL;
      }
    } else if (ShellCommandLineGetFlag(Package, L"-e")) {
      LsService->ChipsetSpi.Erase(&LsService->ChipsetSpi, EfiDataWidthUint8, 0x1000, 0);
    } else if (ShellCommandLineGetFlag(Package, L"-x")) {
      if (UsbSpiInit() == SHELL_SUCCESS)
        ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_XHCI_SUCCESS), gShellSpiHiiHandle, L"spi");
      else
        ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_XHCI_ERROR), gShellSpiHiiHandle, L"spi");
    } else if (ShellCommandLineGetFlag(Package, L"-t")) {
      UsbSpiErase();
    } else if (ShellCommandLineGetFlag(Package, L"-l")) {
      EFI_SHELL_FILE_INFO           *ListHead;
      EFI_SHELL_FILE_INFO           *Node;
      EFI_HANDLE                    *Handle;
      VOID                          *FileBuffer;
      EFI_DEVICE_PATH_PROTOCOL      *DevicePath;
      UINTN Filesize;
      UINTN Handlesize;
      UINTN Index;

      UINTN               ParamCount;
      UINTN               StartAddr = 0,Size = 0,ResultSize = 0 ;
      ListHead            = NULL;
      if (ShellCommandLineGetCount(Package) != 4) {
        ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_INVALID), gShellSpiHiiHandle, L"spi");
        ShellCommandLineFreeVarList (Package);
        return (SHELL_INVALID_PARAMETER);
      }

      for ( ParamCount = 1
          ; ShellCommandLineGetRawValue(Package, ParamCount) != NULL
          ; ParamCount++
         ){
        DEBUG((EFI_D_ERROR, "ParamCount %d. \n", ParamCount));
        Status = ShellOpenFileMetaArg((CHAR16*)ShellCommandLineGetRawValue(Package, ParamCount), EFI_FILE_MODE_READ, &ListHead);

        if ((!EFI_ERROR(Status)) && (ParamCount == 1)) {
          Node = (EFI_SHELL_FILE_INFO *)GetFirstNode(&ListHead->Link);
          DEBUG((EFI_D_WARN, "Node->FullName: %s. \n", Node->FullName));
          Status = gBS->LocateHandleBuffer (ByProtocol, &gEfiSimpleFileSystemProtocolGuid, NULL, (UINTN *)&Handlesize, &Handle);

          for(Index = 0; Index < Handlesize; Index++){
            Status = gBS->HandleProtocol (Handle[Index],&gEfiDevicePathProtocolGuid,(VOID *) &DevicePath);
            if(!EFI_ERROR(Status))
            {
              Status = LoadFile(Handle[Index],(CHAR16 *)Node->FullName, (VOID *)&FileBuffer, &Filesize);
              DEBUG((EFI_D_WARN, "Load File done.Filesize is = 0x%x\n",Filesize));
              if(!EFI_ERROR(Status))
                break;
            }
          }
          Status = ShellCloseFileMetaArg(&ListHead);
        } else {
          if(ParamCount == 2){
            StartAddr = StrHexToUintn((CHAR16*)ShellCommandLineGetRawValue(Package, ParamCount));
            DEBUG((EFI_D_WARN, "StarAdd = 0x%x %d\n",StartAddr,StartAddr));
            if(StartAddr > 0x100000){
              DEBUG((EFI_D_WARN, "StarAdd is more than 1M\n"));
              return (SHELL_INVALID_PARAMETER);
            }
          }else if(ParamCount == 3){
            Size = StrHexToUintn((CHAR16*)ShellCommandLineGetRawValue(Package, ParamCount));
            DEBUG((EFI_D_WARN, "Size = 0x%x %d\n",Size,Size));
            if(StartAddr + Size > 0x100000){
              DEBUG((EFI_D_WARN, "StarAdd + Size is more than 1M\n"));
              return (SHELL_INVALID_PARAMETER);
            }
          }
          Status = ShellCloseFileMetaArg(&ListHead);
        }
      } // end for
      if(Size < Filesize)
        ResultSize = Size;
      else
        ResultSize = Filesize;
      if(ResultSize > 0x100000){
        DEBUG((EFI_D_WARN, "ResultSize is more than 1M\n"));
        ResultSize = 0x100000;
      }
      LsService->ChipsetSpi.Erase (&LsService->ChipsetSpi, EfiDataWidthUint8, ResultSize, StartAddr);
      LsService->ChipsetSpi.Write(&LsService->ChipsetSpi, EfiDataWidthUint8, ResultSize, StartAddr, FileBuffer);
    } else {
      ASSERT(Buffer == NULL);
      ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_NO_VALUE), gShellSpiHiiHandle, L"spi",  L"-w");
      ShellCommandLineFreeVarList (Package);
      return (SHELL_INVALID_PARAMETER);
    }
  }

  return ShellStatus;
}

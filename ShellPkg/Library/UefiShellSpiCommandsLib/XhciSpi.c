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
#include <Library/LoadFile.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/PciIo.h>
#include "UefiShellSpiCommandsLib.h"
#include "XhciFirmware.h"
#include "XhciSpi.h"

UINT64 BaseAddr = 0;

UINT32
UsbSpiErase (
  VOID
  )
{

  UINT32 Tmp;

  Tmp = Pointer32(FWDCS);
  Tmp &= (1 << 31);
  if (!Tmp) {
    DEBUG((EFI_D_INFO,"no external rom exists!\n"));
    return SHELL_UNSUPPORTED;
  }
  Pointer32(DATA0) = 0x5a65726f;
  Pointer32(FWDCS) |= (1 << 17);

  do {
    Tmp = Pointer32(FWDCS);
  } while ((Tmp & (1 << 17)));

  DEBUG((EFI_D_INFO,"erase down.\n"));
  return SHELL_SUCCESS;
}

UINT32
UsbSpiPrepare (
  VOID
  )
{
  UINT32 Tmp;

  Tmp = Pointer32(FWDCS);
  Tmp &= (1 << 31);
  if (!Tmp) {
    DEBUG((EFI_D_INFO,"no external rom exists!\n"));
    return SHELL_UNSUPPORTED;
  }
  Pointer32(DATA0) = 0x53524f4d;
  Pointer32(FWDCS) |= (1 << 16);
  Tmp = Pointer32(FWDCS);
  Tmp &= (7 << 20);
  if (Tmp & (2 << 20)) {
    DEBUG((EFI_D_INFO,"Result code error %x\n",Tmp));
    return SHELL_UNSUPPORTED;
  }
  return SHELL_SUCCESS;
}

UINT32
UsbSpiWrite (
  INT32 Size,
  UINT32 *SpiBuffer
  )
{
  UINT32 *Buffer = SpiBuffer;
  UINT32 Tmp, Times = 10000000;

  if (UsbSpiPrepare() == SHELL_UNSUPPORTED)
    return SHELL_UNSUPPORTED;

  Size /= 2;
  if (Pointer32(FWDCS) & (1 << 24)) {
    DEBUG((EFI_D_INFO,"set data0 status error.\n"));
        return SHELL_UNSUPPORTED;
  }
  Pointer32(DATA0) = *Buffer++;
  if (Pointer32(FWDCS) & (2 << 24)) {
    DEBUG((EFI_D_INFO,"set data1 status error.\n"));
    return SHELL_UNSUPPORTED;
  }
  Pointer32(DATA1) = *Buffer++;
  Pointer32(FWDCS) |= (3 << 24);
  Size -= 1;
  while(Size--) {
    do {
      Tmp = Pointer32(FWDCS);
    } while (Tmp & (1 << 24));
    Pointer32(DATA0) = *Buffer++;
    Pointer32(FWDCS) |= (1 << 24);

    do {
      Tmp = Pointer32(FWDCS);
    } while (Tmp & (2 << 24));
    Pointer32(DATA1) = *Buffer++;
    Pointer32(FWDCS) |= (2 << 24);
  }
  do {
    Tmp = Pointer32(FWDCS);
  } while (Tmp & (2 << 24));

  Pointer32(FWDCS) &= ~(1 << 16);
  do {
    Tmp = Pointer32(FWDCS);
  } while (!(Tmp & (1 << 20)) && Times--);

  if (!(Tmp & (1 << 20))) {
    DEBUG((EFI_D_INFO,"write result code is %x\n",Tmp));
    return SHELL_UNSUPPORTED;
  }
  DEBUG((EFI_D_INFO,"Fw write done.\n"));
  return Size;
}

UINT32
UsbSpiRead (
  INT32 Size,
  UINT32 *ReturnBuffer
  )
{
  UINT32 Tmp;
  UINT32 *Buffur = ReturnBuffer;

  if (UsbSpiPrepare() == SHELL_UNSUPPORTED)
    return SHELL_UNSUPPORTED;

  Pointer32(FWDCS) |= (3 << 26);
  Size /= 2;

  while(Size--) {
    do {
      Tmp = Pointer32(FWDCS);
    } while ((Tmp & (1 << 26)));
    *Buffur++ = Pointer32(DATA0);
    Pointer32(FWDCS) |= (1 << 26);

#ifdef USBSPI_DEBUG
    Tmp = Pointer32(DATA0);
    DEBUG((EFI_D_INFO,"0x%08x ",Tmp));
#endif

    do {
      Tmp = Pointer32(FWDCS);
    } while ((Tmp & (2 << 26)));
    *Buffur++ = Pointer32(DATA1);
    Pointer32(FWDCS) |= (2 << 26);

#ifdef USBSPI_DEBUG
    Tmp = Pointer32(DATA1);
    DEBUG((EFI_D_INFO,"0x%08x ",Tmp));
    if (!(Size % 5))
      DEBUG((EFI_D_INFO,"\n"));
#endif
  }
  do {
    Tmp = Pointer32(FWDCS);
  } while (!(Tmp & (2 << 26)));
  Pointer32(FWDCS) &= ~(1 << 16);

  return Size;
}

UINT32
UsbSpiFirmwareWrite (
  INT32 Size,
  UINT32 *SpiBuffer
  )
{
  UINT32 *Buffer = SpiBuffer;
  UINT32 Tmp;
  //UINT32 Times = 10000;
  UINT32 Result;

  /*enable fw down */
  Pointer32(FWDCS) |= 0x1;

  Size /= 2;

  do {
    Tmp = Pointer32(FWDCS);
  } while (Tmp & (1 << 8));

  Pointer32(DATA0) = *Buffer++;

  //Times = 10000;
  do {
    Tmp = Pointer32(FWDCS);
  } while (Tmp & (1 << 9));

  Pointer32(DATA1) = *Buffer++;

  Pointer32(FWDCS) |= ((1 << 8) | (1 << 9)) ;

  Size -= 1;
  while(Size--) {
    do {
      Tmp = Pointer32(FWDCS);
    } while (Tmp & (1 << 8));
    Pointer32(DATA0) = *Buffer++;
    Pointer32(FWDCS) |= (1 << 8);

    do {
      Tmp = Pointer32(FWDCS);
    } while (Tmp & (1 << 9));
    Pointer32(DATA1) = *Buffer++;
    Pointer32(FWDCS) |= (1 << 9);
  }
  do {
    Tmp = Pointer32(FWDCS);
  } while (Tmp & (1 << 9));

  /*disable fw down */
  Pointer32(FWDCS) &= ~(1);
  while(Pointer32(FWDCS) & 1);

  Result = ((Pointer32(FWDCS) & 0x70) >> 4);

  return Result;

}

UINT32
UsbFirmwareDownload (
  VOID
  )
{
  INT32 Size;
  INT32 Retry;
  UINT32 Tmp;

  DEBUG((EFI_D_INFO,"n UsbFirmwareDownloadi!\n"));

  if (!(Pointer32(FWDCS) & (1 << 31))) {
    DEBUG((EFI_D_INFO,"no external rom exists!\n"));
    return SHELL_UNSUPPORTED;
  }

  for(Retry=0;Retry < 20; Retry ++){

    if (XhciPoniter32(0x20) & 1)
      XhciPoniter32(0x20) &= ~1;

      Tmp = XhciPoniter32(0x20);
      DEBUG((EFI_D_INFO,"- - -20h 0x%x \n",Tmp));

      Tmp = XhciPoniter32(0x24);
      DEBUG((EFI_D_INFO,"- - -24h 0x%x \n",Tmp));

    if(!(XhciPoniter32(0x24) & 1)){
      DEBUG((EFI_D_INFO,"Stop bit can not cleared to ‘0’ !\n"));
      continue;
    }

    if(!(XhciPoniter32(0x24) & (1 << 12))){
      DEBUG((EFI_D_INFO,"Have correct firmware in Serial ROM !\n"));
      return SHELL_SUCCESS;
    }

    Size = 3260;//FW Size

    Tmp=UsbSpiFirmwareWrite(Size,UsbSpiBuffer);
    switch (Tmp) {
      case 0:
        DEBUG((EFI_D_INFO,"Invalid (no result yet!\n"));
        break;
      case 1:
        DEBUG((EFI_D_INFO,"Success!\n"));
        return SHELL_SUCCESS;
      case 2:
        DEBUG((EFI_D_INFO,"Error!\n"));
        break;
      default:
        DEBUG((EFI_D_INFO,"unknown status!\n"));
        break;
    }
  }
  return SHELL_UNSUPPORTED;
}

/**
  This routine is use to check the XCHI controll is present.

  @param Base              Pointer to the XHCI controller base address.

  @retval SHELL_NOT_FOUND  XHCI controller device not be found.
  @retval SHELL_SUCCESS    XHCI controller device is found.

 **/
UINT32
XhciCtrlPresent(
  UINT64  *Base
  )
{
  EFI_HANDLE                *HandleArray;
  UINTN                     HandleArrayCount;
  UINTN                     Index;
  EFI_PCI_IO_PROTOCOL       *PciIo;
  PCI_TYPE00                Pci;
  UINTN                     Segment;
  UINTN                     Bus;
  UINTN                     Device;
  UINTN                     Function;
  EFI_STATUS                Status;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleArrayCount,
                  &HandleArray
                  );
  if (!EFI_ERROR (Status)) {
    for (Index = 0; Index < HandleArrayCount; Index++) {
      Status = gBS->HandleProtocol (
                      HandleArray[Index],
                      &gEfiPciIoProtocolGuid,
                      (VOID **)&PciIo
                      );
      if (!EFI_ERROR (Status)) {

        Status = PciIo->Pci.Read (
                      PciIo,
                      EfiPciIoWidthUint32,
                      0,
                      sizeof (Pci) / sizeof (UINT32),
                      &Pci
                      );
      if (IS_PCI_USB(&Pci)) {

          //
          // There is NEC Renesas xHCI uPD720201, VendorID is 0x1912, DeviceID is
          // 0x0014 in Loongson 7A devel board.
          //
          if (Pci.Hdr.VendorId == 0x1912 && Pci.Hdr.DeviceId == 0x0014) {
            PciIo->GetLocation (PciIo, &Segment, &Bus, &Device, &Function);
            // PHYS_TO_CACHED(0xefdff000000) is the 3A HT configure space type1 base address.
            *Base = (PHYS_TO_UNCACHED(0xefdff000000) | (Bus << 16) | (Device << 11) | (Function << 8));
            DEBUG((EFI_D_INFO,"Found XHCI controller seg %d bus %d dev %d func%d vendorid 0x%x\n",Segment, Bus, Device, Function,Pci.Hdr.VendorId));
            break;
          }
        }
      }
    }
    // If was ture there are not have XHCI controller.
    if (*Base == 0) {
      DEBUG((EFI_D_ERROR,"XHCI controller is not found!\n"));
      return SHELL_NOT_FOUND;
    }
  }
  return SHELL_SUCCESS;
}

UINT32
UsbSpiInit (
  VOID
  )
{
  INT32                     Size,Tmp;
  UINT32                    ReadBuffer[20];
  UINT64                    Base = 0;
  EFI_STATUS                Status;


  DEBUG((EFI_D_INFO,"in UsbSpiInit\n"));

  // Check whether the XHCI controller is present.
  Status = XhciCtrlPresent(&Base);
  if (!EFI_ERROR(Status))
    BaseAddr = Base;
  else
    return SHELL_NOT_FOUND;

  Size = UsbSpiRead(20,ReadBuffer);
  if (Size != -1) {
    DEBUG((EFI_D_INFO,"read error!!!\n"));
    return SHELL_UNSUPPORTED;
  }
  Size = 20;
  Tmp = 0;
  do {
    DebugPrint(EFI_D_INFO,"-> 0x%x\n",ReadBuffer[Tmp++]);
  } while(Size--);

  Size = 3260;//FW size
  if (UsbSpiWrite(Size, UsbSpiBuffer) == SHELL_UNSUPPORTED)
    return SHELL_UNSUPPORTED;
  else
    return SHELL_SUCCESS;
}

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
#ifndef _XHCISPI_H_
#define _XHCISPI_H_
#include <Cpu.h>

#define EXRIR   0xec
#define EXRCR   0xf0
#define FWDCS   0xf4
#define EXRACS  0xf6
#define DATA0   0xf8
#define DATA1   0xfc

extern UINT64 BaseAddr;
#define Pointer32(a) *(volatile UINT32*)(a + BaseAddr)
#define XhciBase ((Pointer32(0x10) & (~0xf)) | PHYS_TO_UNCACHED(0xe0040000000))
#define XhciPoniter32(a) (*(volatile UINT32*)(a + XhciBase))


UINT32
UsbSpiErase (
  VOID
  );

UINT32
UsbSpiPrepare (
  VOID
  );

UINT32
UsbSpiWrite (
  INT32 Size,
  UINT32 *SpiBuffer
  );

UINT32
UsbSpiRead (
  INT32 Size,
  UINT32 *ReturnBuffer
  );

UINT32
UsbSpiFirmwareWrite (
  INT32 Size,
  UINT32 *SpiBuffer
  );

UINT32
UsbFirmwareDownload (
  VOID
  );

UINT32
UsbSpiInit (
  VOID
  );
#endif

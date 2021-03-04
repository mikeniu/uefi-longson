
/** @file
  I/O Library for LOONGARCH.

  Copyright (c) 2020 Loongson Technology Corporation Limited (www.loongson.cn).
  All intellectual property rights(Copyright, Patent and Trademark) reserved.
  Copyright (c) 2006 - 2009, Intel Corporation. All rights reserved.<BR>
  Portions copyright (c) 2008 - 2009, Apple Inc. All rights reserved.<BR>
  Portions copyright (c) 2018, Loongson Inc. All rights reserved.<BR>

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

#include "BaseIoLibIntrinsicInternal.h"

/**
  Reads an 8-bit I/O port.

  Reads the 8-bit I/O port specified by Port. The 8-bit read value is returned.
  This function must guarantee that all I/O read and write operations are
  serialized.
  Because the LOONGARCH architecture use unified addressing, so we must map the IO
  address to memory address, and then it can accessing IO address rightly.

  If 8-bit I/O port operations are not supported, then ASSERT().

  @param  Port  The I/O port to read.

  @RETURN_OUT_OF_RESOURCES  Out of range of IO address.

  @return The value read.

**/
UINT8
EFIAPI
IoRead8 (
  IN      UINTN                     Port
  )
{
  UINTN Address, Range;
  Address = PcdGet64(PcdPciIoBase) + Port;
  Range = PcdGet64(PcdPciIoBase) + PcdGet64(PcdPciIoSize);

  ASSERT(Range > Address);

  return MmioRead8(Address);
}

/**
  Writes an 8-bit I/O port.

  Writes the 8-bit I/O port specified by Port with the value specified by Value
  and returns Value. This function must guarantee that all I/O read and write
  operations are serialized.
  Because the LOONGARCH architecture use unified addressing, so we must map the IO
  address to memory address, and then it can accessing IO address rightly.

  If 8-bit I/O port operations are not supported, then ASSERT().

  @param  Port  The I/O port to write.
  @param  Value The value to write to the I/O port.

  @RETURN_OUT_OF_RESOURCES  Out of range of IO address.

  @return The value written the I/O port.

**/
UINT8
EFIAPI
IoWrite8 (
  IN      UINTN                     Port,
  IN      UINT8                     Value
  )
{
  UINTN Address, Range;
  Address = PcdGet64(PcdPciIoBase) + Port;
  Range = PcdGet64(PcdPciIoBase) + PcdGet64(PcdPciIoSize);

  ASSERT(Range > Address);

  return MmioWrite8(Address, Value);
}

/**
  Reads a 16-bit I/O port.

  Reads the 16-bit I/O port specified by Port. The 16-bit read value is returned.
  This function must guarantee that all I/O read and write operations are
  serialized.
  Because the LOONGARCH architecture use unified addressing, so we must map the IO
  address to memory address, and then it can accessing IO address rightly.

  If 16-bit I/O port operations are not supported, then ASSERT().

  @param  Port  The I/O port to read.

  @RETURN_OUT_OF_RESOURCES  Out of range of IO address.

  @return The value read.

**/
UINT16
EFIAPI
IoRead16 (
  IN      UINTN                     Port
  )
{
  UINTN Address, Range;
  Address = PcdGet64(PcdPciIoBase) + Port;
  Range = PcdGet64(PcdPciIoBase) + PcdGet64(PcdPciIoSize);

  ASSERT(Range > Address);

  return MmioRead16(Address);
}

/**
  Writes a 16-bit I/O port.

  Writes the 16-bit I/O port specified by Port with the value specified by Value
  and returns Value. This function must guarantee that all I/O read and write
  operations are serialized.
  Because the LOONGARCH architecture use unified addressing, so we must map the IO
  address to memory address, and then it can accessing IO address rightly.

  If 16-bit I/O port operations are not supported, then ASSERT().

  @param  Port  The I/O port to write.
  @param  Value The value to write to the I/O port.

  @RETURN_OUT_OF_RESOURCES  Out of range of IO address.

  @return The value written the I/O port.

**/
UINT16
EFIAPI
IoWrite16 (
  IN      UINTN                     Port,
  IN      UINT16                    Value
  )
{
  UINTN Address, Range;
  Address = PcdGet64(PcdPciIoBase) + Port;
  Range = PcdGet64(PcdPciIoBase) + PcdGet64(PcdPciIoSize);

  ASSERT(Range > Address);

  return MmioWrite16(Address, Value);
}

/**
  Reads a 32-bit I/O port.

  Reads the 32-bit I/O port specified by Port. The 32-bit read value is returned.
  This function must guarantee that all I/O read and write operations are
  serialized.
  Because the LOONGARCH architecture use unified addressing, so we must map the IO
  address to memory address, and then it can accessing IO address rightly.

  If 32-bit I/O port operations are not supported, then ASSERT().

  @param  Port  The I/O port to read.

  @RETURN_OUT_OF_RESOURCES  Out of range of IO address.

  @return The value read.

**/
UINT32
EFIAPI
IoRead32 (
  IN      UINTN                     Port
  )
{
  UINTN Address, Range;
  Address = PcdGet64(PcdPciIoBase) + Port;
  Range = PcdGet64(PcdPciIoBase) + PcdGet64(PcdPciIoSize);

  ASSERT(Range > Address);

  return MmioRead32(Address);
}

/**
  Writes a 32-bit I/O port.

  Writes the 32-bit I/O port specified by Port with the value specified by Value
  and returns Value. This function must guarantee that all I/O read and write
  operations are serialized.
  Because the LOONGARCH architecture use unified addressing, so we must map the IO
  address to memory address, and then it can accessing IO address rightly.

  If 32-bit I/O port operations are not supported, then ASSERT().

  @param  Port  The I/O port to write.
  @param  Value The value to write to the I/O port.

  @RETURN_OUT_OF_RESOURCES  Out of range of IO address.

  @return The value written the I/O port.

**/
UINT32
EFIAPI
IoWrite32 (
  IN      UINTN                     Port,
  IN      UINT32                    Value
  )
{
  UINTN Address, Range;
  Address = PcdGet64(PcdPciIoBase) + Port;
  Range = PcdGet64(PcdPciIoBase) + PcdGet64(PcdPciIoSize);

  ASSERT(Range > Address);

  return MmioWrite32(Address, Value);
}

/**
  Reads a 64-bit I/O port.

  Reads the 64-bit I/O port specified by Port. The 64-bit read value is returned.
  This function must guarantee that all I/O read and write operations are
  serialized.
  Because the LOONGARCH architecture use unified addressing, so we must map the IO
  address to memory address, and then it can accessing IO address rightly.

  If 64-bit I/O port operations are not supported, then ASSERT().
  If Port is not aligned on a 64-bit boundary, then ASSERT().

  @param  Port  The I/O port to read.

  @RETURN_OUT_OF_RESOURCES  Out of range of IO address.

  @return The value read.

**/
UINT64
EFIAPI
IoRead64 (
  IN      UINTN                     Port
  )
{
  UINTN Address, Range;
  Address = PcdGet64(PcdPciIoBase) + Port;
  Range = PcdGet64(PcdPciIoBase) + PcdGet64(PcdPciIoSize);

  ASSERT(Range > Address);

  return MmioRead64(Address);
}

/**
  Writes a 64-bit I/O port.

  Writes the 64-bit I/O port specified by Port with the value specified by Value
  and returns Value. This function must guarantee that all I/O read and write
  operations are serialized.
  Because the LOONGARCH architecture use unified addressing, so we must map the IO
  address to memory address, and then it can accessing IO address rightly.

  If 64-bit I/O port operations are not supported, then ASSERT().
  If Port is not aligned on a 64-bit boundary, then ASSERT().

  @param  Port  The I/O port to write.
  @param  Value The value to write to the I/O port.

  @RETURN_OUT_OF_RESOURCES  Out of range of IO address.

  @return The value written to the I/O port.

**/
UINT64
EFIAPI
IoWrite64 (
  IN      UINTN                     Port,
  IN      UINT64                    Value
  )
{
  UINTN Address, Range;
  Address = PcdGet64(PcdPciIoBase) + Port;
  Range = PcdGet64(PcdPciIoBase) + PcdGet64(PcdPciIoSize);

  ASSERT(Range > Address);

  return MmioWrite64(Address, Value);
}


/**
  Reads an 8-bit MMIO register.

  Reads the 8-bit MMIO register specified by Address. The 8-bit read value is
  returned. This function must guarantee that all MMIO read and write
  operations are serialized.

  If 8-bit MMIO register operations are not supported, then ASSERT().

  @param  Address The MMIO register to read.


  @return The value read.

**/
UINT8
EFIAPI
MmioRead8 (
  IN      UINTN                     Address
  )
{
  UINT8                             Value;

  Value = *(volatile UINT8*)Address;
  return Value;
}

/**
  Writes an 8-bit MMIO register.

  Writes the 8-bit MMIO register specified by Address with the value specified
  by Value and returns Value. This function must guarantee that all MMIO read
  and write operations are serialized.

  If 8-bit MMIO register operations are not supported, then ASSERT().

  @param  Address The MMIO register to write.
  @param  Value   The value to write to the MMIO register.

**/
UINT8
EFIAPI
MmioWrite8 (
  IN      UINTN                     Address,
  IN      UINT8                     Value
  )
{
  *(volatile UINT8*)Address = Value;
  return Value;
}

/**
  Reads a 16-bit MMIO register.

  Reads the 16-bit MMIO register specified by Address. The 16-bit read value is
  returned. This function must guarantee that all MMIO read and write
  operations are serialized.

  If 16-bit MMIO register operations are not supported, then ASSERT().

  @param  Address The MMIO register to read.

  @return The value read.

**/
UINT16
EFIAPI
MmioRead16 (
  IN      UINTN                     Address
  )
{
  UINT16                            Value;

  ASSERT ((Address & 1) == 0);
  Value = *(volatile UINT16*)Address;
  return Value;
}

/**
  Writes a 16-bit MMIO register.

  Writes the 16-bit MMIO register specified by Address with the value specified
  by Value and returns Value. This function must guarantee that all MMIO read
  and write operations are serialized.

  If 16-bit MMIO register operations are not supported, then ASSERT().

  @param  Address The MMIO register to write.
  @param  Value   The value to write to the MMIO register.

**/
UINT16
EFIAPI
MmioWrite16 (
  IN      UINTN                     Address,
  IN      UINT16                    Value
  )
{
  ASSERT ((Address & 1) == 0);
  *(volatile UINT16*)Address = Value;
  return Value;
}

/**
  Reads a 32-bit MMIO register.

  Reads the 32-bit MMIO register specified by Address. The 32-bit read value is
  returned. This function must guarantee that all MMIO read and write
  operations are serialized.

  If 32-bit MMIO register operations are not supported, then ASSERT().

  @param  Address The MMIO register to read.

  @return The value read.

**/
UINT32
EFIAPI
MmioRead32 (
  IN      UINTN                     Address
  )
{
  UINT32                            Value;

  ASSERT ((Address & 3) == 0);
  Value = *(volatile UINT32*)Address;
  return Value;
}

/**
  Writes a 32-bit MMIO register.

  Writes the 32-bit MMIO register specified by Address with the value specified
  by Value and returns Value. This function must guarantee that all MMIO read
  and write operations are serialized.

  If 32-bit MMIO register operations are not supported, then ASSERT().

  @param  Address The MMIO register to write.
  @param  Value   The value to write to the MMIO register.

**/
UINT32
EFIAPI
MmioWrite32 (
  IN      UINTN                     Address,
  IN      UINT32                    Value
  )
{
  ASSERT ((Address & 3) == 0);
  *(volatile UINT32*)Address = Value;
  return Value;
}

/**
  Reads a 64-bit MMIO register.

  Reads the 64-bit MMIO register specified by Address. The 64-bit read value is
  returned. This function must guarantee that all MMIO read and write
  operations are serialized.

  If 64-bit MMIO register operations are not supported, then ASSERT().

  @param  Address The MMIO register to read.

  @return The value read.

**/
UINT64
EFIAPI
MmioRead64 (
  IN      UINTN                     Address
  )
{
  UINT64                            Value;

  ASSERT ((Address & 7) == 0);
  Value = *(volatile UINT64*)Address;
  return Value;
}

/**
  Writes a 64-bit MMIO register.

  Writes the 64-bit MMIO register specified by Address with the value specified
  by Value and returns Value. This function must guarantee that all MMIO read
  and write operations are serialized.

  If 64-bit MMIO register operations are not supported, then ASSERT().

  @param  Address The MMIO register to write.
  @param  Value   The value to write to the MMIO register.

**/
UINT64
EFIAPI
MmioWrite64 (
  IN      UINTN                     Address,
  IN      UINT64                    Value
  )
{
  ASSERT ((Address & 7) == 0);
  *(volatile UINT64*)Address = Value;
  return Value;
}

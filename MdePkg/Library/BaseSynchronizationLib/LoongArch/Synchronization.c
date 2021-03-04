/** @file
  Implementation of synchronization functions on LOONGARCH.

  Copyright (c) 2020 Baoqi Zhang (zhangbaoqi@loongson.cn).
  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
  All intellectual property rights(Copyright, Patent and Trademark) reserved.
  Copyright (c) 2006 - 2010, Intel Corporation. All rights reserved.<BR>

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

/**
  Performs an atomic compare exchange operation on a 16-bit
  unsigned integer.

  Performs an atomic compare exchange operation on the 16-bit
  unsigned integer specified by Value.  If Value is equal to
  CompareValue, then Value is set to ExchangeValue and
  CompareValue is returned.  If Value is not equal to
  CompareValue, then Value is returned. The compare exchange
  operation must be performed using MP safe mechanisms.

  @param  Value         A pointer to the 16-bit value for the
                        compare exchange operation.
  @param  CompareValue  16-bit value used in compare operation.
  @param  ExchangeValue 16-bit value used in exchange operation.

  @return The original *Value before exchange.

**/
UINT16
EFIAPI
InternalSyncCompareExchange16 (
  IN      volatile UINT16           *Value,
  IN      UINT16                    CompareValue,
  IN      UINT16                    ExchangeValue
  )
{
  return *Value != CompareValue ? *Value :
           ((*Value = ExchangeValue), CompareValue);
}

/**
  Performs an atomic compare exchange operation on a 32-bit
  unsigned integer.

  Performs an atomic compare exchange operation on the 32-bit
  unsigned integer specified by Value.  If Value is equal to
  CompareValue, then Value is set to ExchangeValue and
  CompareValue is returned.  If Value is not equal to
  CompareValue, then Value is returned. The compare exchange
  operation must be performed using MP safe mechanisms.

  @param  Value         A pointer to the 32-bit value for the
                        compare exchange operation.
  @param  CompareValue  32-bit value used in compare operation.
  @param  ExchangeValue 32-bit value used in exchange operation.

  @return The original *Value before exchange.

**/
UINT32
EFIAPI
InternalSyncCompareExchange32 (
  IN      volatile UINT32           *Value,
  IN      UINT32                    CompareValue,
  IN      UINT32                    ExchangeValue
  )
{
  return *Value != CompareValue ? *Value :
           ((*Value = ExchangeValue), CompareValue);
}

/**
  Performs an atomic compare exchange operation on a 64-bit unsigned integer.

  Performs an atomic compare exchange operation on the 64-bit unsigned integer specified
  by Value.  If Value is equal to CompareValue, then Value is set to ExchangeValue and
  CompareValue is returned.  If Value is not equal to CompareValue, then Value is returned.
  The compare exchange operation must be performed using MP safe mechanisms.

  @param  Value         A pointer to the 64-bit value for the compare exchange
                        operation.
  @param  CompareValue  64-bit value used in compare operation.
  @param  ExchangeValue 64-bit value used in exchange operation.

  @return The original *Value before exchange.

**/
UINT64
EFIAPI
InternalSyncCompareExchange64 (
  IN      volatile UINT64           *Value,
  IN      UINT64                    CompareValue,
  IN      UINT64                    ExchangeValue
  )
{
  return *Value != CompareValue ? *Value :
           ((*Value = ExchangeValue), CompareValue);
}

/**
  Performs an atomic increment of an 32-bit unsigned integer.

  Performs an atomic increment of the 32-bit unsigned integer specified by
  Value and returns the incremented value. The increment operation must be
  performed using MP safe mechanisms. The state of the return value is not
  guaranteed to be MP safe.

  @param  Value A pointer to the 32-bit value to increment.

  @return The incremented value.

**/
UINT32
EFIAPI
InternalSyncIncrement (
  IN      volatile UINT32           *Value
  )
{
  int temp;
  do {
    __asm__ __volatile__(
      "ll.d  %0, %1    \n"
      "addi.d  %0, %0,  1 \n"
      "sc.d  %0, %1    \n"
      :"=&r" (temp), "+m" (*Value)
    );
  }while (!temp);
  return *Value;
}

/**
  Performs an atomic decrement of an 32-bit unsigned integer.

  Performs an atomic decrement of the 32-bit unsigned integer specified by
  Value and returns the decrement value. The decrement operation must be
  performed using MP safe mechanisms. The state of the return value is not
  guaranteed to be MP safe.

  @param  Value A pointer to the 32-bit value to decrement.

  @return The decrement value.

**/
UINT32
EFIAPI
InternalSyncDecrement (
  IN      volatile UINT32           *Value
  )
{
  int temp;
  do {
    __asm__ __volatile__(
      "ll.d    %0, %1    \n"
      "addi.d  %0, %0, -1    \n"
      "sc.d    %0, %1   \n"
      :"=&r" (temp), "+m" (*Value)
    );
  }while (!temp);
  return *Value;
}

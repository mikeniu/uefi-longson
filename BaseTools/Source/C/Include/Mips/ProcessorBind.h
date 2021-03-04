/** @file
  Processor or Compiler specific defines and types for LOONGARCH.

  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
  All intellectual property rights(Copyright, Patent and Trademark) reserved.
  Copyright (c) 2006 - 2012, Intel Corporation. All rights reserved.<BR>
  Portions copyright (c) 2018, Loongson Inc. All rights reserved.<BR>

  Any violations of copyright or other intellectual property rights of the Loongson
  Technology Corporation Limited will be held accountable in accordance with the law,
  if you (or any of your subsidiaries, corporate affiliates or agents) initiate directly
  or indirectly any Intellectual Property Assertion or Intellectual Property Litigation:
  (i) against Loongson Technology Corporation Limited or any of its subsidiaries or corporate affiliates,
  (ii) against any party if such Intellectual Property Assertion or Intellectual Property Litigation arises
  in whole or in part from any software, technology, product or service of Loongson Technology Corporation Limited
  or any of its subsidiaries or corporate affiliates, or (iii) against any party relating to the Software.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION).

**/

#ifndef __PROCESSOR_BIND_H__
#define __PROCESSOR_BIND_H__

#define MDE_CPU_LOONGARCH64

#ifndef __GNUC__
#pragma pack()
#endif

#if _MSC_EXTENSIONS
  typedef unsigned __int64    UINT64;
  typedef __int64             INT64;
  typedef unsigned __int32    UINT32;
  typedef __int32             INT32;
  typedef unsigned short      UINT16;
  typedef unsigned short      CHAR16;
  typedef short               INT16;
  typedef unsigned char       BOOLEAN;
  typedef unsigned char       UINT8;
  typedef char                CHAR8;
  typedef signed char         INT8;
#else
  typedef unsigned long long  UINT64;
  typedef long long           INT64;
  typedef unsigned int        UINT32;
  typedef int                 INT32;
  typedef unsigned short      UINT16;
  typedef unsigned short      CHAR16;
  typedef short               INT16;
  typedef unsigned char       BOOLEAN;
  typedef unsigned char       UINT8;
  typedef char                CHAR8;
  typedef signed char         INT8;

  #define UINT8_MAX 0xFF
#endif

typedef UINT64  UINTN;

typedef INT64   INTN;

#define MAX_BIT      0x8000000000000000ULL

#define MAX_2_BITS   0xC000000000000000ULL

#define MAX_ADDRESS  0xFFFFFFFFFFFFFFFFULL

#define CPU_STACK_ALIGNMENT  sizeof(UINT64)

#define EFIAPI

#if defined(__GNUC__)
  #define ASM_GLOBAL .globl

  #define INTERWORK_FUNC(func__)   .type ASM_PFX(func__), %function

  #define GCC_ASM_EXPORT(func__)  \
        .global  _CONCATENATE (__USER_LABEL_PREFIX__, func__)    ;\
     .type ASM_PFX(func__), %function

  #define GCC_ASM_IMPORT(func__)  \
        .extern  _CONCATENATE (__USER_LABEL_PREFIX__, func__)

#endif

/**
  Return the pointer to the first instruction of a function given a function pointer.
  On LOONGARCH CPU architectures, these two pointer values are the same,
  so the implementation of this macro is very simple.

  @param  FunctionPointer   A pointer to a function.

  @return The pointer to the first instruction of a function given a function pointer.

**/
#define FUNCTION_ENTRY_POINT(FunctionPointer) (VOID *)(UINTN)(FunctionPointer)

#endif

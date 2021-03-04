/** @file
  Copyright (c) 2020 Baoqi Zhang (zhangbaoqi@loongson.cn).
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

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION).
**/

#ifndef __PROCESSOR_BIND_H__
#define __PROCESSOR_BIND_H__

//
//define process type for LOONGARCH
//
#define MDE_CPU_LOONGARCH

#define EFIAPI

#ifndef __GNUC__
#pragma pack()
#endif

//
// Assume standard LOONGARCH alignment.
// Need to check portability of long long
//
typedef unsigned long       UINT64;
typedef long                INT64;
typedef unsigned int        UINT32;
typedef int                 INT32;
typedef unsigned short      UINT16;
typedef unsigned short      CHAR16;
typedef short               INT16;
typedef unsigned char       BOOLEAN;
typedef unsigned char       UINT8;
typedef char                CHAR8;
typedef char                INT8;

#define zero	$r0	/* wired zero */
#define ra	$r1	/* return address */
#define gp	$r2	/* global pointer - caller saved for PIC */
#define sp	$r3	/* stack pointer */
#define v0	$r4	/* return value - caller saved */
#define v1	$r5
#define a0	$r4	/* argument registers */
#define a1	$r5
#define a2	$r6
#define a3	$r7
#define a4	$r8
#define a5	$r9
#define a6	$r10
#define a7	$r11
#define t0	$r12
#define t1	$r13
#define t2	$r14
#define t3	$r15
#define t4	$r16
#define t5	$r17
#define t6	$r18
#define t7	$r19
#define t8	$r20
#define tp	$r21
#define fp	$r22
#define s0	$r23
#define s1	$r24
#define s2	$r25
#define s3	$r26
#define s4	$r27
#define s5	$r28
#define s6	$r29
#define s7	$r30
#define s8	$r31

//
// Unsigned value of native width.  (4 bytes on supported 32-bit processor instructions,
// 8 bytes on supported 64-bit processor instructions)
//

typedef UINT64  UINTN;
#define DEFAULT_PAGE_ALLOCATION_GRANULARITY   (0x1000)
#define RUNTIME_PAGE_ALLOCATION_GRANULARITY   (0x1000)

//
// Signed value of native width.  (4 bytes on supported 32-bit processor instructions,
// 8 bytes on supported 64-bit processor instructions)
//
typedef INT64   INTN;

//
// Processor specific defines
//

//
// A value of native width with the highest bit set.
//
#define MAX_BIT     0x8000000000000000ULL
//
// A value of native width with the two highest bits set.
//
#define MAX_2_BITS  0xC000000000000000ULL

//
// Maximum legal LOONGARCH address
//
#define MAX_ADDRESS   0xFFFFFFFFFFFFFFFFULL

//
// Maximum legal LOONGARCH INTN and UINTN values.
//
#define MAX_INTN   ((INTN)0x7FFFFFFFFFFFFFFFULL)
#define MAX_UINTN  ((UINTN)0xFFFFFFFFFFFFFFFFULL)

//
// The stack alignment required for LOONGARCH
//
#define CPU_STACK_ALIGNMENT   sizeof(UINT64)

#define GLOBAL_REMOVE_IF_UNREFERENCED

/**
  Return the pointer to the first instruction of a function given a function pointer.
  On LOONGARCH CPU architectures, these two pointer values are the same,
  so the implementation of this macro is very simple.

  @param  FunctionPointer   A pointer to a function.

  @return The pointer to the first instruction of a function given a function pointer.

**/
#define FUNCTION_ENTRY_POINT(FunctionPointer) (VOID *)(UINTN)(FunctionPointer)

#endif

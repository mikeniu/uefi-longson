 /** @file
  Implementation of SetJump() and LongJump() on LOONGARCH.

  Copyright (c) 2020 Baoqi Zhang (zhangbaoqi@loongson.cn).
  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
  All intellectual property rights(Copyright, Patent and Trademark) reserved.
  Copyright (c) 2006 - 2008, Intel Corporation. All rights reserved.<BR>

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


#define STORE   st.d    /* 64 bit mode regsave instruction */
#define LOAD    ld.d    /* 64 bit mode regload instruction */
#define RSIZE   8       /* 64 bit mode register size */

    .globl InternalSwitchStackAsm

/**
  This allows the caller to switch the stack and goes to the new entry point

  @param  JumpBuffer    A pointer to CPU context buffer.
**/

#UINTN
#EFIAPI
#InternalSwitchStackAsm (
#  IN     BASE_LIBRARY_JUMP_BUFFER  *JumpBuffer
#  );

InternalSwitchStackAsm:
    LOAD    ra, a0, RSIZE *11
    LOAD    s0, a0, RSIZE * 0
    LOAD    s1, a0, RSIZE * 1
    LOAD    s2, a0, RSIZE * 2
    LOAD    s3, a0, RSIZE * 3
    LOAD    s4, a0, RSIZE * 4
    LOAD    s5, a0, RSIZE * 5
    LOAD    s6, a0, RSIZE * 6
    LOAD    s7, a0, RSIZE * 7
    LOAD    s8, a0, RSIZE * 8
    LOAD    sp, a0, RSIZE * 9
    LOAD    gp, a0, RSIZE *10
    LOAD    a0, sp, 0
    LOAD    a1, sp, 8
    jirl    zero, ra, 0
    .end


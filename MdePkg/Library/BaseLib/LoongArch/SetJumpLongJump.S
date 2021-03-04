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

    .globl SetJump
    .globl InternalLongJump

#/**
#  Saves the current CPU context that can be restored with a call to LongJump() and returns 0.#
#
#  Saves the current CPU context in the buffer specified by JumpBuffer and returns 0. The initial
#  call to SetJump() must always return 0. Subsequent calls to LongJump() cause a non-zero
#  value to be returned by SetJump().
#
#  If JumpBuffer is NULL, then ASSERT().
#
#  @param  JumpBuffer  A pointer to CPU context buffer.
#
#  @retval 0 Indicates a return from SetJump().
#
#**/
#
#UINTN
#EFIAPI
#SetJump (
#  OUT     BASE_LIBRARY_JUMP_BUFFER  *JumpBuffer
#  );

SetJump:
    STORE   s0, a0, RSIZE * 0
    STORE   s1, a0, RSIZE * 1
    STORE   s2, a0, RSIZE * 2
    STORE   s3, a0, RSIZE * 3
    STORE   s4, a0, RSIZE * 4
    STORE   s5, a0, RSIZE * 5
    STORE   s6, a0, RSIZE * 6
    STORE   s7, a0, RSIZE * 7
    STORE   s8, a0, RSIZE * 8
    STORE   sp, a0, RSIZE * 9
    STORE   gp, a0, RSIZE * 10
    STORE   ra, a0, RSIZE * 11
    li      v0, 0                    # Setjmp return
    jirl    zero, ra, 0

#/**
#  Restores the CPU context that was saved with SetJump().#
#
#  Restores the CPU context from the buffer specified by JumpBuffer.
#  This function never returns to the caller.
#  Instead is resumes execution based on the state of JumpBuffer.
#
#  @param  JumpBuffer    A pointer to CPU context buffer.
#  @param  Value         The value to return when the SetJump() context is restored.
#
#**/
#
#VOID
#EFIAPI
#InternalLongJump (
#  IN      BASE_LIBRARY_JUMP_BUFFER  *JumpBuffer,
#  IN      UINTN                     Value
#  );
#

InternalLongJump:
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
    move    a0, a1
    jirl    zero, ra, 0
    .end

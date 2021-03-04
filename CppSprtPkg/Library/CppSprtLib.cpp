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

extern"C"
{
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
}

#undef   NULL
#define  NULL  0

VOID *operator new(UINTN Size) throw()
{
  VOID       *RetVal;
  EFI_STATUS  Status;

  if (Size == 0) {
    return NULL;
  }

  Status = gBS->AllocatePool(EfiLoaderData, (UINTN)Size, &RetVal);
  if (Status != EFI_SUCCESS) {
    RetVal = NULL;
  }
  return RetVal;
}

VOID *operator new[](UINTN cb)
{
  VOID *res = operator new(cb);
  return res;
}

VOID operator delete(VOID *p)
{
  if (p != NULL) {
    (VOID)gBS->FreePool(p);
  }
}

VOID operator delete[](VOID *p)
{
    operator delete(p);
}

extern "C"
VOID _Unwind_Resume(struct _Unwind_Exception *object)
{

}

#include "CppCrt.cpp"

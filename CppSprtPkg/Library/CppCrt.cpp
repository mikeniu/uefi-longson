/*
## @file
#
#  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
#  All INT32ellectual property rights(Copyright, Patent and Trademark) reserved.
#
#  Any violations of copyright or other INT32ellectual property rights of the Loongson Technology
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
extern "C" VOID Print(CONST CHAR16 *, ...);

typedef VOID (*func_ptr)(VOID);
typedef INT32 (*_PVFV)(VOID);

_PVFV *atexits  = NULL;
INT32 num_atexit  = 0;
INT32 max_atexit  =-1;

extern "C" {VOID *__dso_handle = 0;}

//#ifdef USE_SIMPLE_STD
extern "C"
{
  INT32 __cxa_atexit(VOID (*handler)(VOID))
  {
    if (handler == NULL) {
      return 0;
    }
    if (num_atexit >= max_atexit) {
      max_atexit += 32;
      _PVFV* old_handler = atexits;
      atexits  = new _PVFV[max_atexit];
      if (atexits == NULL) {
        atexits = old_handler;
        return -1;
      }
      for(INT32 i = 0; i < max_atexit - 32; i++) {
        atexits[i] = old_handler[i];
      }
      delete old_handler;
    }
    atexits[num_atexit++] = (_PVFV)handler;
    return 0;
  }
}
//#endif

extern "C"
{
  VOID static _g_finit()
  {
    for (INT32 i = num_atexit - 1; i >= 0; i--) {
      if ( atexits[i] != NULL ) {
        (*atexits[i])();
      }
    }
  }
}

func_ptr __CTOR_LIST__[1]
__attribute__((__used__, section(".ctors"), aligned(sizeof(func_ptr))))
= { (func_ptr) (-1) };

func_ptr __CTOR_END__[1]
__attribute__((__used__, section(".ctors"), aligned(sizeof(func_ptr))))
= { (func_ptr) 0 };

func_ptr __DTOR_LIST__[1]
__attribute__((__used__, section(".dtors"), aligned(sizeof(func_ptr))))
= { (func_ptr) (-1) };

func_ptr __DTOR_END__[1]
__attribute__((__used__, section(".dtors"), aligned(sizeof(func_ptr))))
= { (func_ptr) 0 };

func_ptr __xp_finitz[]
__attribute__((__used__, section(".dtors"), aligned(sizeof(func_ptr))))
= { _g_finit };


extern "C"
{
  VOID __attribute__((used))
    __do_global_ctors_aux(VOID)
    {
      func_ptr *p =  __CTOR_END__ - 1;
      for (p = __CTOR_LIST__; p < __CTOR_END__ ; p++) {
        if (*p && *p != (func_ptr) - 1)
          (*p) ();
      }
#ifdef CPP_CRT_DEBUG
      {
        func_ptr *p =  __CTOR_END__ - 1;
        INT32 i = 0;
        for (i = -5; i< 5; i++) {
          Print((CONST CHAR16*)L"%2d: %p %p\n", i, &p[i], p[i]);

        }
        Print((CONST CHAR16*)L"[%p %p]\n", __CTOR_LIST__, __CTOR_END__);
      }
#endif
    }
}

extern "C"
{
  VOID __attribute__((used))
    __do_global_dtors_aux (VOID)
    {
      func_ptr *p =  __DTOR_END__ - 1;
      for (p = __DTOR_END__ - 1 ; p >= __DTOR_LIST__; p--) {
        if (*p && *p != (func_ptr) - 1) {
          (*p) ();
        }
      }
#ifdef CPP_CRT_DEBUG
      {
        func_ptr *p =  __DTOR_END__ - 1;
        INT32 i = 0;
        for (i = -5; i< 5; i++) {
          Print((CONST CHAR16*)L"%2d: %p %p\n", i,&p[i],  p[i]);

        }
        Print((CONST CHAR16*)L"[%p %p]\n", __DTOR_LIST__, __DTOR_END__);
      }
#endif
    }
}

INT32 CppMain(IN INT32 Argc,  IN CHAR8 **Argv);

INT32
EFIAPI
main (
  IN INT32 Argc,
  IN CHAR8 **Argv
  )
{
  INT32 Result;

  __do_global_ctors_aux ();

  Result = CppMain(Argc, Argv);

  __do_global_dtors_aux ();

  return Result;
}

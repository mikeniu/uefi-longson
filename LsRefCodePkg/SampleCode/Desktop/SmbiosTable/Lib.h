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
/*
 * Transplate from pmon
 * */
#ifndef _LIB_H_
#define _LIB_H_

#include <Cpu.h>
#include <LsHobTypes.h>
#include <Ppi/Ls3aCpu/Ls3aPlatformTablePpi.h>
//
//pmon code
//
typedef signed char          int8_t;
typedef unsigned char        u_int8_t;
typedef short                int16_t;
typedef unsigned short       u_int16_t;
typedef int                  int32_t;
typedef unsigned int         u_int32_t;
typedef long long            int64_t;
typedef unsigned long long   u_int64_t;
typedef long                 size_t;
extern char                  vers[];

#define SPD_INFO_ADDR PHYS_TO_UNCACHED(0x8fffa000)
#define FL_BUS_8  0x01  /* Byte wide bus */
#define FL_BUS_16 0x02  /* Short wide bus */
#define TARGET_FLASH_DEVICES_16 \
    { PHYS_TO_UNCACHED(0x1c000000), 0x00080000, 1, 1, FL_BUS_8  },  \
    { PHYS_TO_UNCACHED(0x1f800000), 0x00080000, 1, 1, FL_BUS_8  },  \
    { PHYS_TO_UNCACHED(0x1c000000), 0x00200000, 2, 1, FL_BUS_16  }, \
    { 0x00000000, 0x00000000 }
/*
 *  Structure describing targets FLASH memory map.
 */
struct fl_map {
  u_int32_t fl_map_base;  /* Start of flash area in physical map */
  u_int32_t fl_map_size;  /* Size of flash area in physical map */
  int fl_map_width; /* Number of bytes to program in one cycle */
  int fl_map_chips; /* Number of chips to operate in one cycle */
  int fl_map_bus; /* Bus width type, se below */
  int fl_map_offset;  /* Flash Offset mapped in memory */
  int fl_type;
};

UINT64 get_processor_prid(void);
int cpu_probe_release(void);
struct fl_map *
tgt_flashmap();
struct fl_map *
fl_find_map(int base);
void tgt_flashinfo(int p, size_t *t);

EFI_STATUS
GetCpuInfoHob (
  LS_CPU_INFO_HOB *CpuInfo
  );

EFI_STATUS
GetMemInfoHob (
  LS_MEM_INFO_HOB *MemInfo
  );

#endif

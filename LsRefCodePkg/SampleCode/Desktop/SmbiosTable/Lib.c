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
 * Transplat from pmon code
 *
 */
#include "SmbiosTable.h"
#include "Lib.h"
#include <Library/HobLib.h>

struct fl_map tgt_fl_map_boot16[]={
  TARGET_FLASH_DEVICES_16
};

struct fl_map *
tgt_flashmap()
{
  return tgt_fl_map_boot16;
}

/*
 *  Find what flash map address nelongs to.
 */
struct fl_map *
fl_find_map(int base)
{
  struct fl_map *map;
  for(map = tgt_flashmap(); map->fl_map_size != 0; map++) {
    // can not compile this pmon code
    if(map->fl_map_base > (u_int32_t )base ||
      (map->fl_map_base + map->fl_map_size - 1) < (u_int32_t )base) {
      continue; /* Not this one */
    }
    else {
      return(map);
    }
  }
  return((struct fl_map *)NULL);
}

void
tgt_flashinfo(int p, size_t *t)
{
  struct fl_map *map;

  map = fl_find_map(p);
  if(map) {
    *t = map->fl_map_size;
  } else {
    *t = 0;
  }
}

int cpu_probe_release(void)
{
  unsigned long cputype_flag = 0;

  cputype_flag = *(volatile UINT32*)(PHYS_TO_UNCACHED(0xefdfb00005c));
  if(cputype_flag == 0x7778888) //check whether it is a 3A5
    return 1;
  else
    return 0;
}

UINT64 get_processor_prid(void)
{
  UINT64 prid;

  __asm__ volatile (
      "csrrd    %0, 0xc0\r\n"
      :"=r"(prid));

  return (prid & 0xffffffff);
}

EFI_STATUS
GetCpuInfoHob (
  IN OUT LS_CPU_INFO_HOB   *CpuInfoData
  )
{
  EFI_HOB_GUID_TYPE    *CpuInfoHob;
  LS_CPU_INFO_HOB      *CpuInfoHobData;

  ASSERT(CpuInfoData != NULL);

  CpuInfoHob = GetFirstGuidHob (&gLsCpuInfoHobGuid);

  CpuInfoHobData = (LS_CPU_INFO_HOB *)(CpuInfoHob + 1);

  CopyMem (CpuInfoData, CpuInfoHobData, sizeof(LS_CPU_INFO_HOB));

  return EFI_SUCCESS;
}

EFI_STATUS
GetMemInfoHob (
  IN OUT LS_MEM_INFO_HOB   *MemInfoData
  )
{
  EFI_HOB_GUID_TYPE    *MemInfoHob;
  LS_MEM_INFO_HOB      *MemInfoHobData;

  ASSERT(MemInfoData != NULL);

  MemInfoHob = GetFirstGuidHob (&gLsMemInfoHobGuid);

  MemInfoHobData = (LS_MEM_INFO_HOB *)(MemInfoHob + 1);

  CopyMem (MemInfoData, MemInfoHobData, sizeof(LS_MEM_INFO_HOB));

  return EFI_SUCCESS;
}

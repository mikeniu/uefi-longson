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

#ifndef __HDA_VERB_TABLES_H__
#define __HDA_VERB_TABLES_H__

#include <Library/BaseLib.h>

UINT32 HdaVerbTableAlc269[] = {
  //===================================================================================================
  //
  //                                 Realtek Semiconductor Corp.
  //
  //===================================================================================================

  //Realtek High Definition Audio Configuration - Version : 5.0.3.2
  //Realtek HD Audio Codec : ALC269-VC3
  //PCI PnP ID : PCI\VEN_8086&DEV_2668&SUBSYS_026910EC
  //HDA Codec PnP ID : HDAUDIO\FUNC_01&VEN_10EC&DEV_0269&SUBSYS_10EC0269
  //The number of verb command block : 16

  //    NID 0x12 : 0x40000000
  //    NID 0x14 : 0x90170110
  //    NID 0x15 : 0x0121401F
  //    NID 0x17 : 0x90170120
  //    NID 0x18 : 0x01A19040
  //    NID 0x19 : 0x02A1904F
  //    NID 0x1A : 0x01813050
  //    NID 0x1B : 0x02014012
  //    NID 0x1D : 0x40400001
  //    NID 0x1E : 0x99430130
  //    NID 0x20 : 0x0313FFFF
  //===== HDA Codec Subsystem ID Verb-table =====
  //HDA Codec Subsystem ID  : 0x10EC7A07
  0x00172007,
  0x0017217A,
  0x001722EC,
  0x00172310,
  //===== Pin Widget Verb-table =====
  //Widget node 0x01 :
  0x0017FF00,
  0x0017FF00,
  0x0017FF00,
  0x0017FF00,
  //Pin widget 0x12 - DMIC
  0x01271C00,
  0x01271D00,
  0x01271E00,
  0x01271F40,
  //Pin widget 0x14 - SPEAKER-OUT (Port-D)
  0x01471C10,
  0x01471D01,
  0x01471E17,
  0x01471F90,
  //Pin widget 0x15 - HP-OUT (Port-A)
  0x01571C1F,
  0x01571D40,
  0x01571E21,
  0x01571F01,
  //Pin widget 0x17 - MONO-OUT (Port-H)
  0x01771C20,
  0x01771D01,
  0x01771E17,
  0x01771F90,
  //Pin widget 0x18 - MIC1 (Port-B)
  0x01871C40,
  0x01871D90,
  0x01871EA1,
  0x01871F01,
  //Pin widget 0x19 - MIC2 (Port-F)
  0x01971C4F,
  0x01971D90,
  0x01971EA1,
  0x01971F02,
  //Pin widget 0x1A - LINE1 (Port-C)
  0x01A71C50,
  0x01A71D30,
  0x01A71E81,
  0x01A71F01,
  //Pin widget 0x1B - LINE2 (Port-E)
  0x01B71C12,
  0x01B71D40,
  0x01B71E01,
  0x01B71F02,
  //Pin widget 0x1D - PC-BEEP
  0x01D71C01,
  0x01D71D00,
  0x01D71E40,
  0x01D71F40,
  //Pin widget 0x1E - S/PDIF-OUT
  0x01E71C30,
  0x01E71D01,
  0x01E71E43,
  0x01E71F99,
  //Widget node 0x20 :
  0x02050018,
  0x02045180,
  0x0205001C,
  0x02042800,
  //Widget node 0x20 - 1 :
  0x02050024,
  0x02040054,
  0x02050004,
  0x02040080,
  //Widget node 0x20 - 2 :
  0x02050015,
  0x02048002,
  0x02050015,
  0x02048002,
  //Widget node 0x0C :
  0x00C37080,
  0x00270610,
  0x00D37080,
  0x00370610
};

UINT32 *gHdaVerbTable    = HdaVerbTableAlc269;
UINT64 gHdaVerbTableSize = sizeof (HdaVerbTableAlc269) / sizeof (UINT32);

#endif

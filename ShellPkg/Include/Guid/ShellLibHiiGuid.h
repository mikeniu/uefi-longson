/** @file
  GUIDs for HII package list installed by Shell libraries.

  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
  All intellectual property rights(Copyright, Patent and Trademark) reserved.
  Copyright (c) 2011 - 2016, Intel Corporation. All rights reserved.<BR>

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

#ifndef _SHELLLIB_HII_GUID_H_
#define _SHELLLIB_HII_GUID_H_

#define HANDLE_PARSING_HII_GUID \
  { \
  0xb8969637, 0x81de, 0x43af, { 0xbc, 0x9a, 0x24, 0xd9, 0x89, 0x13, 0xf2, 0xf6 } \
  }

#define SHELL_DEBUG1_HII_GUID \
  { \
    0x25f200aa, 0xd3cb, 0x470a, { 0xbf, 0x51, 0xe7, 0xd1, 0x62, 0xd2, 0x2e, 0x6f } \
  }

#define SHELL_DRIVER1_HII_GUID \
  { \
  0xaf0b742, 0x63ec, 0x45bd, {0x8d, 0xb6, 0x71, 0xad, 0x7f, 0x2f, 0xe8, 0xe8} \
  }

#define SHELL_INSTALL1_HII_GUID \
  { \
    0x7d574d54, 0xd364, 0x4d4a, { 0x95, 0xe3, 0x49, 0x45, 0xdb, 0x7a, 0xd3, 0xee } \
  }

#define SHELL_LEVEL1_HII_GUID \
  { \
    0xdec5daa4, 0x6781, 0x4820, { 0x9c, 0x63, 0xa7, 0xb0, 0xe4, 0xf1, 0xdb, 0x31 } \
  }

#define SHELL_LEVEL2_HII_GUID \
  { \
    0xf95a7ccc, 0x4c55, 0x4426, { 0xa7, 0xb4, 0xdc, 0x89, 0x61, 0x95, 0xb, 0xae } \
  }

#define SHELL_LEVEL3_HII_GUID \
  { \
    0x4344558d, 0x4ef9, 0x4725, { 0xb1, 0xe4, 0x33, 0x76, 0xe8, 0xd6, 0x97, 0x4f } \
  }

#define SHELL_NETWORK1_HII_GUID \
  { \
    0xf3d301bb, 0xf4a5, 0x45a8, { 0xb0, 0xb7, 0xfa, 0x99, 0x9c, 0x62, 0x37, 0xae } \
  }

#define SHELL_NETWORK2_HII_GUID \
  { \
    0x174b2b5, 0xf505, 0x4b12, { 0xaa, 0x60, 0x59, 0xdf, 0xf8, 0xd6, 0xea, 0x37 } \
  }

#define SHELL_TFTP_HII_GUID \
  { \
    0x738a9314, 0x82c1, 0x4592, { 0x8f, 0xf7, 0xc1, 0xbd, 0xf1, 0xb2, 0x0e, 0xd4 } \
  }


#define SHELL_BCFG_HII_GUID \
  { \
    0x5f5f605d, 0x1583, 0x4a2d, {0xa6, 0xb2, 0xeb, 0x12, 0xda, 0xb4, 0xa2, 0xb6 } \
  }

#define UEFI_VERS_HII_GUID \
  { \
    0xede2454b, 0x361a, 0x4a2e, {0xb3, 0x2d, 0xa4, 0x41, 0xde, 0x6f, 0x82, 0x01} \
  }

extern EFI_GUID gHandleParsingHiiGuid;
extern EFI_GUID gShellDebug1HiiGuid;
extern EFI_GUID gShellDriver1HiiGuid;
extern EFI_GUID gShellInstall1HiiGuid;
extern EFI_GUID gShellLevel1HiiGuid;
extern EFI_GUID gShellLevel2HiiGuid;
extern EFI_GUID gShellLevel3HiiGuid;
extern EFI_GUID gShellNetwork1HiiGuid;
extern EFI_GUID gShellNetwork2HiiGuid;
extern EFI_GUID gShellTftpHiiGuid;
extern EFI_GUID gShellBcfgHiiGuid;
extern EFI_GUID gShellSpiHiiGuid;
extern EFI_GUID gUefiVersHiiGuid;

#endif

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
#include <Uefi.h>
#include <Library/DebugLib.h>
#include "DcPhySil9022a.h"

static UINT8  DebugFlagSil9022 = (0x01<<0|0x01<<1|0x00<<2);

#define LOG_MSG_SIL90322A   "Sil9022a"

#define PrintfError(format, ...) \
  if(DebugFlagSil9022&(0x01<<0)){\
    DbgPrint(EFI_D_ERROR,format, ##__VA_ARGS__);\
  }else{\
    ;\
  }
#define Printf(format, ...) \
  if(DebugFlagSil9022&(0x01<<1)){\
    DbgPrint(EFI_D_INFO,format, ##__VA_ARGS__);\
  }else{\
    ;\
  }
#define Log(format, ...)\
  if(DebugFlagSil9022&(0x01<<2)){\
    DbgPrint(EFI_D_INFO,"[%a:%d:%a]"format"", LOG_MSG_SIL90322A, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
  }else{\
    ;\
  }

EFI_STATUS
EFIAPI
DcPhySil9022aConfig(
  IN UINT8  DvoChannel,
  IN UINT8  DevAddr,
  IN UINT32 FreqKhz,
  IN UINT8  Mode
  )
{
  UINT8 Data;
  UINT8 Id0, Id1, Id2;
  EFI_STATUS ret = EFI_SUCCESS;

  /* get reg base addr */
  Ls7aI2cGpioInit(Mode, FreqKhz);

  if(!Ls7aI2cSendData(DvoChannel, DevAddr, 0xC7, 0x00)){
    Printf("ERROR: i2c send.\n");
    ret = EFI_DEVICE_ERROR;
  }

  if(!Ls7aI2cReceiveData(DvoChannel, DevAddr, 0x1B, &Id0)){
    Printf("ERROR: i2c read.\n");
    ret = EFI_DEVICE_ERROR;
  }
  if(!Ls7aI2cReceiveData(DvoChannel, DevAddr, 0x1C, &Id1)){
    Printf("ERROR: i2c read.\n");
    ret = EFI_DEVICE_ERROR;
  }
  if(!Ls7aI2cReceiveData(DvoChannel, DevAddr, 0x1D, &Id2)){
    Printf("ERROR: i2c read.\n");
    ret = EFI_DEVICE_ERROR;
  }
  if (Id0 != 0xb0 || Id1 != 0x2 || Id2 != 0x3) {
    Printf("id err: %d,%d,%d.\n", Id0, Id1, Id2);
    ret = EFI_DEVICE_ERROR;
  }

  if(!Ls7aI2cReceiveData(DvoChannel, DevAddr, 0x1E, &Data)){
    Printf("ERROR: i2c read.\n");
    ret = EFI_DEVICE_ERROR;
  }
  Data &= ~(0x3);
  if(!Ls7aI2cSendData(DvoChannel, DevAddr, 0x1E, Data)){
    Printf("ERROR: i2c send.\n");
    ret = EFI_DEVICE_ERROR;
  }

  if(!Ls7aI2cReceiveData(DvoChannel, DevAddr, 0x1A, &Data)){
    Printf("ERROR: i2c read.\n");
    ret = EFI_DEVICE_ERROR;
  }
  Data &= ~(1 << 4);
  Data |= (1 << 0);
  if(!Ls7aI2cSendData(DvoChannel, DevAddr, 0x1A, Data)){
    Printf("ERROR: i2c send.\n");
    ret = EFI_DEVICE_ERROR;
  }

  if(!Ls7aI2cSendData(DvoChannel, DevAddr, 0x26, 0x40)){
    Printf("ERROR: i2c send.\n");
    ret = EFI_DEVICE_ERROR;
  }
  if(Mode == GPIO_I2C_MODE_PEI){
    Ls7aI2cGpioExit();
  }

  return ret;
}

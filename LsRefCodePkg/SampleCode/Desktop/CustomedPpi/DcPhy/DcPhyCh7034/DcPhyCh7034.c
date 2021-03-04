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
#include "DcPhyCh7034.h"

static UINT8  DebugFlagCh7034 = (0x01<<0|0x01<<1|0x00<<2);

#define LOG_MSG_CH704  "Ch7034"

#define PrintfError(format, ...) \
  if(DebugFlagCh7034&(0x01<<0)){\
    DbgPrint(EFI_D_ERROR,format, ##__VA_ARGS__);\
  }else{\
    ;\
  }
#define Printf(format, ...) \
  if(DebugFlagCh7034&(0x01<<1)){\
    DbgPrint(EFI_D_INFO,format, ##__VA_ARGS__);\
  }else{\
    ;\
  }
#define Log(format, ...)\
  if(DebugFlagCh7034&(0x01<<2)){\
    DbgPrint(EFI_D_INFO,"[%a:%d:%a]"format"", LOG_MSG_CH704, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
  }else{\
    ;\
  }

UINT8 CH7034_VGA_REG_TABLE2[][131][2] = {
  {//VGA
    //MYIN 800x600,out 800x600,ch7034,bypassmode vga,mode_idx=1
    { 0x03, 0x04 },//page 4
    { 0x52, 0xC3 },
    { 0x5A, 0x06 },
    { 0x5A, 0x04 },
    { 0x5A, 0x06 },
    { 0x52, 0xC1 },
    { 0x52, 0xC3 },
    { 0x5A, 0x04 },

    { 0x03, 0x00 },//page 1
    { 0x07, 0xD9 },
    { 0x08, 0xF1 },
    { 0x09, 0x13 },
    { 0x0A, 0xBE },
    { 0x0B, 0x23 },
    { 0x0C, 0x20 },
    { 0x0D, 0x20 },
    { 0x0E, 0x00 },
    { 0x0F, 0x28 },
    { 0x10, 0x80 },
    { 0x11, 0x12 },
    { 0x12, 0x58 },
    { 0x13, 0x74 },
    { 0x14, 0x00 },
    { 0x15, 0x01 },
    { 0x16, 0x04 },
    { 0x17, 0x00 },
    { 0x18, 0x00 },//modify 888RGB
    // { 0x18, 0x20 },//565RGB
    // { 0x18, 0x02 },//888GRB
    { 0x19, 0xF8 },//freq
    { 0x1A, 0x9B },
    { 0x1B, 0x78 },
    { 0x1C, 0x69 },
    { 0x1D, 0x78 },
    { 0x1E, 0x00 },//output is  progressive
    { 0x1F, 0x23 },
    { 0x20, 0x20 },
    { 0x21, 0x20 },
    { 0x22, 0x00 },
    { 0x23, 0x10 },
    { 0x24, 0x60 },
    { 0x25, 0x12 },
    { 0x26, 0x58 },
    { 0x27, 0x74 },
    { 0x28, 0x00 },
    { 0x29, 0x0A },
    { 0x2A, 0x02 },
    { 0x2B, 0x09 },//vga output format:bypass mode
    { 0x2C, 0x00 },
    { 0x2D, 0x00 },
    { 0x2E, 0x3D },
    { 0x2F, 0x00 },//??
    { 0x32, 0xC0 },//??
    { 0x36, 0x40 },
    { 0x38, 0x47 },
    { 0x3D, 0x86 },
    { 0x3E, 0x00 },
    { 0x40, 0x0E },
    { 0x4B, 0x40 },//pwm control
    { 0x4C, 0x40 },//lvds output channel order register
    { 0x4D, 0x80 },
    { 0x54, 0x80 },//lvds
    { 0x55, 0x28 },//lvds
    { 0x56, 0x80 },//lvds
    { 0x57, 0x00 },//lvds
    { 0x58, 0x01 },//lvds
    { 0x59, 0x04 },//lvds
    { 0x5A, 0x02 },
    { 0x5B, 0xF2 },
    { 0x5C, 0xB9 },
    { 0x5D, 0xD6 },
    { 0x5E, 0x54 },
    { 0x60, 0x00 },
    { 0x61, 0x00 },
    { 0x64, 0x2D },
    { 0x68, 0x44 },
    { 0x6A, 0x40 },
    { 0x6B, 0x00 },
    { 0x6C, 0x10 },
    { 0x6D, 0x00 },
    { 0x6E, 0xA0 },
    { 0x70, 0x98 },
    { 0x74, 0x30 },//scaling control
    { 0x75, 0x80 },//scaling control
    { 0x7E, 0x0F },//de and pwm control
    { 0x7F, 0x00 },//test pattern

    { 0x03, 0x01 },//page 2
    { 0x08, 0x05 },
    { 0x09, 0x04 },//diffen register
    { 0x0B, 0x65 },
    { 0x0C, 0x4A },
    { 0x0D, 0x29 },
    { 0x0F, 0x9C },
    { 0x12, 0xD4 },
    { 0x13, 0x28 },
    { 0x14, 0x83 },
    { 0x15, 0x00 },
    { 0x16, 0x00 },
    { 0x1A, 0x6C },//DAC termination control register
    { 0x1B, 0x00 },
    { 0x1C, 0x00 },
    { 0x1D, 0x00 },
    { 0x23, 0x63 },
    { 0x24, 0xB4 },
    { 0x28, 0x54 },
    { 0x29, 0x60 },
    { 0x41, 0x60 },//lvds
    { 0x63, 0x2D },//DE polarity
    { 0x6B, 0x11 },
    { 0x6C, 0x06 },

    { 0x03, 0x03 },//page3
    { 0x26, 0x00 },
    { 0x28, 0x08 },//output control:DAC output is VGA
    { 0x2A, 0x00 },//output control:HDTV output through scaler

    { 0x03, 0x04 },//page 4
    { 0x10, 0x00 },
    { 0x11, 0x9B },
    { 0x12, 0x78 },
    { 0x13, 0x02 },
    { 0x14, 0x88 },
    { 0x15, 0x70 },
    { 0x20, 0x00 },
    { 0x21, 0x00 },
    { 0x22, 0x00 },
    { 0x23, 0x00 },
    { 0x24, 0x00 },
    { 0x25, 0x00 },
    { 0x26, 0x00 },
    { 0x54, 0xC4 },
    { 0x55, 0x5B },
    { 0x56, 0x4D },
    { 0x60, 0x01 },
    { 0x61, 0x62 },
  },
};

#define CH7034_REGMAP_LENGTH_VGA (sizeof(CH7034_VGA_REG_TABLE2[0]) / (2*sizeof(UINT8)))

EFI_STATUS
  EFIAPI
DcPhyCh7034Config(
    IN UINT8  DvoChannel,
    IN UINT8  DevAddr,
    IN UINT32 FreqKhz,
    IN UINT8  Mode
    )
{
  EFI_STATUS Status;
  INTN  Count;
  UINT8 DataAddr, Data,Data2;

  Data2 = 0;
  Status = EFI_SUCCESS;

  /* get reg base addr */
  Ls7aI2cGpioInit(Mode, FreqKhz);

  for (Count = 0; Count < CH7034_REGMAP_LENGTH_VGA; Count++)
  {
    DataAddr = CH7034_VGA_REG_TABLE2[0][Count][0];
    Data = CH7034_VGA_REG_TABLE2[0][Count][1];
    if(!Ls7aI2cSendData(DvoChannel, DevAddr, DataAddr, Data)){
      PrintfError("ERROR: i2c send.\n");
    }

    //check
    if(!Ls7aI2cReceiveData(DvoChannel, DevAddr, DataAddr, &Data2)){
      PrintfError("ERROR: i2c read.\n");
    }
    if (Data != Data2){
      PrintfError("Count %d :Data 0x%d, Data2 0x%x. (error)\n", Count, Data, Data2);
      Status = EFI_DEVICE_ERROR;
      break;
    }else{
      Log("Count %d :Data 0x%x, Data2 0x%x.\n", Count, Data, Data2);
    }
    Data2 = 0;
  }
  if(Mode == GPIO_I2C_MODE_PEI){
    Ls7aI2cGpioExit();
  }

  return Status;
}

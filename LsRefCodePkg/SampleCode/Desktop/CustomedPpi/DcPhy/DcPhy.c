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
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Ppi/RcCorePpi.h>
#include "DcPhySil9022a/DcPhySil9022a.h"
#include "DcPhyIt66121/DcPhyIt66121.h"
#include "DcPhyCh7034/DcPhyCh7034.h"

// debug
static UINT8  DebugFlagDcPhy = (0x01<<0|0x01<<1|0x01<<2);

#define LOG_MSG_DC_PHY_INIT   "DcPhyInit"

#define PrintfError(format, ...) \
  if(DebugFlagDcPhy&(0x01<<0)){\
    DbgPrint(EFI_D_ERROR,format, ##__VA_ARGS__);\
  }else{\
    ;\
  }
#define Printf(format, ...) \
  if(DebugFlagDcPhy&(0x01<<1)){\
    DbgPrint(EFI_D_INFO,format, ##__VA_ARGS__);\
  }else{\
    ;\
  }
#define Log(format, ...)\
  if(DebugFlagDcPhy&(0x01<<2)){\
    DbgPrint(EFI_D_INFO,"[%a:%d:%a]"format"", LOG_MSG_DC_PHY_INIT, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
  }else{\
    ;\
  }

#define  DC_PHY_FUNC_DISABLE  (0)
#define  DC_PHY_FUNC_ENABLE   (1)

typedef
EFI_STATUS
(EFIAPI *DC_PHY_CONFIG_FUNC)(
  IN UINT8  DvoChannel,
  IN UINT8  DevAddr,
  IN UINT32 FreqKhz,
  IN UINT8  Mode
  );

typedef struct{
  char *LabelString;
  UINT8 Enable;
  DC_PHY_CONFIG_FUNC  Func;
  UINT8 ParaDvoCh;
  UINT8 ParaDevAddr;
  UINT8 ParaFreqKHz;
  UINT8 ParaMode;
}DC_PHY_INIT_FUNC_LIST;

static DC_PHY_INIT_FUNC_LIST DcPhyInitFuncList[] = {
  {
    .LabelString = "Sil9022a",
#ifdef HDMI_SIL9022A_SUPPORT
    .Enable = DC_PHY_FUNC_ENABLE,
#else
    .Enable = DC_PHY_FUNC_DISABLE,
#endif
    .Func = DcPhySil9022aConfig,
    .ParaDvoCh = DVO_CH1,
    .ParaDevAddr = 0x72,
    .ParaFreqKHz = 10,
    .ParaMode = GPIO_I2C_MODE_PEI,
  },
  {
    .LabelString = "it66121",
#ifdef HDMI_IT66121_SUPPORT
    .Enable = DC_PHY_FUNC_ENABLE,
#else
    .Enable = DC_PHY_FUNC_DISABLE,
#endif
    .Func = DcPhyIt66121Config,
    .ParaDvoCh = DVO_CH1,
    .ParaDevAddr = 0x9a,
    .ParaFreqKHz = 100,
    .ParaMode = GPIO_I2C_MODE_PEI,
  },
  {
    .LabelString = "ch7034",
#ifdef VGA_CH7034_SUPPORT
    .Enable = DC_PHY_FUNC_ENABLE,
#else
    .Enable = DC_PHY_FUNC_DISABLE,
#endif
    .Func = DcPhyCh7034Config,
    .ParaDvoCh = DVO_CH1,
    .ParaDevAddr = 0xea,
    .ParaFreqKHz = 100,
    .ParaMode = GPIO_I2C_MODE_PEI,
  }
};

EFI_STATUS
EFIAPI
DcPhyInitFunc(
    IN UINTN  DvoChannel
    )
{
  EFI_STATUS Status;
  UINT8 Conunt;
  UINT8 Index;

  Log("Enter.\n");
  Status = EFI_SUCCESS;
  Conunt = sizeof(DcPhyInitFuncList)/sizeof(DC_PHY_INIT_FUNC_LIST);
  for(Index = 0; Index < Conunt; Index++) {
    if (DvoChannel == DcPhyInitFuncList[Index].ParaDvoCh) {
      if(DcPhyInitFuncList[Index].Enable) {
        Status = DcPhyInitFuncList[Index].Func(
            DcPhyInitFuncList[Index].ParaDvoCh,
            DcPhyInitFuncList[Index].ParaDevAddr,
            DcPhyInitFuncList[Index].ParaFreqKHz,
            DcPhyInitFuncList[Index].ParaMode
            );
        if(EFI_ERROR (Status)) {
          PrintfError("Index:%d %a init fail! (DvoCh:%d, DevAddr:0x%x, FreqKhz:%dKhz).\n",
              Index,
              DcPhyInitFuncList[Index].LabelString,
              DcPhyInitFuncList[Index].ParaDvoCh,
              DcPhyInitFuncList[Index].ParaDevAddr,
              DcPhyInitFuncList[Index].ParaFreqKHz
              );
        } else {
          Printf ("Index:%d %a init ok!\n",
              Index,
              DcPhyInitFuncList[Index].LabelString
              );
        }
      }
    }
  }
  Log("Leave.\n");
  return Status;
}

#define DVO0      0
#define DVO1      1

//Dc Phy Config
EFI_STATUS
Dvo0Init ( //Gpio 0 and 1
    VOID
  )
{
  EFI_STATUS Status;

  Status = DcPhyInitFunc(DVO0);

  return Status;
}

EFI_STATUS
Dvo1Init ( //Gpio 2 and 3
    VOID
  )
{
  EFI_STATUS Status;

  Status = DcPhyInitFunc(DVO1);

  return Status;
}

LS_DC_PHY_CFG  LsDcPhyCfg;

EFI_STATUS
RegDcPhyCfgFunc (
    LS_CUSTOMER_CFG_PPI   *CustomedCfgPpi
    )
{
  //EFI_STATUS Status;

  LsDcPhyCfg.Dvo0Cfg       =  Dvo0Init;   //VGA
  LsDcPhyCfg.Dvo1Cfg       =  Dvo1Init;   //HDMI

  CustomedCfgPpi->DcPhyCfg = (LS_DC_PHY_CFG*)&LsDcPhyCfg;

  return EFI_SUCCESS;
}


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
#include <Library/TimerLib.h>
#include "hdmitx_drv.h"
#include "hdmitx_sys.h"
#include "DcPhyIt66121.h"

#define  DEBUG_IT66121    0
#define LOG_MSG_IT66121      "IT66121"

#if (DEBUG_IT66121)
#define Log(info,format, ...) \
  do{\
    DebugPrint(info,"[%a:%d:%a]"format"", LOG_MSG_IT66121, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
  }while(0);
#define DeBug(info,format, ...) \
  do{\
    DebugPrint(info,format, ##__VA_ARGS__); \
  }while(0);
#else
#define Log(info,format, ...) \
  do{\
  }while(0);
#define DeBug(info,format, ...)\
  do{\
  }while(0);
#endif


#define SUPPORT_OUTPUTYUV
#define SUPPORT_OUTPUTRGB
// #define DISABLE_HDMITX_CSC

#define SUPPORT_INPUTRGB
#define SUPPORT_INPUTYUV444
#define SUPPORT_INPUTYUV422
// #define SUPPORT_SYNCEMBEDDED
// #define SUPPORT_DEGEN

#define INPUT_SIGNAL_TYPE 0 // 24 bit sync seperate
#define I2S_FORMAT 0x01 // 32bit audio
#define INPUT_SAMPLE_FREQ 2//AUDFS_48KHz

#define F_MODE_RGB444  0
#define F_MODE_YUV422 1
#define F_MODE_YUV444 2
#define F_MODE_CLRMOD_MASK 3


#define F_MODE_INTERLACE  1

#define F_VIDMODE_ITU709  (1<<4)
#define F_VIDMODE_ITU601  0

#define F_VIDMODE_0_255   0
#define F_VIDMODE_16_235  (1<<5)

#define F_VIDMODE_EN_UDFILT (1<<6)
#define F_VIDMODE_EN_DITHER (1<<7)

#define T_MODE_CCIR656 (1<<0)
#define T_MODE_SYNCEMB (1<<1)
#define T_MODE_INDDR   (1<<2)
#define T_MODE_PCLKDIV2 (1<<3)
#define T_MODE_DEGEN (1<<4)
#define T_MODE_SYNCGEN (1<<5)


//#ifndef INV_INPUT_PCLK
//#define PCLKINV 0
//#else
#define PCLKINV B_TX_VDO_LATCH_EDGE
//#endif

#ifndef INV_INPUT_ACLK
#define InvAudCLK 0
#else
#define InvAudCLK B_TX_AUDFMT_FALL_EDGE_SAMPLE_WS
#endif

#define SUPPORT_AUDI_AudSWL 24 // Jeilin case.

#define F_AUDIO_ON  (1<<7)
#define F_AUDIO_HBR (1<<6)
#define F_AUDIO_DSD (1<<5)
#define F_AUDIO_NLPCM (1<<4)
#define F_AUDIO_LAYOUT_1 (1<<3)
#define F_AUDIO_LAYOUT_0 (0<<3)

// HBR - 1100
// DSD - 1010
// NLPCM - 1001
// LPCM - 1000

#define T_AUDIO_MASK 0xF0
#define T_AUDIO_OFF 0
#define T_AUDIO_HBR (F_AUDIO_ON|F_AUDIO_HBR)
#define T_AUDIO_DSD (F_AUDIO_ON|F_AUDIO_DSD)
#define T_AUDIO_NLPCM (F_AUDIO_ON|F_AUDIO_NLPCM)
#define T_AUDIO_LPCM (F_AUDIO_ON)

// for sample clock
#define AUDFS_22p05KHz  4
#define AUDFS_44p1KHz 0
#define AUDFS_88p2KHz 8
#define AUDFS_176p4KHz    12

#define AUDFS_24KHz  6
#define AUDFS_48KHz  2
#define AUDFS_96KHz  10
#define AUDFS_192KHz 14

#define AUDFS_768KHz 9

#define AUDFS_32KHz  3
#define AUDFS_OTHER    1

// Audio Enable
#define ENABLE_SPDIF    (1<<4)
#define ENABLE_I2S_SRC3  (1<<3)
#define ENABLE_I2S_SRC2  (1<<2)
#define ENABLE_I2S_SRC1  (1<<1)
#define ENABLE_I2S_SRC0  (1<<0)

#define AUD_SWL_NOINDICATE  0x0
#define AUD_SWL_16          0x2
#define AUD_SWL_17          0xC
#define AUD_SWL_18          0x4
#define AUD_SWL_20          0xA // for maximum 20 bit
#define AUD_SWL_21          0xD
#define AUD_SWL_22          0x5
#define AUD_SWL_23          0x9
#define AUD_SWL_24          0xB
#define CHTSTS_SWCODE 0x0B

#define HDMITX_MAX_DEV_COUNT 1

#define INPUT_COLOR_MODE F_MODE_RGB444
#define INPUT_COLOR_DEPTH 24
#define OUTPUT_COLOR_MODE F_MODE_YUV444
#define INPUT_CLOCK_DELAY  0x01

#define F_AUDIO_ON  (1<<7)
#define REG_TX_HDCP_DESIRE 0x20
#define B_TX_ENABLE_HDPC11 (1<<1)
#define B_TX_CPDESIRE  (1<<0)


#define DELAY_IT6612(time)   (MicroSecondDelay(time))
static UINT8 I2cAddr = 0x98;
static UINT8 I2cCh = 0;

UINT8 bCSCMtx_RGB2YUV_ITU601_16_235[] =
{
  0x00,0x80,0x00,
  0xB2,0x04,0x65,0x02,0xE9,0x00,
  0x93,0x3C,0x18,0x04,0x55,0x3F,
  0x49,0x3D,0x9F,0x3E,0x18,0x04
} ;

UINT8 bCSCMtx_RGB2YUV_ITU601_0_255[] =
{
  0x10,0x80,0x10,
  0x09,0x04,0x0E,0x02,0xC9,0x00,
  0x0F,0x3D,0x84,0x03,0x6D,0x3F,
  0xAB,0x3D,0xD1,0x3E,0x84,0x03
} ;

UINT8 bCSCMtx_RGB2YUV_ITU709_16_235[] =
{
  0x00,0x80,0x00,
  0xB8,0x05,0xB4,0x01,0x94,0x00,
  0x4a,0x3C,0x17,0x04,0x9F,0x3F,
  0xD9,0x3C,0x10,0x3F,0x17,0x04
} ;

UINT8 bCSCMtx_RGB2YUV_ITU709_0_255[] =
{
  0x10,0x80,0x10,
  0xEa,0x04,0x77,0x01,0x7F,0x00,
  0xD0,0x3C,0x83,0x03,0xAD,0x3F,
  0x4B,0x3D,0x32,0x3F,0x83,0x03
} ;

UINT8 bCSCMtx_YUV2RGB_ITU601_16_235[] =
{
  0x00,0x00,0x00,
  0x00,0x08,0x6B,0x3A,0x50,0x3D,
  0x00,0x08,0xF5,0x0A,0x02,0x00,
  0x00,0x08,0xFD,0x3F,0xDA,0x0D
} ;

UINT8 bCSCMtx_YUV2RGB_ITU601_0_255[] =
{
  0x04,0x00,0xA7,
  0x4F,0x09,0x81,0x39,0xDD,0x3C,
  0x4F,0x09,0xC4,0x0C,0x01,0x00,
  0x4F,0x09,0xFD,0x3F,0x1F,0x10
} ;

UINT8 bCSCMtx_YUV2RGB_ITU709_16_235[] =
{
  0x00,0x00,0x00,
  0x00,0x08,0x55,0x3C,0x88,0x3E,
  0x00,0x08,0x51,0x0C,0x00,0x00,
  0x00,0x08,0x00,0x00,0x84,0x0E
} ;

UINT8 bCSCMtx_YUV2RGB_ITU709_0_255[] =
{
  0x04,0x00,0xA7,
  0x4F,0x09,0xBA,0x3B,0x4B,0x3E,
  0x4F,0x09,0x57,0x0E,0x02,0x00,
  0x4F,0x09,0xFE,0x3F,0xE8,0x10
};

RegSetEntry HDMITX_Init_Table[] = {
  {0x0F, 0x40, 0x00},

  {0x62, 0x08, 0x00},
  {0x64, 0x04, 0x00},
  {0x01,0x00,0x00},//idle(100);

  {0x04, 0x20, 0x20},
  {0x04, 0x1D, 0x1D},
  {0x01,0x00,0x00},//idle(100);
  {0x0F, 0x01, 0x00}, // bank 0 ;
#ifdef INIT_CLK_LOW
  {0x62, 0x90, 0x10},
  {0x64, 0x89, 0x09},
  {0x68, 0x10, 0x10},
#endif

  {0xD1, 0x0E, 0x0C},
  {0x65, 0x03, 0x00},
#if 1//def NON_SEQUENTIAL_YCBCR422 // for ITE HDMIRX
  {0x71, 0xFC, 0x1C},
#else
  {0x71, 0xFC, 0x18},
#endif

  {0x8D, 0xFF, CEC_I2C_SLAVE_ADDR},
  {0x0F, 0x08, 0x08},

  {0xF8,0xFF,0xC3},
  {0xF8,0xFF,0xA5},
  {0x20, 0x80, 0x80},
  {0x37, 0x01, 0x00},
  {0x20, 0x80, 0x00},
  {0xF8,0xFF,0xFF},
  // 2014/01/07 HW Request for ROSC stable
  {0x5D,0x03,0x01},
  //~2014/01/07
#ifdef USE_IT66120
  {0x5A, 0x02, 0x00},
  {0xE2, 0xFF, 0xFF},
#endif
  {0x59, 0xD8, 0x40|PCLKINV},
  {0xE1, 0x20, InvAudCLK},
  {0x05, 0xe0, 0x80},
  {REG_TX_INT_MASK1, 0xFF, ~(B_TX_RXSEN_MASK|B_TX_HPD_MASK)},
  {REG_TX_INT_MASK2, 0xFF, ~(B_TX_KSVLISTCHK_MASK|B_TX_AUTH_DONE_MASK|B_TX_AUTH_FAIL_MASK)},
  {REG_TX_INT_MASK3, 0xFF, ~(B_TX_VIDSTABLE_MASK)},
  {0x0C, 0xFF, 0xFF},
  {0x0D, 0xFF, 0xFF},
  {0x0E, 0x03, 0x03},

  {0x0C, 0xFF, 0x00},
  {0x0D, 0xFF, 0x00},
  {0x0E, 0x02, 0x00},
  {0x09, 0x03, 0x00}, // Enable HPD and RxSen Interrupt
  {0x20,0x01,0x00},

  {0,0,0}
};

RegSetEntry HDMITX_DefaultVideo_Table[] = {
  ////////////////////////////////////////////////////
  // Config default output format.
  ////////////////////////////////////////////////////
  {0x72, 0xff, 0x00},
  {0x70, 0xff, 0x00},
#ifndef DEFAULT_INPUT_YCBCR
  // GenCSC\RGB2YUV_ITU709_16_235.c
  {0x72, 0xFF, 0x02},
  {0x73, 0xFF, 0x00},
  {0x74, 0xFF, 0x80},
  {0x75, 0xFF, 0x00},
  {0x76, 0xFF, 0xB8},
  {0x77, 0xFF, 0x05},
  {0x78, 0xFF, 0xB4},
  {0x79, 0xFF, 0x01},
  {0x7A, 0xFF, 0x93},
  {0x7B, 0xFF, 0x00},
  {0x7C, 0xFF, 0x49},
  {0x7D, 0xFF, 0x3C},
  {0x7E, 0xFF, 0x18},
  {0x7F, 0xFF, 0x04},
  {0x80, 0xFF, 0x9F},
  {0x81, 0xFF, 0x3F},
  {0x82, 0xFF, 0xD9},
  {0x83, 0xFF, 0x3C},
  {0x84, 0xFF, 0x10},
  {0x85, 0xFF, 0x3F},
  {0x86, 0xFF, 0x18},
  {0x87, 0xFF, 0x04},
#else
  // GenCSC\YUV2RGB_ITU709_16_235.c
  {0x0F, 0x01, 0x00},
  {0x72, 0xFF, 0x03},
  {0x73, 0xFF, 0x00},
  {0x74, 0xFF, 0x80},
  {0x75, 0xFF, 0x00},
  {0x76, 0xFF, 0x00},
  {0x77, 0xFF, 0x08},
  {0x78, 0xFF, 0x53},
  {0x79, 0xFF, 0x3C},
  {0x7A, 0xFF, 0x89},
  {0x7B, 0xFF, 0x3E},
  {0x7C, 0xFF, 0x00},
  {0x7D, 0xFF, 0x08},
  {0x7E, 0xFF, 0x51},
  {0x7F, 0xFF, 0x0C},
  {0x80, 0xFF, 0x00},
  {0x81, 0xFF, 0x00},
  {0x82, 0xFF, 0x00},
  {0x83, 0xFF, 0x08},
  {0x84, 0xFF, 0x00},
  {0x85, 0xFF, 0x00},
  {0x86, 0xFF, 0x87},
  {0x87, 0xFF, 0x0E},
#endif
  // 2012/12/20 added by Keming's suggestion test
  {0x88, 0xF0, 0x00},
  //~jauchih.tseng@ite.com.tw
  {0x04, 0x08, 0x00},
  {0,0,0}
};

RegSetEntry HDMITX_SetHDMI_Table[] = {
  ////////////////////////////////////////////////////
  // Config default HDMI Mode
  ////////////////////////////////////////////////////
  {0xC0, 0x01, 0x01},
  {0xC1, 0x03, 0x03},
  {0xC6, 0x03, 0x03},
  {0,0,0}
};

RegSetEntry HDMITX_SetDVI_Table[] = {
  ////////////////////////////////////////////////////
  // Config default HDMI Mode
  ////////////////////////////////////////////////////
  {0x0F, 0x01, 0x01},
  {0x58, 0xFF, 0x00},
  {0x0F, 0x01, 0x00},
  {0xC0, 0x01, 0x00},
  {0xC1, 0x03, 0x02},
  {0xC6, 0x03, 0x00},
  {0,0,0}
};

RegSetEntry HDMITX_DefaultAVIInfo_Table[] = {
  ////////////////////////////////////////////////////
  // Config default avi infoframe
  ////////////////////////////////////////////////////
  {0x0F, 0x01, 0x01},
  {0x58, 0xFF, 0x10},
  {0x59, 0xFF, 0x08},
  {0x5A, 0xFF, 0x00},
  {0x5B, 0xFF, 0x00},
  {0x5C, 0xFF, 0x00},
  {0x5D, 0xFF, 0x57},
  {0x5E, 0xFF, 0x00},
  {0x5F, 0xFF, 0x00},
  {0x60, 0xFF, 0x00},
  {0x61, 0xFF, 0x00},
  {0x62, 0xFF, 0x00},
  {0x63, 0xFF, 0x00},
  {0x64, 0xFF, 0x00},
  {0x65, 0xFF, 0x00},
  {0x0F, 0x01, 0x00},
  {0xCD, 0x03, 0x03},
  {0,0,0}
};

RegSetEntry HDMITX_DeaultAudioInfo_Table[] = {
  ////////////////////////////////////////////////////
  // Config default audio infoframe
  ////////////////////////////////////////////////////
  {0x0F, 0x01, 0x01},
  {0x68, 0xFF, 0x00},
  {0x69, 0xFF, 0x00},
  {0x6A, 0xFF, 0x00},
  {0x6B, 0xFF, 0x00},
  {0x6C, 0xFF, 0x00},
  {0x6D, 0xFF, 0x71},
  {0x0F, 0x01, 0x00},
  {0xCE, 0x03, 0x03},

  {0,0,0}
};

RegSetEntry HDMITX_Aud_CHStatus_LPCM_20bit_48Khz[] =
{
  {0x0F, 0x01, 0x01},
  {0x33, 0xFF, 0x00},
  {0x34, 0xFF, 0x18},
  {0x35, 0xFF, 0x00},
  {0x91, 0xFF, 0x00},
  {0x92, 0xFF, 0x00},
  {0x93, 0xFF, 0x01},
  {0x94, 0xFF, 0x00},
  {0x98, 0xFF, 0x02},
  {0x99, 0xFF, 0xDA},
  {0x0F, 0x01, 0x00},
  {0,0,0}//end of table
} ;

RegSetEntry HDMITX_AUD_SPDIF_2ch_24bit[] =
{
  {0x0F, 0x11, 0x00},
  {0x04, 0x14, 0x04},
  {0xE0, 0xFF, 0xD1},
  {0xE1, 0xFF, 0x01},
  {0xE2, 0xFF, 0xE4},
  {0xE3, 0xFF, 0x10},
  {0xE4, 0xFF, 0x00},
  {0xE5, 0xFF, 0x00},
  {0x04, 0x14, 0x00},
  {0,0,0}//end of table
} ;

RegSetEntry HDMITX_AUD_I2S_2ch_24bit[] =
{
  {0x0F, 0x11, 0x00},
  {0x04, 0x14, 0x04},
  {0xE0, 0xFF, 0xC1},
  {0xE1, 0xFF, 0x01},
#ifdef USE_IT66120
  {0x5A, 0x02, 0x00},
  {0xE2, 0xFF, 0xFF},
#else
  {0xE2, 0xFF, 0xE4},
#endif
  {0xE3, 0xFF, 0x00},
  {0xE4, 0xFF, 0x00},
  {0xE5, 0xFF, 0x00},
  {0x04, 0x14, 0x00},
  {0,0,0}//end of table
} ;

RegSetEntry HDMITX_DefaultAudio_Table[] = {
  ////////////////////////////////////////////////////
  // Config default audio output format.
  ////////////////////////////////////////////////////
  {0x0F, 0x21, 0x00},
  {0x04, 0x14, 0x04},
  {0xE0, 0xFF, 0xC1},
  {0xE1, 0xFF, 0x01},
#ifdef USE_IT66120
  {0xE2, 0xFF, 0xFF},
#else
  {0xE2, 0xFF, 0xE4},
#endif
  {0xE3, 0xFF, 0x00},
  {0xE4, 0xFF, 0x00},
  {0xE5, 0xFF, 0x00},
  {0x0F, 0x01, 0x01},
  {0x33, 0xFF, 0x00},
  {0x34, 0xFF, 0x18},
  {0x35, 0xFF, 0x00},
  {0x91, 0xFF, 0x00},
  {0x92, 0xFF, 0x00},
  {0x93, 0xFF, 0x01},
  {0x94, 0xFF, 0x00},
  {0x98, 0xFF, 0x02},
  {0x99, 0xFF, 0xDB},
  {0x0F, 0x01, 0x00},
  {0x04, 0x14, 0x00},

  {0x00, 0x00, 0x00} // End of Table.
} ;

RegSetEntry HDMITX_PwrDown_Table[] = {
  // Enable GRCLK
  {0x0F, 0x40, 0x00},
  // PLL Reset
  {0x61, 0x10, 0x10},   // DRV_RST
  {0x62, 0x08, 0x00},   // XP_RESETB
  {0x64, 0x04, 0x00},   // IP_RESETB
  {0x01, 0x00, 0x00}, // idle(100);

  // PLL PwrDn
  {0x61, 0x20, 0x20},   // PwrDn DRV
  {0x62, 0x44, 0x44},   // PwrDn XPLL
  {0x64, 0x40, 0x40},   // PwrDn IPLL

  // HDMITX PwrDn
  {0x05, 0x01, 0x01},   // PwrDn PCLK
  {0x0F, 0x78, 0x78},   // PwrDn GRCLK
  {0x00, 0x00, 0x00} // End of Table.
};

RegSetEntry HDMITX_PwrOn_Table[] = {
  {0x0F, 0x78, 0x38},   // PwrOn GRCLK
  {0x05, 0x01, 0x00},   // PwrOn PCLK

  // PLL PwrOn
  {0x61, 0x20, 0x00},   // PwrOn DRV
  {0x62, 0x44, 0x00},   // PwrOn XPLL
  {0x64, 0x40, 0x00},   // PwrOn IPLL

  // PLL Reset OFF
  {0x61, 0x10, 0x00},   // DRV_RST
  {0x62, 0x08, 0x08},   // XP_RESETB
  {0x64, 0x04, 0x04},   // IP_RESETB
  {0x0F, 0x78, 0x08},   // PwrOn IACLK
  {0x00, 0x00, 0x00} // End of Table.
};

HDMITXDEV InstanceData =
{
  0,      // BYTE I2C_DEV ;
  HDMI_TX_I2C_SLAVE_ADDR,    // BYTE I2C_ADDR ;

  /////////////////////////////////////////////////
  // Interrupt Type
  /////////////////////////////////////////////////
  0x40,      // BYTE bIntType ; // = 0 ;
  /////////////////////////////////////////////////
  // Video Property
  /////////////////////////////////////////////////
  INPUT_SIGNAL_TYPE ,// BYTE bInputVideoSignalType ; // for Sync Embedded,CCIR656,InputDDR

  /////////////////////////////////////////////////
  // Audio Property
  /////////////////////////////////////////////////
  I2S_FORMAT, // BYTE bOutputAudioMode ; // = 0 ;
  FALSE , // BYTE bAudioChannelSwap ; // = 0 ;
  0x01, // BYTE bAudioChannelEnable ;
  INPUT_SAMPLE_FREQ ,// BYTE bAudFs ;
  0, // unsigned long TMDSClock ;
  0, // unsigned RCLK ;
#ifdef _SUPPORT_HDCP_REPEATER_
  TxHDCP_Off,//HDMITX_HDCP_State TxHDCP_State ;
  0,         //unsigned short usHDCPTimeOut ;
  0,         //unsigned short Tx_BStatus ;
#endif
  FALSE, // BYTE bAuthenticated:1 ;
  FALSE, // BYTE bHDMIMode: 1;
  FALSE, // BYTE bIntPOL:1 ; // 0 = Low Active
  FALSE, // BYTE bHPD:1 ;
  FALSE,
  FALSE,
};

UINT8 bInputColorMode = INPUT_COLOR_MODE;
UINT8 OutputColorDepth = INPUT_COLOR_DEPTH ;
UINT8 bOutputColorMode = OUTPUT_COLOR_MODE ;

UINT8 iVideoModeSelect=0 ;

UINT32 VideoPixelClock ;
UINT8 VIC ; // 480p60
UINT8 pixelrep ; // no pixelrepeating
HDMI_Aspec aspec ;
HDMI_Colorimetry Colorimetry ;

UINT32 ulAudioSampleFS = INPUT_SAMPLE_FREQ_HZ ;
// UINT8 bAudioSampleFreq = INPUT_SAMPLE_FREQ ;
UINT8 bOutputAudioChannel = OUTPUT_CHANNEL ;

UINT8 init_flag = 1;

UINT8 bHDMIMode = 1;
UINT8 bAudioEnable =0 ;
UINT8 HPDStatus = FALSE;
UINT8 HPDChangeStatus = FALSE;
UINT8 bChangeAudio = TRUE ;
HDMITXDEV hdmiTxDev[HDMITX_MAX_DEV_COUNT] ;

UINT8 bOutputAudioType=F_AUDIO_ON;

UINT8 bChangeMode = FALSE ;
UINT64 index_bak=0;

UINT8 ucIEC60958ChStat[5];

UINT32 HDMITX_ReadI2C_Byte (UINT8 Address)
{
  UINT8 Value = 0;
  if(!Ls7aI2cReceiveData(I2cCh, I2cAddr, Address, &Value)){
    DeBug(EFI_D_INFO, "ERROR:%d:%a. Ls7aI2cReceiveData false.\r\n",__LINE__, __FUNCTION__);
  }

  return Value;
}

EFI_STATUS
HDMITX_WriteI2C_Byte (
    UINT8 Address,
    UINT8 Data
    )
{
  EFI_STATUS Status = EFI_SUCCESS;
  if(!Ls7aI2cSendData(I2cCh, I2cAddr, Address, Data)){
    Status = EFI_DEVICE_ERROR;
    DeBug(EFI_D_INFO, "ERROR:%d:%a. Ls7aI2cSendData false.\r\n",__LINE__, __FUNCTION__);
  }

  return Status;
}

EFI_STATUS
HDMITX_SetI2C_Byte(UINT8 Reg, UINT8 Mask, UINT8 Value)
{
  UINT8 Temp;
  if( Mask != 0xFF )
  {
    Temp=HDMITX_ReadI2C_Byte(Reg);
    Temp&=(~Mask);
    Temp|=Value&Mask;
  }
  else
  {
    Temp=Value;
  }
  return HDMITX_WriteI2C_Byte(Reg,Temp);

}

void hdmitx_LoadRegSetting(RegSetEntry table[])
{
  int i ;

  for( i = 0 ;  ; i++ )
  {
    if( table[i].offset == 0 && table[i].invAndMask == 0 && table[i].OrMask == 0 )
    {
      return ;
    }
    else if( table[i].invAndMask == 0 && table[i].OrMask == 0 )
    {
      DeBug(EFI_D_INFO, "delay(%d)\n",(int)table[i].offset);
      DELAY_IT6612((table[i].offset)*1000);
    }
    else if( table[i].invAndMask == 0xFF )
    {
      DeBug(EFI_D_INFO, "HDMITX_WriteI2C_Byte(%02x,%02x)\n",(int)table[i].offset,(int)table[i].OrMask);
      HDMITX_WriteI2C_Byte(table[i].offset,table[i].OrMask);
    }
    else
    {
      DeBug(EFI_D_INFO, "HDMITX_SetI2C_Byte(%02x,%02x,%02x)\n",(int)table[i].offset,(int)table[i].invAndMask,(int)table[i].OrMask);
      HDMITX_SetI2C_Byte(table[i].offset,table[i].invAndMask,table[i].OrMask);
    }
  }
}

void HDMITX_ChangeDisplayOption(HDMI_Video_Type OutputVideoTiming, HDMI_OutputColorMode OutputColorMode)
{
  //HDMI_Video_Type  t=HDMI_480i60_16x9;
  if((F_MODE_RGB444)==(bOutputColorMode&F_MODE_CLRMOD_MASK))//Force output RGB in RGB only case
  {
    OutputColorMode=F_MODE_RGB444;
  }
  else if ((F_MODE_YUV422)==(bOutputColorMode&F_MODE_CLRMOD_MASK))//YUV422 only
  {
    if(OutputColorMode==HDMI_YUV444){OutputColorMode=F_MODE_YUV422;}
  }
  else if ((F_MODE_YUV444)==(bOutputColorMode&F_MODE_CLRMOD_MASK))//YUV444 only
  {
    if(OutputColorMode==HDMI_YUV422){OutputColorMode=F_MODE_YUV444;}
  }
  switch(OutputVideoTiming)
  {
  case HDMI_0x55:
    VIC = 0;
    VideoPixelClock = 85500000L ;
    pixelrep = 0 ;
    aspec = HDMI_16x9 ;
    Colorimetry = HDMI_ITU709 ;
    break;
  case HDMI_640x480p60:
    VIC = 1 ;
    VideoPixelClock = 25000000 ;
    pixelrep = 0 ;
    aspec = HDMI_4x3 ;
    Colorimetry = HDMI_ITU601 ;
    break ;
  case HDMI_480p60:
    VIC = 2 ;
    VideoPixelClock = 27000000 ;
    pixelrep = 0 ;
    aspec = HDMI_4x3 ;
    Colorimetry = HDMI_ITU601 ;
    break ;
  case HDMI_480p60_16x9:
    VIC = 3 ;
    VideoPixelClock = 27000000 ;
    pixelrep = 0 ;
    aspec = HDMI_16x9 ;
    Colorimetry = HDMI_ITU601 ;
    break ;
  case HDMI_720p60:
    VIC = 4 ;
    VideoPixelClock = 74250000 ;
    pixelrep = 0 ;
    aspec = HDMI_16x9 ;
    Colorimetry = HDMI_ITU709 ;
    break ;
  case HDMI_1080i60:
    VIC = 5 ;
    VideoPixelClock = 74250000 ;
    pixelrep = 0 ;
    aspec = HDMI_16x9 ;
    Colorimetry = HDMI_ITU709 ;
    break ;
  case HDMI_480i60:
    VIC = 6 ;
    VideoPixelClock = 13500000 ;
    pixelrep = 1 ;
    aspec = HDMI_4x3 ;
    Colorimetry = HDMI_ITU601 ;
    break ;
  case HDMI_480i60_16x9:
    VIC = 7 ;
    VideoPixelClock = 13500000 ;
    pixelrep = 1 ;
    aspec = HDMI_16x9 ;
    Colorimetry = HDMI_ITU601 ;
    break ;
  case HDMI_1080p60:
    VIC = 16 ;
    VideoPixelClock = 148500000 ;
    pixelrep = 0 ;
    aspec = HDMI_16x9 ;
    Colorimetry = HDMI_ITU709 ;
    break ;
  case HDMI_576p50:
    VIC = 17 ;
    VideoPixelClock = 27000000 ;
    pixelrep = 0 ;
    aspec = HDMI_4x3 ;
    Colorimetry = HDMI_ITU601 ;
    break ;
  case HDMI_576p50_16x9:
    VIC = 18 ;
    VideoPixelClock = 27000000 ;
    pixelrep = 0 ;
    aspec = HDMI_16x9 ;
    Colorimetry = HDMI_ITU601 ;
    break ;
  case HDMI_720p50:
    VIC = 19 ;
    VideoPixelClock = 74250000 ;
    pixelrep = 0 ;
    aspec = HDMI_16x9 ;
    Colorimetry = HDMI_ITU709 ;
    break ;
  case HDMI_1080i50:
    VIC = 20 ;
    VideoPixelClock = 74250000 ;
    pixelrep = 0 ;
    aspec = HDMI_16x9 ;
    Colorimetry = HDMI_ITU709 ;
    break ;
  case HDMI_576i50:
    VIC = 21 ;
    VideoPixelClock = 13500000 ;
    pixelrep = 1 ;
    aspec = HDMI_4x3 ;
    Colorimetry = HDMI_ITU601 ;
    break ;
  case HDMI_576i50_16x9:
    VIC = 22 ;
    VideoPixelClock = 13500000 ;
    pixelrep = 1 ;
    aspec = HDMI_16x9 ;
    Colorimetry = HDMI_ITU601 ;
    break ;
  case HDMI_1080p50:
    VIC = 31 ;
    VideoPixelClock = 148500000 ;
    pixelrep = 0 ;
    aspec = HDMI_16x9 ;
    Colorimetry = HDMI_ITU709 ;
    break ;
  case HDMI_1080p24:
    VIC = 32 ;
    VideoPixelClock = 74250000 ;
    pixelrep = 0 ;
    aspec = HDMI_16x9 ;
    Colorimetry = HDMI_ITU709 ;
    break ;
  case HDMI_1080p25:
    VIC = 33 ;
    VideoPixelClock = 74250000 ;
    pixelrep = 0 ;
    aspec = HDMI_16x9 ;
    Colorimetry = HDMI_ITU709 ;
    break ;
  case HDMI_1080p30:
    VIC = 34 ;
    VideoPixelClock = 74250000 ;
    pixelrep = 0 ;
    aspec = HDMI_16x9 ;
    Colorimetry = HDMI_ITU709 ;
    break ;

  case HDMI_720p30:
    VIC = 0 ;
    VideoPixelClock = 74250000 ;
    pixelrep = 0 ;
    aspec = HDMI_16x9 ;
    Colorimetry = HDMI_ITU709 ;

#ifdef SUPPORT_SYNCEMBEDDED
    /*
       VTiming.HActive=1280 ;
       VTiming.VActive=720 ;
       VTiming.HTotal=3300 ;
       VTiming.VTotal=750 ;
       VTiming.PCLK=VideoPixelClock ;
       VTiming.xCnt=0x2E ;
       VTiming.HFrontPorch= 1760;
       VTiming.HSyncWidth= 40 ;
       VTiming.HBackPorch= 220 ;
       VTiming.VFrontPorch= 5;
       VTiming.VSyncWidth= 5 ;
       VTiming.VBackPorch= 20 ;
       VTiming.ScanMode=PROG ;
       VTiming.VPolarity=Vneg ;
       VTiming.HPolarity=Hneg ;
       */
#endif
    break ;
  default:
    bChangeMode = FALSE ;
    return ;
  }
  switch(OutputColorMode)
  {
  case HDMI_YUV444:
    bOutputColorMode = F_MODE_YUV444 ;
    break ;
  case HDMI_YUV422:
    bOutputColorMode = F_MODE_YUV422 ;
    break ;
  case HDMI_RGB444:
  default:
    bOutputColorMode = F_MODE_RGB444 ;
    break ;
  }
  if( Colorimetry == HDMI_ITU709 )
  {
    bInputColorMode |= F_VIDMODE_ITU709 ;
  }
  else
  {
    bInputColorMode &= ~F_VIDMODE_ITU709 ;
  }
  // if( Colorimetry != HDMI_640x480p60)
  if( OutputVideoTiming != HDMI_640x480p60)
  {
    bInputColorMode |= F_VIDMODE_16_235 ;
  }
  else
  {
    bInputColorMode &= ~F_VIDMODE_16_235 ;
  }
  bChangeMode = TRUE ;
}
UINT8 AudioDelayCnt=0;
UINT8 LastRefaudfreqnum=0;
UINT8 bForceCTS = FALSE;

UINT8 CommunBuff[128] ;

void HDMITX_DisableAudioOutput()
{
  //UINT8 uc = (HDMITX_ReadI2C_Byte(REG_TX_SW_RST) | (B_HDMITX_AUD_RST | B_TX_AREF_RST));
  //HDMITX_WriteI2C_Byte(REG_TX_SW_RST,uc);
  AudioDelayCnt=250;
  LastRefaudfreqnum=0;
  HDMITX_SetI2C_Byte(REG_TX_SW_RST, (B_HDMITX_AUD_RST | B_TX_AREF_RST), (B_HDMITX_AUD_RST | B_TX_AREF_RST) );
  HDMITX_SetI2C_Byte(0x0F, 0x10, 0x10 );
}

UINT8 setHDMITX_VideoSignalType(UINT8 inputSignalType)
{
  hdmiTxDev[0].bInputVideoSignalType = inputSignalType ;
  // hdmitx_SetInputMode(inputColorMode,hdmiTxDev[0].bInputVideoSignalType);
  return TRUE ;
}

void setHDMITX_AVMute(UINT8 bEnable)
{
  Switch_HDMITX_Bank(0);
  HDMITX_SetI2C_Byte(REG_TX_GCP,B_TX_SETAVMUTE, bEnable?B_TX_SETAVMUTE:0 );
  HDMITX_WriteI2C_Byte(REG_TX_PKT_GENERAL_CTRL,B_TX_ENABLE_PKT|B_TX_REPEAT_PKT);
}

void hdmitx_SetInputMode(UINT8 InputColorMode,UINT8 bInputSignalType)
{
  UINT8 ucData ;

  ucData = HDMITX_ReadI2C_Byte(REG_TX_INPUT_MODE);
  ucData &= ~(M_TX_INCOLMOD|B_TX_2X656CLK|B_TX_SYNCEMB|B_TX_INDDR|B_TX_PCLKDIV2);
  ucData |= INPUT_CLOCK_DELAY ;//input clock delay 1 for 1080P DDR

  switch(InputColorMode & F_MODE_CLRMOD_MASK)
  {
  case F_MODE_YUV422:
    ucData |= B_TX_IN_YUV422 ;
    break ;
  case F_MODE_YUV444:
    ucData |= B_TX_IN_YUV444 ;
    break ;
  case F_MODE_RGB444:
  default:
    ucData |= B_TX_IN_RGB ;
    break ;
  }
  if(bInputSignalType & T_MODE_PCLKDIV2)
  {
    ucData |= B_TX_PCLKDIV2 ; DeBug(EFI_D_INFO, "PCLK Divided by 2 mode\n");
  }
  if(bInputSignalType & T_MODE_CCIR656)
  {
    ucData |= B_TX_2X656CLK ; DeBug(EFI_D_INFO, "CCIR656 mode\n");
  }
  if(bInputSignalType & T_MODE_SYNCEMB)
  {
    ucData |= B_TX_SYNCEMB ; DeBug(EFI_D_INFO, "Sync Embedded mode\n");
  }
  if(bInputSignalType & T_MODE_INDDR)
  {
    ucData |= B_TX_INDDR ; DeBug(EFI_D_INFO, "Input DDR mode\n");
  }
  HDMITX_WriteI2C_Byte(REG_TX_INPUT_MODE,ucData);
}

void hdmitx_SetCSCScale(UINT8 bInputMode,UINT8 bOutputMode)
{
  UINT8 ucData,csc = 0 ;
  UINT8 i ;
  UINT8 filter = 0 ; // filter is for Video CTRL DN_FREE_GO,EN_DITHER,and ENUDFILT

  // (1) YUV422 in,RGB/YUV444 output (Output is 8-bit,input is 12-bit)
  // (2) YUV444/422  in,RGB output (CSC enable,and output is not YUV422)
  // (3) RGB in,YUV444 output   (CSC enable,and output is not YUV422)
  //
  // YUV444/RGB24 <-> YUV422 need set up/down filter.
  DeBug(EFI_D_INFO, "hdmitx_SetCSCScale(UINT8 bInputMode = %x,UINT8 bOutputMode = %x)\n", (int)bInputMode, (int)bOutputMode) ;
  switch(bInputMode&F_MODE_CLRMOD_MASK)
  {
#ifdef SUPPORT_INPUTYUV444
  case F_MODE_YUV444:
    DeBug(EFI_D_INFO, "Input mode is YUV444 ");
    switch(bOutputMode&F_MODE_CLRMOD_MASK)
    {
    case F_MODE_YUV444:
      DeBug(EFI_D_INFO, "Output mode is YUV444\n");
      csc = B_HDMITX_CSC_BYPASS ;
      break ;

    case F_MODE_YUV422:
      DeBug(EFI_D_INFO, "Output mode is YUV422\n");
      if(bInputMode & F_VIDMODE_EN_UDFILT) // YUV444 to YUV422 need up/down filter for processing.
      {
        filter |= B_TX_EN_UDFILTER ;
      }
      csc = B_HDMITX_CSC_BYPASS ;
      break ;
    case F_MODE_RGB444:
      DeBug(EFI_D_INFO, "Output mode is RGB24\n");
      csc = B_HDMITX_CSC_YUV2RGB ;
      if(bInputMode & F_VIDMODE_EN_DITHER) // YUV444 to RGB24 need dither
      {
        filter |= B_TX_EN_DITHER | B_TX_DNFREE_GO ;
      }
      break ;
    }
    break ;
#endif

#ifdef SUPPORT_INPUTYUV422
  case F_MODE_YUV422:
    DeBug(EFI_D_INFO, "Input mode is YUV422\n");
    switch(bOutputMode&F_MODE_CLRMOD_MASK)
    {
    case F_MODE_YUV444:
      DeBug(EFI_D_INFO, "Output mode is YUV444\n");
      csc = B_HDMITX_CSC_BYPASS ;
      if(bInputMode & F_VIDMODE_EN_UDFILT) // YUV422 to YUV444 need up filter
      {
        filter |= B_TX_EN_UDFILTER ;
      }
      if(bInputMode & F_VIDMODE_EN_DITHER) // YUV422 to YUV444 need dither
      {
        filter |= B_TX_EN_DITHER | B_TX_DNFREE_GO ;
      }
      break ;
    case F_MODE_YUV422:
      DeBug(EFI_D_INFO, "Output mode is YUV422\n");
      csc = B_HDMITX_CSC_BYPASS ;

      break ;

    case F_MODE_RGB444:
      DeBug(EFI_D_INFO, "Output mode is RGB24\n");
      csc = B_HDMITX_CSC_YUV2RGB ;
      if(bInputMode & F_VIDMODE_EN_UDFILT) // YUV422 to RGB24 need up/dn filter.
      {
        filter |= B_TX_EN_UDFILTER ;
      }
      if(bInputMode & F_VIDMODE_EN_DITHER) // YUV422 to RGB24 need dither
      {
        filter |= B_TX_EN_DITHER | B_TX_DNFREE_GO ;
      }
      break ;
    }
    break ;
#endif

#ifdef SUPPORT_INPUTRGB
  case F_MODE_RGB444:
    DeBug(EFI_D_INFO, "Input mode is RGB24\n");
    switch(bOutputMode&F_MODE_CLRMOD_MASK)
    {
    case F_MODE_YUV444:
      DeBug(EFI_D_INFO, "Output mode is YUV444\n");
      csc = B_HDMITX_CSC_RGB2YUV ;

      if(bInputMode & F_VIDMODE_EN_DITHER) // RGB24 to YUV444 need dither
      {
        filter |= B_TX_EN_DITHER | B_TX_DNFREE_GO ;
      }
      break ;

    case F_MODE_YUV422:
      DeBug(EFI_D_INFO, "Output mode is YUV422\n");
      if(bInputMode & F_VIDMODE_EN_UDFILT) // RGB24 to YUV422 need down filter.
      {
        filter |= B_TX_EN_UDFILTER ;
      }
      if(bInputMode & F_VIDMODE_EN_DITHER) // RGB24 to YUV422 need dither
      {
        filter |= B_TX_EN_DITHER | B_TX_DNFREE_GO ;
      }
      csc = B_HDMITX_CSC_RGB2YUV ;
      break ;

    case F_MODE_RGB444:
      DeBug(EFI_D_INFO, "Output mode is RGB24\n");
      csc = B_HDMITX_CSC_BYPASS ;
      break ;
    }
    break ;
#endif
  }
#ifndef DISABLE_HDMITX_CSC

#ifdef SUPPORT_INPUTRGB
  // set the CSC metrix registers by colorimetry and quantization
  if(csc == B_HDMITX_CSC_RGB2YUV)
  {
    DeBug(EFI_D_INFO, "CSC = RGB2YUV %x ",csc);
    switch(bInputMode&(F_VIDMODE_ITU709|F_VIDMODE_16_235))
    {
    case F_VIDMODE_ITU709|F_VIDMODE_16_235:
      DeBug(EFI_D_INFO, "ITU709 16-235 ");
      for( i = 0 ; i < SIZEOF_CSCMTX ; i++ ){ HDMITX_WriteI2C_Byte(REG_TX_CSC_YOFF+i,bCSCMtx_RGB2YUV_ITU709_16_235[i]) ;
        DeBug(EFI_D_INFO, "reg%02X <- %02X\n",(int)(i+REG_TX_CSC_YOFF),(int)bCSCMtx_RGB2YUV_ITU709_16_235[i]);}
      break ;
    case F_VIDMODE_ITU709|F_VIDMODE_0_255:
      DeBug(EFI_D_INFO, "ITU709 0-255 ");
      for( i = 0 ; i < SIZEOF_CSCMTX ; i++ ){ HDMITX_WriteI2C_Byte(REG_TX_CSC_YOFF+i,bCSCMtx_RGB2YUV_ITU709_0_255[i]) ;
        DeBug(EFI_D_INFO, "reg%02X <- %02X\n",(int)(i+REG_TX_CSC_YOFF),(int)bCSCMtx_RGB2YUV_ITU709_0_255[i]);}
      break ;
    case F_VIDMODE_ITU601|F_VIDMODE_16_235:
      DeBug(EFI_D_INFO, "ITU601 16-235 ");
      for( i = 0 ; i < SIZEOF_CSCMTX ; i++ ){ HDMITX_WriteI2C_Byte(REG_TX_CSC_YOFF+i,bCSCMtx_RGB2YUV_ITU601_16_235[i]) ;
        DeBug(EFI_D_INFO, "reg%02X <- %02X\n",(int)(i+REG_TX_CSC_YOFF),(int)bCSCMtx_RGB2YUV_ITU601_16_235[i]);}
      break ;
    case F_VIDMODE_ITU601|F_VIDMODE_0_255:
    default:
      DeBug(EFI_D_INFO, "ITU601 0-255 ");
      for( i = 0 ; i < SIZEOF_CSCMTX ; i++ ){ HDMITX_WriteI2C_Byte(REG_TX_CSC_YOFF+i,bCSCMtx_RGB2YUV_ITU601_0_255[i]) ;
        DeBug(EFI_D_INFO, "reg%02X <- %02X\n",(int)(i+REG_TX_CSC_YOFF),(int)bCSCMtx_RGB2YUV_ITU601_0_255[i]);}
      break ;
    }
  }
#endif

#ifdef SUPPORT_INPUTYUV
  if (csc == B_HDMITX_CSC_YUV2RGB)
  {
    DeBug(EFI_D_INFO, "CSC = YUV2RGB %x ",csc);

    switch(bInputMode&(F_VIDMODE_ITU709|F_VIDMODE_16_235))
    {
    case F_VIDMODE_ITU709|F_VIDMODE_16_235:
      DeBug(EFI_D_INFO, "ITU709 16-235 ");
      for( i = 0 ; i < SIZEOF_CSCMTX ; i++ ){ HDMITX_WriteI2C_Byte(REG_TX_CSC_YOFF+i,bCSCMtx_YUV2RGB_ITU709_16_235[i]) ;
        HDMITX_DEBUG_PRINTF(("reg%02X <- %02X\n",(int)(i+REG_TX_CSC_YOFF),(int)bCSCMtx_YUV2RGB_ITU709_16_235[i]));}
      break ;
    case F_VIDMODE_ITU709|F_VIDMODE_0_255:
      DeBug(EFI_D_INFO, "ITU709 0-255 ");
      for( i = 0 ; i < SIZEOF_CSCMTX ; i++ ){ HDMITX_WriteI2C_Byte(REG_TX_CSC_YOFF+i,bCSCMtx_YUV2RGB_ITU709_0_255[i]) ;
        HDMITX_DEBUG_PRINTF(("reg%02X <- %02X\n",(int)(i+REG_TX_CSC_YOFF),(int)bCSCMtx_YUV2RGB_ITU709_0_255[i]));}
      break ;
    case F_VIDMODE_ITU601|F_VIDMODE_16_235:
      DeBug(EFI_D_INFO, "ITU601 16-235 ");
      for( i = 0 ; i < SIZEOF_CSCMTX ; i++ ){ HDMITX_WriteI2C_Byte(REG_TX_CSC_YOFF+i,bCSCMtx_YUV2RGB_ITU601_16_235[i]) ;
        HDMITX_DEBUG_PRINTF(("reg%02X <- %02X\n",(int)(i+REG_TX_CSC_YOFF),(int)bCSCMtx_YUV2RGB_ITU601_16_235[i]));}
      break ;
    case F_VIDMODE_ITU601|F_VIDMODE_0_255:
    default:
      DeBug(EFI_D_INFO, "ITU601 0-255 ");
      for( i = 0 ; i < SIZEOF_CSCMTX ; i++ ){ HDMITX_WriteI2C_Byte(REG_TX_CSC_YOFF+i,bCSCMtx_YUV2RGB_ITU601_0_255[i]) ;
        HDMITX_DEBUG_PRINTF(("reg%02X <- %02X\n",(int)(i+REG_TX_CSC_YOFF),(int)bCSCMtx_YUV2RGB_ITU601_0_255[i]));}
      break ;
    }
  }
#endif
#else// DISABLE_HDMITX_CSC
  csc = B_HDMITX_CSC_BYPASS ;
#endif// DISABLE_HDMITX_CSC

  if( csc == B_HDMITX_CSC_BYPASS )
  {
    HDMITX_SetI2C_Byte(0xF, 0x10, 0x10);
  }
  else
  {
    HDMITX_SetI2C_Byte(0xF, 0x10, 0x00);
  }
  ucData = HDMITX_ReadI2C_Byte(REG_TX_CSC_CTRL) & ~(M_TX_CSC_SEL|B_TX_DNFREE_GO|B_TX_EN_DITHER|B_TX_EN_UDFILTER);
  ucData |= filter|csc ;

  HDMITX_WriteI2C_Byte(REG_TX_CSC_CTRL,ucData);

  // set output Up/Down Filter,Dither control

}

void hdmitx_SetupAFE(VIDEOPCLKLEVEL level)
{

  HDMITX_WriteI2C_Byte(REG_TX_AFE_DRV_CTRL,B_TX_AFE_DRV_RST);/* 0x10 */
  switch(level)
  {
  case PCLK_HIGH:
    HDMITX_SetI2C_Byte(0x62, 0x90, 0x80);
    HDMITX_SetI2C_Byte(0x64, 0x89, 0x80);
    HDMITX_SetI2C_Byte(0x68, 0x10, 0x80);
    DeBug(EFI_D_INFO, "hdmitx_SetupAFE()===================HIGHT\n");
    break ;
  default:
    HDMITX_SetI2C_Byte(0x62, 0x90, 0x10);
    HDMITX_SetI2C_Byte(0x64, 0x89, 0x09);
    HDMITX_SetI2C_Byte(0x68, 0x10, 0x10);
    DeBug(EFI_D_INFO, "hdmitx_SetupAFE()===================LOW\n");
    break ;
  }
#ifdef REDUCE_HDMITX_SRC_JITTER
  HDMITX_SetI2C_Byte(0x64, 0x01, 0x00);
  HDMITX_SetI2C_Byte(0x6A, 0xFF, 0xFF);
#endif // REDUCE_HDMITX_SRC_JITTER
  HDMITX_SetI2C_Byte(REG_TX_SW_RST,B_TX_REF_RST_HDMITX|B_HDMITX_VID_RST,0);
  HDMITX_WriteI2C_Byte(REG_TX_AFE_DRV_CTRL,0);
  DELAY_IT6612(1000);
}

//////////////////////////////////////////////////////////////////////
// Function: hdmitx_FireAFE
// Parameter: N/A
// Return: N/A
// Remark: write reg61 with 0x04
//         When program reg61 with 0x04,then audio and video circuit work.
// Side-Effect: N/A
//////////////////////////////////////////////////////////////////////
void hdmitx_FireAFE()
{
  Switch_HDMITX_Bank(0);
  HDMITX_WriteI2C_Byte(REG_TX_AFE_DRV_CTRL,0);
}

UINT8 HDMITX_EnableVideoOutput(VIDEOPCLKLEVEL level,UINT8 inputColorMode,UINT8 outputColorMode,UINT8 bHDMI)
{
#ifdef INVERT_VID_LATCHEDGE
  UINT8 uc = 0 ;
#endif // INVERT_VID_LATCHEDGE

  // bInputVideoMode,bOutputVideoMode,hdmiTxDev[0].bInputVideoSignalType,bAudioInputType,should be configured by upper F/W or loaded from EEPROM.
  // should be configured by initsys.c
  // VIDEOPCLKLEVEL level ;

  HDMITX_WriteI2C_Byte(REG_TX_SW_RST,B_HDMITX_VID_RST|B_HDMITX_AUD_RST|B_TX_AREF_RST|B_TX_HDCP_RST_HDMITX);

  hdmiTxDev[0].bHDMIMode = (UINT8)bHDMI ;
  // 2009/12/09 added by jau-chih.tseng@ite.com.tw
  Switch_HDMITX_Bank(1);
  HDMITX_WriteI2C_Byte(REG_TX_AVIINFO_DB1,0x00);
  Switch_HDMITX_Bank(0);
  //~jau-chih.tseng@ite.com.tw

  if(hdmiTxDev[0].bHDMIMode)
  {
    setHDMITX_AVMute(TRUE);
  }
  hdmitx_SetInputMode(inputColorMode,hdmiTxDev[0].bInputVideoSignalType);

  hdmitx_SetCSCScale(inputColorMode,outputColorMode);

  if(hdmiTxDev[0].bHDMIMode)
  {
    HDMITX_WriteI2C_Byte(REG_TX_HDMI_MODE,B_TX_HDMI_MODE);
  }
  else
  {
    HDMITX_WriteI2C_Byte(REG_TX_HDMI_MODE,B_TX_DVI_MODE);
  }
#ifdef INVERT_VID_LATCHEDGE
  uc = HDMITX_ReadI2C_Byte(REG_TX_CLK_CTRL1);
  uc |= B_TX_VDO_LATCH_EDGE ;
  HDMITX_WriteI2C_Byte(REG_TX_CLK_CTRL1, uc);
#endif

  hdmitx_SetupAFE(level); // pass if High Freq request
  HDMITX_WriteI2C_Byte(REG_TX_SW_RST,          B_HDMITX_AUD_RST|B_TX_AREF_RST|B_TX_HDCP_RST_HDMITX);

  hdmitx_FireAFE();

  return TRUE ;
}

EFI_STATUS
hdmitx_SetAVIInfoFrame(AVI_InfoFrame *pAVIInfoFrame)
{
  int i ;
  UINT8 checksum ;

  if(!pAVIInfoFrame)
  {
    return EFI_UNSUPPORTED;
  }
  Switch_HDMITX_Bank(1);
  HDMITX_WriteI2C_Byte(REG_TX_AVIINFO_DB1,pAVIInfoFrame->pktbyte.AVI_DB[0]);
  HDMITX_WriteI2C_Byte(REG_TX_AVIINFO_DB2,pAVIInfoFrame->pktbyte.AVI_DB[1]);
  HDMITX_WriteI2C_Byte(REG_TX_AVIINFO_DB3,pAVIInfoFrame->pktbyte.AVI_DB[2]);
  HDMITX_WriteI2C_Byte(REG_TX_AVIINFO_DB4,pAVIInfoFrame->pktbyte.AVI_DB[3]);
  HDMITX_WriteI2C_Byte(REG_TX_AVIINFO_DB5,pAVIInfoFrame->pktbyte.AVI_DB[4]);
  HDMITX_WriteI2C_Byte(REG_TX_AVIINFO_DB6,pAVIInfoFrame->pktbyte.AVI_DB[5]);
  HDMITX_WriteI2C_Byte(REG_TX_AVIINFO_DB7,pAVIInfoFrame->pktbyte.AVI_DB[6]);
  HDMITX_WriteI2C_Byte(REG_TX_AVIINFO_DB8,pAVIInfoFrame->pktbyte.AVI_DB[7]);
  HDMITX_WriteI2C_Byte(REG_TX_AVIINFO_DB9,pAVIInfoFrame->pktbyte.AVI_DB[8]);
  HDMITX_WriteI2C_Byte(REG_TX_AVIINFO_DB10,pAVIInfoFrame->pktbyte.AVI_DB[9]);
  HDMITX_WriteI2C_Byte(REG_TX_AVIINFO_DB11,pAVIInfoFrame->pktbyte.AVI_DB[10]);
  HDMITX_WriteI2C_Byte(REG_TX_AVIINFO_DB12,pAVIInfoFrame->pktbyte.AVI_DB[11]);
  HDMITX_WriteI2C_Byte(REG_TX_AVIINFO_DB13,pAVIInfoFrame->pktbyte.AVI_DB[12]);
  for(i = 0,checksum = 0; i < 13 ; i++)
  {
    checksum -= pAVIInfoFrame->pktbyte.AVI_DB[i] ;
  }
  /*
     HDMITX_DEBUG_PRINTF(("SetAVIInfo(): "));
     HDMITX_DEBUG_PRINTF(("%02X ",(int)HDMITX_ReadI2C_Byte(REG_TX_AVIINFO_DB1)));
     HDMITX_DEBUG_PRINTF(("%02X ",(int)HDMITX_ReadI2C_Byte(REG_TX_AVIINFO_DB2)));
     HDMITX_DEBUG_PRINTF(("%02X ",(int)HDMITX_ReadI2C_Byte(REG_TX_AVIINFO_DB3)));
     HDMITX_DEBUG_PRINTF(("%02X ",(int)HDMITX_ReadI2C_Byte(REG_TX_AVIINFO_DB4)));
     HDMITX_DEBUG_PRINTF(("%02X ",(int)HDMITX_ReadI2C_Byte(REG_TX_AVIINFO_DB5)));
     HDMITX_DEBUG_PRINTF(("%02X ",(int)HDMITX_ReadI2C_Byte(REG_TX_AVIINFO_DB6)));
     HDMITX_DEBUG_PRINTF(("%02X ",(int)HDMITX_ReadI2C_Byte(REG_TX_AVIINFO_DB7)));
     HDMITX_DEBUG_PRINTF(("%02X ",(int)HDMITX_ReadI2C_Byte(REG_TX_AVIINFO_DB8)));
     HDMITX_DEBUG_PRINTF(("%02X ",(int)HDMITX_ReadI2C_Byte(REG_TX_AVIINFO_DB9)));
     HDMITX_DEBUG_PRINTF(("%02X ",(int)HDMITX_ReadI2C_Byte(REG_TX_AVIINFO_DB10)));
     HDMITX_DEBUG_PRINTF(("%02X ",(int)HDMITX_ReadI2C_Byte(REG_TX_AVIINFO_DB11)));
     HDMITX_DEBUG_PRINTF(("%02X ",(int)HDMITX_ReadI2C_Byte(REG_TX_AVIINFO_DB12)));
     HDMITX_DEBUG_PRINTF(("%02X ",(int)HDMITX_ReadI2C_Byte(REG_TX_AVIINFO_DB13)));
     HDMITX_DEBUG_PRINTF(("\n"));
     */
  checksum -= AVI_INFOFRAME_VER+AVI_INFOFRAME_TYPE+AVI_INFOFRAME_LEN ;
  HDMITX_WriteI2C_Byte(REG_TX_AVIINFO_SUM,checksum);

  Switch_HDMITX_Bank(0);
  hdmitx_ENABLE_AVI_INFOFRM_PKT();
  return EFI_SUCCESS ;
}

UINT8 HDMITX_EnableAVIInfoFrame(UINT8 bEnable,UINT8 *pAVIInfoFrame)
{
  if(!bEnable)
  {
    hdmitx_DISABLE_AVI_INFOFRM_PKT();
    return TRUE ;
  }
  if(hdmitx_SetAVIInfoFrame((AVI_InfoFrame *)pAVIInfoFrame) == EFI_SUCCESS)
  {
    return TRUE ;
  }
  return FALSE ;
}

void ConfigAVIInfoFrame(UINT8 VIC, UINT8 pixelrep)
{
  AVI_InfoFrame *AviInfo;
  AviInfo = (AVI_InfoFrame *)CommunBuff ;

  AviInfo->pktbyte.AVI_HB[0] = AVI_INFOFRAME_TYPE|0x80 ;
  AviInfo->pktbyte.AVI_HB[1] = AVI_INFOFRAME_VER ;
  AviInfo->pktbyte.AVI_HB[2] = AVI_INFOFRAME_LEN ;

  switch(bOutputColorMode)
  {
  case F_MODE_YUV444:
    // AviInfo->info.ColorMode = 2 ;
    AviInfo->pktbyte.AVI_DB[0] = (2<<5)|(1<<4);
    break ;
  case F_MODE_YUV422:
    // AviInfo->info.ColorMode = 1 ;
    AviInfo->pktbyte.AVI_DB[0] = (1<<5)|(1<<4);
    break ;
  case F_MODE_RGB444:
  default:
    // AviInfo->info.ColorMode = 0 ;
    AviInfo->pktbyte.AVI_DB[0] = (0<<5)|(1<<4);
    break ;
  }
  AviInfo->pktbyte.AVI_DB[1] = 8 ;
  AviInfo->pktbyte.AVI_DB[1] |= (aspec != HDMI_16x9)?(1<<4):(2<<4); // 4:3 or 16:9
  AviInfo->pktbyte.AVI_DB[1] |= (Colorimetry != HDMI_ITU709)?(1<<6):(2<<6); // 4:3 or 16:9
  AviInfo->pktbyte.AVI_DB[2] = 0 ;
  AviInfo->pktbyte.AVI_DB[3] = VIC ;
  AviInfo->pktbyte.AVI_DB[4] =  pixelrep & 3 ;
  AviInfo->pktbyte.AVI_DB[5] = 0 ;
  AviInfo->pktbyte.AVI_DB[6] = 0 ;
  AviInfo->pktbyte.AVI_DB[7] = 0 ;
  AviInfo->pktbyte.AVI_DB[8] = 0 ;
  AviInfo->pktbyte.AVI_DB[9] = 0 ;
  AviInfo->pktbyte.AVI_DB[10] = 0 ;
  AviInfo->pktbyte.AVI_DB[11] = 0 ;
  AviInfo->pktbyte.AVI_DB[12] = 0 ;

  HDMITX_EnableAVIInfoFrame(TRUE, (unsigned char *)AviInfo);
}

EFI_STATUS hdmitx_SetVSIInfoFrame(VendorSpecific_InfoFrame *pVSIInfoFrame)
{
  UINT8 ucData=0 ;

  if(!pVSIInfoFrame)
  {
    return EFI_UNSUPPORTED;
  }

  Switch_HDMITX_Bank(1);
  HDMITX_WriteI2C_Byte(0x80,pVSIInfoFrame->pktbyte.VS_DB[3]);
  HDMITX_WriteI2C_Byte(0x81,pVSIInfoFrame->pktbyte.VS_DB[4]);

  ucData -= pVSIInfoFrame->pktbyte.VS_DB[3] ;
  ucData -= pVSIInfoFrame->pktbyte.VS_DB[4] ;

  if(  pVSIInfoFrame->pktbyte.VS_DB[4] & (1<<7 ))
  {
    ucData -= pVSIInfoFrame->pktbyte.VS_DB[5] ;
    HDMITX_WriteI2C_Byte(0x82,pVSIInfoFrame->pktbyte.VS_DB[5]);
    ucData -= VENDORSPEC_INFOFRAME_TYPE + VENDORSPEC_INFOFRAME_VER + 6 + 0x0C + 0x03 ;
  }
  else
  {
    ucData -= VENDORSPEC_INFOFRAME_TYPE + VENDORSPEC_INFOFRAME_VER + 5 + 0x0C + 0x03 ;
  }

  pVSIInfoFrame->pktbyte.CheckSum=ucData;

  HDMITX_WriteI2C_Byte(0x83,pVSIInfoFrame->pktbyte.CheckSum);
  Switch_HDMITX_Bank(0);
  HDMITX_WriteI2C_Byte(REG_TX_3D_INFO_CTRL,B_TX_ENABLE_PKT|B_TX_REPEAT_PKT);
  return EFI_SUCCESS ;
}

UINT8 HDMITX_EnableVSInfoFrame(UINT8 bEnable,UINT8 *pVSInfoFrame)
{
  if(!bEnable)
  {
    hdmitx_DISABLE_VSDB_PKT();
    return TRUE ;
  }
  if(hdmitx_SetVSIInfoFrame((VendorSpecific_InfoFrame *)pVSInfoFrame) == EFI_SUCCESS)
  {
    return TRUE ;
  }
  return FALSE ;
}

void HDMITX_PowerOn()
{
  hdmitx_LoadRegSetting(HDMITX_PwrOn_Table);
}

EFI_STATUS
hdmitx_SetAudioInfoFrame(Audio_InfoFrame *pAudioInfoFrame)
{
  UINT8 checksum ;

  if(!pAudioInfoFrame)
  {
    return EFI_UNSUPPORTED;
  }
  Switch_HDMITX_Bank(1);
  checksum = 0x100-(AUDIO_INFOFRAME_VER+AUDIO_INFOFRAME_TYPE+AUDIO_INFOFRAME_LEN );
  HDMITX_WriteI2C_Byte(REG_TX_PKT_AUDINFO_CC,pAudioInfoFrame->pktbyte.AUD_DB[0]);
  checksum -= HDMITX_ReadI2C_Byte(REG_TX_PKT_AUDINFO_CC); checksum &= 0xFF ;
  HDMITX_WriteI2C_Byte(REG_TX_PKT_AUDINFO_SF,pAudioInfoFrame->pktbyte.AUD_DB[1]);
  checksum -= HDMITX_ReadI2C_Byte(REG_TX_PKT_AUDINFO_SF); checksum &= 0xFF ;
  HDMITX_WriteI2C_Byte(REG_TX_PKT_AUDINFO_CA,pAudioInfoFrame->pktbyte.AUD_DB[3]);
  checksum -= HDMITX_ReadI2C_Byte(REG_TX_PKT_AUDINFO_CA); checksum &= 0xFF ;
  HDMITX_WriteI2C_Byte(REG_TX_PKT_AUDINFO_DM_LSV,pAudioInfoFrame->pktbyte.AUD_DB[4]);
  checksum -= HDMITX_ReadI2C_Byte(REG_TX_PKT_AUDINFO_DM_LSV); checksum &= 0xFF ;

  HDMITX_WriteI2C_Byte(REG_TX_PKT_AUDINFO_SUM,checksum);

  Switch_HDMITX_Bank(0);
  hdmitx_ENABLE_AUD_INFOFRM_PKT();
  return EFI_SUCCESS ;
}

UINT8 HDMITX_EnableAudioInfoFrame(UINT8 bEnable,UINT8 *pAudioInfoFrame)
{
  if(!bEnable)
  {
    hdmitx_DISABLE_AVI_INFOFRM_PKT();
    return TRUE ;
  }
  if(hdmitx_SetAudioInfoFrame((Audio_InfoFrame *)pAudioInfoFrame) == EFI_SUCCESS)
  {
    return TRUE ;
  }
  return FALSE ;
}

void ConfigAudioInfoFrm()
{
  UINT8 i ;

  Audio_InfoFrame *AudioInfo ;
  AudioInfo = (Audio_InfoFrame *)CommunBuff ;

  DeBug(EFI_D_INFO, "ConfigAudioInfoFrm(%d)\n",2);

  AudioInfo->pktbyte.AUD_HB[0] = AUDIO_INFOFRAME_TYPE ;
  AudioInfo->pktbyte.AUD_HB[1] = 1 ;
  AudioInfo->pktbyte.AUD_HB[2] = AUDIO_INFOFRAME_LEN ;
  AudioInfo->pktbyte.AUD_DB[0] = 1 ;
  for( i = 1 ;i < AUDIO_INFOFRAME_LEN ; i++ )
  {
    AudioInfo->pktbyte.AUD_DB[i] = 0 ;
  }
  HDMITX_EnableAudioInfoFrame(TRUE, (unsigned char *)AudioInfo);
}

void setHDMITX_NCTS(UINT8 Fs)
{
  UINT32 n;
  UINT8 LoopCnt=255,CTSStableCnt=0;
  UINT32 diff;
  UINT32 CTS=0,LastCTS=0;
  UINT8 HBR_mode;
  // BYTE aVIC;

  if(B_TX_HBR & HDMITX_ReadI2C_Byte(REG_TX_AUD_HDAUDIO))
  {
    HBR_mode=TRUE;
  }
  else
  {
    HBR_mode=FALSE;
  }
  switch(Fs)
  {
  case AUDFS_32KHz: n = 4096; break;
  case AUDFS_44p1KHz: n = 6272; break;
  case AUDFS_48KHz: n = 6144; break;
  case AUDFS_88p2KHz: n = 12544; break;
  case AUDFS_96KHz: n = 12288; break;
  case AUDFS_176p4KHz: n = 25088; break;
  case AUDFS_192KHz: n = 24576; break;
  case AUDFS_768KHz: n = 24576; break ;
  default: n = 6144;
  }
  // tr_printf((" n = %ld\n",n));
  Switch_HDMITX_Bank(1);
  HDMITX_WriteI2C_Byte(REGPktAudN0,(UINT8)((n)&0xFF));
  HDMITX_WriteI2C_Byte(REGPktAudN1,(UINT8)((n>>8)&0xFF));
  HDMITX_WriteI2C_Byte(REGPktAudN2,(UINT8)((n>>16)&0xF));

  if(bForceCTS)
  {
    UINT32 SumCTS=0;
    while(LoopCnt--)
    {
      DELAY_IT6612(1000);
      CTS = ((unsigned long)HDMITX_ReadI2C_Byte(REGPktAudCTSCnt2)) << 12 ;
      CTS |= ((unsigned long)HDMITX_ReadI2C_Byte(REGPktAudCTSCnt1)) <<4 ;
      CTS |= ((unsigned long)HDMITX_ReadI2C_Byte(REGPktAudCTSCnt0)&0xf0)>>4  ;
      if( CTS == 0)
      {
        continue;
      }
      else
      {
        if(LastCTS>CTS )
        {diff=LastCTS-CTS;}
        else
        {diff=CTS-LastCTS;}
        //HDMITX_DEBUG_PRINTF(("LastCTS= %u%u",(WORD)(LastCTS/10000),(WORD)(LastCTS%10000)));
        //HDMITX_DEBUG_PRINTF(("       CTS= %u%u\n",(WORD)(CTS/10000),(WORD)(CTS%10000)));
        LastCTS=CTS;
        if(5>diff)
        {
          CTSStableCnt++;
          SumCTS+=CTS;
        }
        else
        {
          CTSStableCnt=0;
          SumCTS=0;
          continue;
        }
        if(CTSStableCnt>=32)
        {
          LastCTS=(SumCTS>>5);
          break;
        }
      }
    }
  }
  HDMITX_WriteI2C_Byte(REGPktAudCTS0,(UINT8)((LastCTS)&0xFF));
  HDMITX_WriteI2C_Byte(REGPktAudCTS1,(UINT8)((LastCTS>>8)&0xFF));
  HDMITX_WriteI2C_Byte(REGPktAudCTS2,(UINT8)((LastCTS>>16)&0xF));
  Switch_HDMITX_Bank(0);
#ifdef Force_CTS
  bForceCTS = TRUE;
#endif
  HDMITX_WriteI2C_Byte(0xF8, 0xC3);
  HDMITX_WriteI2C_Byte(0xF8, 0xA5);
  if(bForceCTS)
  {
    HDMITX_OrReg_Byte(REG_TX_PKT_SINGLE_CTRL,B_TX_SW_CTS); // D[1] = 0, HW auto count CTS
  }
  else
  {
    HDMITX_AndReg_Byte(REG_TX_PKT_SINGLE_CTRL,~B_TX_SW_CTS); // D[1] = 0, HW auto count CTS
  }
  HDMITX_WriteI2C_Byte(0xF8, 0xFF);

  if(FALSE==HBR_mode) //LPCM
  {
    UINT8 uData;
    Switch_HDMITX_Bank(1);
    Fs = AUDFS_768KHz ;
    HDMITX_WriteI2C_Byte(REG_TX_AUDCHST_CA_FS,0x00|Fs);
    Fs = ~Fs ; // OFS is the one's complement of FS
    uData = (0x0f&HDMITX_ReadI2C_Byte(REG_TX_AUDCHST_OFS_WL));
    HDMITX_WriteI2C_Byte(REG_TX_AUDCHST_OFS_WL,(Fs<<4)|uData);
    Switch_HDMITX_Bank(0);
  }
}

void setHDMITX_ChStat(UINT8 ucIEC60958ChStat[])
{
  UINT8 uc ;

  Switch_HDMITX_Bank(1);
  uc = (ucIEC60958ChStat[0] <<1)& 0x7C ;
  HDMITX_WriteI2C_Byte(REG_TX_AUDCHST_MODE,uc);
  HDMITX_WriteI2C_Byte(REG_TX_AUDCHST_CAT,ucIEC60958ChStat[1]); // 192, audio CATEGORY
  HDMITX_WriteI2C_Byte(REG_TX_AUDCHST_SRCNUM,ucIEC60958ChStat[2]&0xF);
  HDMITX_WriteI2C_Byte(REG_TX_AUD0CHST_CHTNUM,(ucIEC60958ChStat[2]>>4)&0xF);
  HDMITX_WriteI2C_Byte(REG_TX_AUDCHST_CA_FS,ucIEC60958ChStat[3]); // choose clock
  HDMITX_WriteI2C_Byte(REG_TX_AUDCHST_OFS_WL,ucIEC60958ChStat[4]);
  Switch_HDMITX_Bank(0);
}

void setHDMITX_HBRAudio(UINT8 bSPDIF)
{
  // BYTE rst;
  Switch_HDMITX_Bank(0);

  // rst = HDMITX_ReadI2C_Byte(REG_TX_SW_RST);
  // rst &= ~(B_HDMITX_AUD_RST|B_TX_AREF_RST);

  // HDMITX_WriteI2C_Byte(REG_TX_SW_RST, rst | B_HDMITX_AUD_RST );

  HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL1,0x47); // regE1 bOutputAudioMode should be loaded from ROM image.
#ifdef USE_IT66120
  if( bSPDIF )
  {
    HDMITX_WriteI2C_Byte(REG_TX_AUDIO_FIFOMAP,0xE4); // default mapping.
  }
  else
  {
    HDMITX_WriteI2C_Byte(REG_TX_AUDIO_FIFOMAP,0xFF); // default mapping.
  }
#else
  HDMITX_WriteI2C_Byte(REG_TX_AUDIO_FIFOMAP,0xE4); // default mapping.
#endif

  if( bSPDIF )
  {
    HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL0, M_TX_AUD_24BIT|B_TX_AUD_SPDIF);
    HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL3,B_TX_CHSTSEL);
  }
  else
  {
    HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL0, M_TX_AUD_24BIT);
    HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL3,0);
  }
  HDMITX_WriteI2C_Byte(REG_TX_AUD_SRCVALID_FLAT,0x08);
  HDMITX_WriteI2C_Byte(REG_TX_AUD_HDAUDIO,B_TX_HBR); // regE5 = 0 ;

  //uc = HDMITX_ReadI2C_Byte(REG_TX_CLK_CTRL1);
  //uc &= ~M_TX_AUD_DIV ;
  //HDMITX_WriteI2C_Byte(REG_TX_CLK_CTRL1, uc);

  if( bSPDIF )
  {
    UINT8 i ;
    for( i = 0 ; i < 100 ; i++ )
    {
      if(HDMITX_ReadI2C_Byte(REG_TX_CLK_STATUS2) & B_TX_OSF_LOCK)
      {
        break ; // stable clock.
      }
    }
    HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL0, M_TX_AUD_24BIT|B_TX_AUD_SPDIF|B_TX_AUD_EN_SPDIF);
  }
  else
  {
    HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL0, M_TX_AUD_24BIT|B_TX_AUD_EN_I2S3|B_TX_AUD_EN_I2S2|B_TX_AUD_EN_I2S1|B_TX_AUD_EN_I2S0);
  }
  HDMITX_AndReg_Byte(0x5c,~(1<<6));
  hdmiTxDev[0].bAudioChannelEnable=HDMITX_ReadI2C_Byte(REG_TX_AUDIO_CTRL0);
  // HDMITX_WriteI2C_Byte(REG_TX_SW_RST, rst  );
}

void setHDMITX_DSDAudio()
{
  // to be continue
  // BYTE rst;
  // rst = HDMITX_ReadI2C_Byte(REG_TX_SW_RST);

  //HDMITX_WriteI2C_Byte(REG_TX_SW_RST, rst | (B_HDMITX_AUD_RST|B_TX_AREF_RST) );

  HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL1,0x41); // regE1 bOutputAudioMode should be loaded from ROM image.
  HDMITX_WriteI2C_Byte(REG_TX_AUDIO_FIFOMAP,0xE4); // default mapping.

  HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL0, M_TX_AUD_24BIT);
  HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL3,0);

  HDMITX_WriteI2C_Byte(REG_TX_AUD_SRCVALID_FLAT,0x00);
  HDMITX_WriteI2C_Byte(REG_TX_AUD_HDAUDIO,B_TX_DSD); // regE5 = 0 ;
  //HDMITX_WriteI2C_Byte(REG_TX_SW_RST, rst & ~(B_HDMITX_AUD_RST|B_TX_AREF_RST) );

  //uc = HDMITX_ReadI2C_Byte(REG_TX_CLK_CTRL1);
  //uc &= ~M_TX_AUD_DIV ;
  //HDMITX_WriteI2C_Byte(REG_TX_CLK_CTRL1, uc);

  HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL0, M_TX_AUD_24BIT|B_TX_AUD_EN_I2S3|B_TX_AUD_EN_I2S2|B_TX_AUD_EN_I2S1|B_TX_AUD_EN_I2S0);
}

void setHDMITX_LPCMAudio(UINT8 AudioSrcNum, UINT8 AudSWL, UINT8 bSPDIF)
{

  UINT8 AudioEnable, AudioFormat ;

  AudioEnable = 0 ;
  AudioFormat = hdmiTxDev[0].bOutputAudioMode ;

  switch(AudSWL)
  {
  case 16:
    AudioEnable |= M_TX_AUD_16BIT ;
    break ;
  case 18:
    AudioEnable |= M_TX_AUD_18BIT ;
    break ;
  case 20:
    AudioEnable |= M_TX_AUD_20BIT ;
    break ;
  case 24:
  default:
    AudioEnable |= M_TX_AUD_24BIT ;
    break ;
  }
  if( bSPDIF )
  {
    AudioFormat &= ~0x40 ;
    AudioEnable |= B_TX_AUD_SPDIF|B_TX_AUD_EN_I2S0 ;
  }
  else
  {
    AudioFormat |= 0x40 ;
    switch(AudioSrcNum)
    {
    case 4:
      AudioEnable |= B_TX_AUD_EN_I2S3|B_TX_AUD_EN_I2S2|B_TX_AUD_EN_I2S1|B_TX_AUD_EN_I2S0 ;
      break ;

    case 3:
      AudioEnable |= B_TX_AUD_EN_I2S2|B_TX_AUD_EN_I2S1|B_TX_AUD_EN_I2S0 ;
      break ;

    case 2:
      AudioEnable |= B_TX_AUD_EN_I2S1|B_TX_AUD_EN_I2S0 ;
      break ;

    case 1:
    default:
      AudioFormat &= ~0x40 ;
      AudioEnable |= B_TX_AUD_EN_I2S0 ;
      break ;

    }
  }
  AudioFormat|=0x01;//mingchih add
  hdmiTxDev[0].bAudioChannelEnable=AudioEnable;

  Switch_HDMITX_Bank(0);
  HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL0,AudioEnable&0xF0);

  HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL1,AudioFormat); // regE1 bOutputAudioMode should be loaded from ROM image.
#ifdef USE_IT66120
  HDMITX_SetI2C_Byte(0x5A,0x02, 0x00);
  if( bSPDIF )
  {
    HDMITX_WriteI2C_Byte(REG_TX_AUDIO_FIFOMAP,0xE4); // default mapping.
  }
  else
  {
    HDMITX_WriteI2C_Byte(REG_TX_AUDIO_FIFOMAP,0xFF); // default mapping.
  }
#else
  HDMITX_WriteI2C_Byte(REG_TX_AUDIO_FIFOMAP,0xE4); // default mapping.
#endif

#ifdef USE_SPDIF_CHSTAT
  if( bSPDIF )
  {
    HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL3,B_TX_CHSTSEL);
  }
  else
  {
    HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL3,0);
  }
#else // not USE_SPDIF_CHSTAT
  HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL3,0);
#endif // USE_SPDIF_CHSTAT

  HDMITX_WriteI2C_Byte(REG_TX_AUD_SRCVALID_FLAT,0x00);
  HDMITX_WriteI2C_Byte(REG_TX_AUD_HDAUDIO,0x00); // regE5 = 0 ;

  if( bSPDIF )
  {
    UINT8 i ;
    HDMITX_OrReg_Byte(0x5c,(1<<6));
    for( i = 0 ; i < 100 ; i++ )
    {
      if(HDMITX_ReadI2C_Byte(REG_TX_CLK_STATUS2) & B_TX_OSF_LOCK)
      {
        break ; // stable clock.
      }
    }
  }
}

void setHDMITX_NLPCMAudio(UINT8 bSPDIF) // no Source Num, no I2S.
{
  UINT8 AudioEnable;
  //UINT8 AudioFormat ;
  UINT8 i ;

  //AudioFormat = 0x01 ; // NLPCM must use standard I2S mode.
  if( bSPDIF )
  {
    AudioEnable = M_TX_AUD_24BIT|B_TX_AUD_SPDIF;
  }
  else
  {
    AudioEnable = M_TX_AUD_24BIT;
  }

  Switch_HDMITX_Bank(0);
  // HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL0, M_TX_AUD_24BIT|B_TX_AUD_SPDIF);
  HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL0, AudioEnable);
  //HDMITX_AndREG_Byte(REG_TX_SW_RST,~(B_HDMITX_AUD_RST|B_TX_AREF_RST));

  HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL1,0x01); // regE1 bOutputAudioMode should be loaded from ROM image.
#ifdef USE_IT66120
  if( bSPDIF )
  {
    HDMITX_WriteI2C_Byte(REG_TX_AUDIO_FIFOMAP,0xE4); // default mapping.
  }
  else
  {
    HDMITX_WriteI2C_Byte(REG_TX_AUDIO_FIFOMAP,0xFF); // default mapping.
  }
#else
  HDMITX_WriteI2C_Byte(REG_TX_AUDIO_FIFOMAP,0xE4); // default mapping.
#endif

#ifdef USE_SPDIF_CHSTAT
  HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL3,B_TX_CHSTSEL);
#else // not USE_SPDIF_CHSTAT
  HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL3,0);
#endif // USE_SPDIF_CHSTAT

  HDMITX_WriteI2C_Byte(REG_TX_AUD_SRCVALID_FLAT,0x00);
  HDMITX_WriteI2C_Byte(REG_TX_AUD_HDAUDIO,0x00); // regE5 = 0 ;

  if( bSPDIF )
  {
    for( i = 0 ; i < 100 ; i++ )
    {
      if(HDMITX_ReadI2C_Byte(REG_TX_CLK_STATUS2) & B_TX_OSF_LOCK)
      {
        break ; // stable clock.
      }
    }
  }
  HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL0, AudioEnable|B_TX_AUD_EN_I2S0);
}

void HDMITX_EnableAudioOutput(UINT8 AudioType, UINT8 bSPDIF,  UINT32 SampleFreq,  UINT8 ChNum, UINT8 *pIEC60958ChStat, UINT32 TMDSClock)
{
  UINT8 ucIEC60958ChStat[5] ;

  UINT8 Fs ;
  AudioDelayCnt=36;
  LastRefaudfreqnum=0;
  hdmiTxDev[0].TMDSClock=TMDSClock;
  hdmiTxDev[0].bAudioChannelEnable=0;
  hdmiTxDev[0].bSPDIF_OUT=bSPDIF;

  DeBug(EFI_D_INFO, "HDMITX_EnableAudioOutput(%02X, %s, %ld, %d, %p, %ld);\n",
      AudioType, bSPDIF?"SPDIF":"I2S",SampleFreq, ChNum, pIEC60958ChStat, TMDSClock
      );

  HDMITX_OrReg_Byte(REG_TX_SW_RST,(B_HDMITX_AUD_RST | B_TX_AREF_RST));
  HDMITX_WriteI2C_Byte(REG_TX_CLK_CTRL0,B_TX_AUTO_OVER_SAMPLING_CLOCK|B_TX_EXT_256FS|0x01);

  HDMITX_SetI2C_Byte(0x0F, 0x10, 0x00 ); // power on the ACLK

  if(bSPDIF)
  {
    if(AudioType==T_AUDIO_HBR)
    {
      HDMITX_WriteI2C_Byte(REG_TX_CLK_CTRL0,0x81);
    }
    HDMITX_OrReg_Byte(REG_TX_AUDIO_CTRL0,B_TX_AUD_SPDIF);
  }
  else
  {
    HDMITX_AndReg_Byte(REG_TX_AUDIO_CTRL0,(~B_TX_AUD_SPDIF));
  }
  if( AudioType != T_AUDIO_DSD)
  {
    // one bit audio have no channel status.
    switch(SampleFreq)
    {
    case  44100L: Fs =  AUDFS_44p1KHz ; break ;
    case  88200L: Fs =  AUDFS_88p2KHz ; break ;
    case 176400L: Fs = AUDFS_176p4KHz ; break ;
    case  32000L: Fs =    AUDFS_32KHz ; break ;
    case  48000L: Fs =    AUDFS_48KHz ; break ;
    case  96000L: Fs =    AUDFS_96KHz ; break ;
    case 192000L: Fs =   AUDFS_192KHz ; break ;
    case 768000L: Fs =   AUDFS_768KHz ; break ;
    default:
                  SampleFreq = 48000L ;
                  Fs =    AUDFS_48KHz ;
                  break ; // default, set Fs = 48KHz.
    }
#ifdef SUPPORT_AUDIO_MONITOR
    hdmiTxDev[0].bAudFs=Fs;// AUDFS_OTHER;
#else
    hdmiTxDev[0].bAudFs=Fs;
#endif
    setHDMITX_NCTS(hdmiTxDev[0].bAudFs);
    if( pIEC60958ChStat == NULL )
    {
      ucIEC60958ChStat[0] = 0 ;
      ucIEC60958ChStat[1] = 0 ;
      ucIEC60958ChStat[2] = (ChNum+1)/2 ;

      if(ucIEC60958ChStat[2]<1)
      {
        ucIEC60958ChStat[2] = 1 ;
      }
      else if( ucIEC60958ChStat[2] >4 )
      {
        ucIEC60958ChStat[2] = 4 ;
      }
      ucIEC60958ChStat[3] = Fs ;
      ucIEC60958ChStat[4] = (((~Fs)<<4) & 0xF0) | CHTSTS_SWCODE ; // Fs | 24bit word length
      pIEC60958ChStat = ucIEC60958ChStat ;
    }
  }
  HDMITX_SetI2C_Byte(REG_TX_SW_RST,(B_HDMITX_AUD_RST|B_TX_AREF_RST),B_TX_AREF_RST);

  switch(AudioType)
  {
  case T_AUDIO_HBR:
    DeBug(EFI_D_INFO, "T_AUDIO_HBR\n");
    pIEC60958ChStat[0] |= 1<<1 ;
    pIEC60958ChStat[2] = 0;
    pIEC60958ChStat[3] &= 0xF0 ;
    pIEC60958ChStat[3] |= AUDFS_768KHz ;
    pIEC60958ChStat[4] |= (((~AUDFS_768KHz)<<4) & 0xF0)| 0xB ;
    setHDMITX_ChStat(pIEC60958ChStat);
    setHDMITX_HBRAudio(bSPDIF);

    break ;
  case T_AUDIO_DSD:
    DeBug(EFI_D_INFO, "T_AUDIO_DSD\n");
    setHDMITX_DSDAudio();
    break ;
  case T_AUDIO_NLPCM:
    DeBug(EFI_D_INFO, "T_AUDIO_NLPCM\n");
    pIEC60958ChStat[0] |= 1<<1 ;
    setHDMITX_ChStat(pIEC60958ChStat);
    setHDMITX_NLPCMAudio(bSPDIF);
    break ;
  case T_AUDIO_LPCM:
    DeBug(EFI_D_INFO, "T_AUDIO_LPCM\n");
    pIEC60958ChStat[0] &= ~(1<<1);

    setHDMITX_ChStat(pIEC60958ChStat);
    setHDMITX_LPCMAudio((ChNum+1)/2, SUPPORT_AUDI_AudSWL, bSPDIF);
    // can add auto adjust
    break ;
  }
  HDMITX_AndReg_Byte(REG_TX_INT_MASK1,(~B_TX_AUDIO_OVFLW_MASK));
  HDMITX_WriteI2C_Byte(REG_TX_AUDIO_CTRL0, hdmiTxDev[0].bAudioChannelEnable);

  HDMITX_SetI2C_Byte(REG_TX_SW_RST,(B_HDMITX_AUD_RST|B_TX_AREF_RST),0);
}

void HDMITX_SetAudioOutput()
{
  if( bAudioEnable )
  {
    ConfigAudioInfoFrm();
    // HDMITX_EnableAudioOutput(T_AUDIO_LPCM, FALSE, ulAudioSampleFS,OUTPUT_CHANNEL,NULL,TMDSClock);
    HDMITX_EnableAudioOutput(
        //CNOFIG_INPUT_AUDIO_TYPE,
        bOutputAudioType,
        CONFIG_INPUT_AUDIO_SPDIF,
        ulAudioSampleFS,
        bOutputAudioChannel,
        NULL, // pointer to cahnnel status.
        VideoPixelClock*(pixelrep+1));
    // if you have channel status , set here.
    // setHDMITX_ChStat(BYTE ucIEC60958ChStat[]);
    bChangeAudio = FALSE ;
  }
}

void HDMITX_SetOutput()
{
  VIDEOPCLKLEVEL level ;
  unsigned long TMDSClock = VideoPixelClock*(pixelrep+1);
  HDMITX_DisableAudioOutput();
  //    HDMITX_EnableHDCP(FALSE);

  if( TMDSClock>80000000L )
  {
    level = PCLK_HIGH ;
  }
  else if(TMDSClock>20000000L)
  {
    level = PCLK_MEDIUM ;
  }
  else
  {
    level = PCLK_LOW ;
  }
#ifdef IT6615
  HDMITX_DEBUG_PRINTF(("OutputColorDepth = %02X\n",(int)OutputColorDepth)) ;
  setHDMITX_ColorDepthPhase(OutputColorDepth,0);
#endif

  setHDMITX_VideoSignalType(InstanceData.bInputVideoSignalType);
#ifdef SUPPORT_SYNCEMBEDDED
  if(InstanceData.bInputVideoSignalType & T_MODE_SYNCEMB)
  {
    setHDMITX_SyncEmbeddedByVIC(VIC,InstanceData.bInputVideoSignalType);
  }
#endif

  DeBug(EFI_D_INFO, "level = %d, ,bInputColorMode=%x,bOutputColorMode=%x,bHDMIMode=%x\n",(int)level,(int)bInputColorMode,(int)bOutputColorMode ,(int)bHDMIMode);
  HDMITX_EnableVideoOutput(level,bInputColorMode,bOutputColorMode ,bHDMIMode);

  if( bHDMIMode )
  {
#ifdef OUTPUT_3D_MODE
    ConfigfHdmiVendorSpecificInfoFrame(OUTPUT_3D_MODE);
#endif
    //HDMITX_EnableHDCP(TRUE);
    ConfigAVIInfoFrame(VIC, pixelrep);

    HDMITX_SetAudioOutput();

    // if( bAudioEnable )
    // {
    //     ConfigAudioInfoFrm();
    // #ifdef SUPPORT_HBR_AUDIO
    //     HDMITX_EnableAudioOutput(T_AUDIO_HBR, CONFIG_INPUT_AUDIO_SPDIF, 768000L,8,NULL,TMDSClock);
    // #else
    //     // HDMITX_EnableAudioOutput(T_AUDIO_LPCM, FALSE, ulAudioSampleFS,OUTPUT_CHANNEL,NULL,TMDSClock);
    //     HDMITX_EnableAudioOutput(CNOFIG_INPUT_AUDIO_TYPE, CONFIG_INPUT_AUDIO_SPDIF, ulAudioSampleFS,bOutputAudioChannel,NULL,TMDSClock);
    // #endif
    // }

  }
  else
  {
    HDMITX_EnableAVIInfoFrame(FALSE ,NULL);
    HDMITX_EnableVSInfoFrame(FALSE,NULL);
  }
#ifdef SUPPORT_CEC
  Switch_HDMITX_Bank(0);
  HDMITX_WriteI2C_Byte( 0xf, 0 );

  Initial_Ext_Int1();
  HDMITX_CEC_Init();
#endif // SUPPORT_CEC
  setHDMITX_AVMute(FALSE);
  bChangeMode = FALSE ;
  //    DumpHDMITXReg() ;
}

void HDMITX_InitTxDev(HDMITXDEV *pInstance)
{
  if(pInstance && 0 < HDMITX_MAX_DEV_COUNT)
  {
    hdmiTxDev[0] = *pInstance ;
  }
}

void InitHDMITX_Variable()
{
  HDMITX_InitTxDev(&InstanceData);
  HPDStatus = FALSE;
  HPDChangeStatus = FALSE;
}

void InitHDMITX()
{

  hdmitx_LoadRegSetting(HDMITX_Init_Table);
  //HDMITX_WriteI2C_Byte(REG_TX_INT_CTRL,hdmiTxDev[0].bIntType);
  hdmiTxDev[0].bIntPOL = (hdmiTxDev[0].bIntType&B_TX_INTPOL_ACTH)?TRUE:FALSE ;

  // Avoid power loading in un play status.
  //////////////////////////////////////////////////////////////////
  // Setup HDCP ROM
  //////////////////////////////////////////////////////////////////
#ifdef HDMITX_INPUT_INFO
  hdmiTxDev[0].RCLK = CalcRCLK();
#endif
  hdmitx_LoadRegSetting(HDMITX_DefaultVideo_Table);
  hdmitx_LoadRegSetting(HDMITX_SetHDMI_Table);
  hdmitx_LoadRegSetting(HDMITX_DefaultAVIInfo_Table);
  hdmitx_LoadRegSetting(HDMITX_DeaultAudioInfo_Table);
  hdmitx_LoadRegSetting(HDMITX_Aud_CHStatus_LPCM_20bit_48Khz);
  hdmitx_LoadRegSetting(HDMITX_AUD_SPDIF_2ch_24bit);
  //    HDMITX_PowerOn();

#ifdef SUPPORT_CEC
  Switch_HDMITX_Bank(0);
  HDMITX_WriteI2C_Byte( 0xf, 0 );

  Initial_Ext_Int1();
  HDMITX_CEC_Init();
#endif // SUPPORT_CEC
  DeBug(EFI_D_INFO,
      "-----------------------------------------------------\n"
      "  Init HDMITX\n"
      "-----------------------------------------------------\n");

  //    DumpHDMITXReg();
}

UINT8 getHDMITX_LinkStatus()
{
  if(B_TX_RXSENDETECT & HDMITX_ReadI2C_Byte(REG_TX_SYS_STATUS))
  {
    if(0==HDMITX_ReadI2C_Byte(REG_TX_AFE_DRV_CTRL))
    {
      //HDMITX_DEBUG_PRINTF(("getHDMITX_LinkStatus()!!\n") );
      return TRUE;
    }
  }
  //HDMITX_DEBUG_PRINTF(("GetTMDS not Ready()!!\n") );

  return FALSE;
}

void hdmitx_ClearDDCFIFO()
{
  HDMITX_WriteI2C_Byte(REG_TX_DDC_MASTER_CTRL,B_TX_MASTERDDC|B_TX_MASTERHOST);
  HDMITX_WriteI2C_Byte(REG_TX_DDC_CMD,CMD_FIFO_CLR);
}

void hdmitx_AbortDDC()
{
  //UINT8 CPDesire;
  UINT8 SWReset;
  //UINT8 DDCMaster ;
  UINT8 uc, timeout, i ;
  // save the SW reset,DDC master,and CP Desire setting.
  SWReset = HDMITX_ReadI2C_Byte(REG_TX_SW_RST);
  //CPDesire = HDMITX_ReadI2C_Byte(REG_TX_HDCP_DESIRE);
  //DDCMaster = HDMITX_ReadI2C_Byte(REG_TX_DDC_MASTER_CTRL);

  // HDMITX_WriteI2C_Byte(REG_TX_HDCP_DESIRE,CPDesire&(~B_TX_CPDESIRE)); // @emily change order
  HDMITX_WriteI2C_Byte(REG_TX_SW_RST,SWReset|B_TX_HDCP_RST_HDMITX);         // @emily change order
  HDMITX_WriteI2C_Byte(REG_TX_DDC_MASTER_CTRL,B_TX_MASTERDDC|B_TX_MASTERHOST);

  // 2009/01/15 modified by Jau-Chih.Tseng@ite.com.tw
  // do abort DDC twice.
  for( i = 0 ; i < 2 ; i++ )
  {
    HDMITX_WriteI2C_Byte(REG_TX_DDC_CMD,CMD_DDC_ABORT);

    for( timeout = 0 ; timeout < 200 ; timeout++ )
    {
      uc = HDMITX_ReadI2C_Byte(REG_TX_DDC_STATUS);
      if (uc&B_TX_DDC_DONE)
      {
        break ; // success
      }
      if( uc & (B_TX_DDC_NOACK|B_TX_DDC_WAITBUS|B_TX_DDC_ARBILOSE) )
      {
        //                HDMITX_DEBUG_PRINTF(("hdmitx_AbortDDC Fail by reg16=%02X\n",(int)uc));
        break ;
      }
      DELAY_IT6612(1000); // delay 1 ms to stable.
    }
  }
  //~Jau-Chih.Tseng@ite.com.tw

}

UINT8 CheckHDMITX(UINT8 *pHPD,UINT8 *pHPDChange)
{
  UINT8 intdata1;
  UINT8 intdata2;
  UINT8 intdata3;
  UINT8 sysstat;
  UINT8  intclr3 = 0 ;
  UINT8 PrevHPD = hdmiTxDev[0].bHPD ;
  UINT8 HPD ;

  intdata2 = HDMITX_ReadI2C_Byte(REG_TX_SYS_STATUS);
  sysstat = intdata2;
  // HDMITX_DEBUG_PRINTF(("REG_TX_SYS_STATUS = %X \n",sysstat));

  if((sysstat & (B_TX_HPDETECT/*|B_TX_RXSENDETECT*/)) == (B_TX_HPDETECT/*|B_TX_RXSENDETECT*/))
  {
    HPD = TRUE;
  }
  else
  {
    HPD = FALSE;
  }
  // CheckClockStable(sysstat);
  // 2007/06/20 added by jj_tseng@chipadvanced.com

  if(pHPDChange)
  {
    *pHPDChange = (HPD!=PrevHPD)?TRUE:FALSE ; // default give pHPDChange value compared to previous HPD value.

  }
  //~jj_tseng@chipadvanced.com 2007/06/20

  if(HPD==FALSE)
  {
    hdmiTxDev[0].bAuthenticated = FALSE ;
  }
  if(sysstat & B_TX_INT_ACTIVE)
  {
    DeBug(EFI_D_INFO, "REG_TX_SYS_STATUS = 0x%02X \n",(int)sysstat);

    intdata1 = HDMITX_ReadI2C_Byte(REG_TX_INT_STAT1);
    DeBug(EFI_D_INFO, "INT_Handler: reg%X = %X\n",(int)REG_TX_INT_STAT1,(int)intdata1);
    if(intdata1 & B_TX_INT_AUD_OVERFLOW)
    {
      DeBug(EFI_D_INFO, "B_TX_INT_AUD_OVERFLOW.\n");
      HDMITX_OrReg_Byte(REG_TX_SW_RST,(B_HDMITX_AUD_RST|B_TX_AREF_RST));
      HDMITX_AndReg_Byte(REG_TX_SW_RST,~(B_HDMITX_AUD_RST|B_TX_AREF_RST));
      //AudioDelayCnt=AudioOutDelayCnt;
      //LastRefaudfreqnum=0;
    }
    if(intdata1 & B_TX_INT_DDCFIFO_ERR)
    {
      DeBug(EFI_D_INFO, "DDC FIFO Error.\n");
      hdmitx_ClearDDCFIFO();
      hdmiTxDev[0].bAuthenticated= FALSE ;
    }
    if(intdata1 & B_TX_INT_DDC_BUS_HANG)
    {
      DeBug(EFI_D_INFO, "DDC BUS HANG.\n");
      hdmitx_AbortDDC();

      if(hdmiTxDev[0].bAuthenticated)
      {
        DeBug(EFI_D_INFO, "when DDC hang,and aborted DDC,the HDCP authentication need to restart.\n");
#ifndef _SUPPORT_HDCP_REPEATER_
#ifdef SUPPORT_HDCP
        hdmitx_hdcp_ResumeAuthentication();
#endif
#else
        TxHDCP_chg(TxHDCP_AuthFail);
#endif
      }
    }
    if(intdata1 & (B_TX_INT_HPD_PLUG/*|B_TX_INT_RX_SENSE*/))
    {

      if(pHPDChange)
      {
        *pHPDChange = TRUE ;
      }
      if(HPD == FALSE)
      {
        /*
           HDMITX_WriteI2C_Byte(REG_TX_SW_RST,B_TX_AREF_RST|B_HDMITX_VID_RST|B_HDMITX_AUD_RST|B_TX_HDCP_RST_HDMITX);
           delay1ms(1);
           HDMITX_WriteI2C_Byte(REG_TX_AFE_DRV_CTRL,B_TX_AFE_DRV_RST|B_TX_AFE_DRV_PWD);
           */
        //HDMITX_DEBUG_PRINTF(("Unplug,%x %x\n",(int)HDMITX_ReadI2C_Byte(REG_TX_SW_RST),(int)HDMITX_ReadI2C_Byte(REG_TX_AFE_DRV_CTRL)));
      }
    }
    if(intdata1 & (B_TX_INT_RX_SENSE))
    {
      hdmiTxDev[0].bAuthenticated = FALSE;
    }
    intdata2 = HDMITX_ReadI2C_Byte(REG_TX_INT_STAT2);
    DeBug(EFI_D_INFO, "INT_Handler: reg%X = %X\n",(int)REG_TX_INT_STAT2,(int)intdata2);

#ifdef SUPPORT_HDCP
    if(intdata2 & B_TX_INT_AUTH_DONE)
    {
      HDMITX_DEBUG_PRINTF(("interrupt Authenticate Done.\n"));
      HDMITX_OrReg_Byte(REG_TX_INT_MASK2,(UINT8)B_TX_AUTH_DONE_MASK);
      //hdmiTxDev[0].bAuthenticated = TRUE ;
      //setHDMITX_AVMute(FALSE);
    }
    if(intdata2 & B_TX_INT_AUTH_FAIL)
    {
      hdmiTxDev[0].bAuthenticated = FALSE;
      //HDMITX_DEBUG_PRINTF(("interrupt Authenticate Fail.\n"));
      hdmitx_AbortDDC();   // @emily add
#ifdef _SUPPORT_HDCP_REPEATER_
      TxHDCP_chg(TxHDCP_AuthFail) ;
#endif
      //hdmitx_hdcp_ResumeAuthentication();
    }
#ifdef _SUPPORT_HDCP_REPEATER_
    if(intdata2 & B_TX_INT_KSVLIST_CHK )
    {
      TxHDCP_chg(TxHDCP_CheckFIFORDY) ;// check FIFO ready
    }
#endif // _SUPPORT_HDCP_REPEATER_
#endif // SUPPORT_HDCP

    intdata3= HDMITX_ReadI2C_Byte(0xEE);
    if( intdata3 )
    {
      HDMITX_WriteI2C_Byte(0xEE,intdata3); // clear ext interrupt ;
      DeBug(EFI_D_INFO, "%s%s%s%s%s%s%s\n",
          (intdata3&0x40)?"video parameter change ":"",
          (intdata3&0x20)?"HDCP Pj check done ":"",
          (intdata3&0x10)?"HDCP Ri check done ":"",
          (intdata3&0x8)? "DDC bus hang ":"",
          (intdata3&0x4)? "Video input FIFO auto reset ":"",
          (intdata3&0x2)? "No audio input interrupt  ":"",
          (intdata3&0x1)? "Audio decode error interrupt ":"");
    }
    HDMITX_WriteI2C_Byte(REG_TX_INT_CLR0,0xFF);
    HDMITX_WriteI2C_Byte(REG_TX_INT_CLR1,0xFF);
    intclr3 = (HDMITX_ReadI2C_Byte(REG_TX_SYS_STATUS))|B_TX_CLR_AUD_CTS | B_TX_INTACTDONE ;
    HDMITX_WriteI2C_Byte(REG_TX_SYS_STATUS,intclr3); // clear interrupt.
    intclr3 &= ~(B_TX_INTACTDONE);
    HDMITX_WriteI2C_Byte(REG_TX_SYS_STATUS,intclr3); // INTACTDONE reset to zero.
  }
  if(pHPDChange)
  {
    if((*pHPDChange==TRUE) &&(HPD==FALSE))
    {
      HDMITX_WriteI2C_Byte(REG_TX_AFE_DRV_CTRL,B_TX_AFE_DRV_RST|B_TX_AFE_DRV_PWD);
    }
  }
  if(pHPD)
  {
    *pHPD = HPD    ;
  }
  hdmiTxDev[0].bHPD = HPD ;
  return HPD ;
}

void HDMITX_DisableVideoOutput()
{
  UINT8 uc = HDMITX_ReadI2C_Byte(REG_TX_SW_RST) | B_HDMITX_VID_RST ;
  HDMITX_WriteI2C_Byte(REG_TX_SW_RST,uc);
  HDMITX_WriteI2C_Byte(REG_TX_AFE_DRV_CTRL,B_TX_AFE_DRV_RST|B_TX_AFE_DRV_PWD);
  HDMITX_SetI2C_Byte(0x62, 0x90, 0x00);
  HDMITX_SetI2C_Byte(0x64, 0x89, 0x00);
}

void HDMITX_PowerDown()
{
  hdmitx_LoadRegSetting(HDMITX_PwrDown_Table);
}

VOID
EFIAPI
HDMITX_DevLoopProc (
    //  IN  EFI_EVENT  Event,
    //  IN  VOID       *Context
    )
{
  CheckHDMITX(&HPDStatus,&HPDChangeStatus);

  //Yuan
  HPDChangeStatus = TRUE;
  HPDStatus = TRUE;

  if(HPDChangeStatus || init_flag)
  {
    if( HPDStatus || init_flag)
    {
      HDMITX_PowerOn();
      //      ParseEDID();
      bOutputColorMode = F_MODE_RGB444;

      bHDMIMode = TRUE;
      bChangeAudio = TRUE;
      bChangeMode=TRUE;

      bAudioEnable = TRUE ;
      //Yuan
      //<<
      if( bChangeMode )
      {
        DeBug(EFI_D_INFO, "Mode change HDMITX_SetOutput();\n");
        HDMITX_SetOutput();
        bChangeMode = FALSE ;
      }
      if(getHDMITX_LinkStatus())
      {
        // AudioModeDetect();
        setHDMITX_AVMute(FALSE);
      }

      if(bChangeAudio)
      {
        HDMITX_SetAudioOutput() ;
      }
      //>>
    }
    else
    {
      // unplug mode, ...
      DeBug(EFI_D_INFO, "HPD OFF HDMITX_DisableVideoOutput()\n");

#ifdef _SUPPORT_HDCP_REPEATER_
      TxHDCP_chg(TxHDCP_Off);
#endif

      HDMITX_DisableVideoOutput();
      HDMITX_PowerDown();

    }
  }
  else
  {
    if( bChangeMode )
    {
      DeBug(EFI_D_INFO, "Mode change HDMITX_SetOutput();\n");
      HDMITX_SetOutput();
      bChangeMode = FALSE ;
    }
    if(getHDMITX_LinkStatus())
    {
      // AudioModeDetect();
      setHDMITX_AVMute(FALSE);
    }

    if(bChangeAudio)
    {
      HDMITX_SetAudioOutput() ;
    }
  }
  init_flag = 0;
}

EFI_STATUS
EFIAPI
DcPhyIt66121Config(
  IN UINT8  DvoChannel,
  IN UINT8  DevAddr,
  IN UINT32 FreqKhz,
  IN UINT8  Mode
  )
{
  UINT32 VID1, VID2, VID3;

  Ls7aI2cGpioInit(Mode, FreqKhz);
  I2cAddr = DevAddr;
  I2cCh = DvoChannel;

  VID1 = HDMITX_ReadI2C_Byte(00);
  DeBug(EFI_D_INFO, "VID = 0x%x\n ",VID1);
  VID2 = HDMITX_ReadI2C_Byte(01);
  DeBug(EFI_D_INFO, "VID = 0x%x\n ",VID2);
  VID3 = HDMITX_ReadI2C_Byte(02);
  DeBug(EFI_D_INFO, "VID = 0x%x\n ",VID3);

  if((VID1!=0x54)||(VID2!=0x49)||(VID3!=0x12)){
    return EFI_DEVICE_ERROR;
  }

  if( HDMITX_SetI2C_Byte(0x05,0x80,0x80) != EFI_SUCCESS ){
    return EFI_DEVICE_ERROR;
  }

  InitHDMITX_Variable();
  InitHDMITX();
  HDMITX_ChangeDisplayOption(HDMI_720p60,HDMI_RGB444) ;
  HDMITX_DevLoopProc();
  VID1 = HDMITX_ReadI2C_Byte(0x5);
  DeBug(EFI_D_INFO, "reg05 = 0x%x\n ",VID1);
  if(Mode == GPIO_I2C_MODE_PEI){
    Ls7aI2cGpioExit();
  }

  return EFI_SUCCESS;
}

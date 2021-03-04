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

#ifndef _HDMITX_SYS_H_
#define _HDMITX_SYS_H_

#define I2S 0
#define SPDIF 1

#ifndef I2S_FORMAT
#define I2S_FORMAT 0x01 // 32bit audio
#endif

#ifndef INPUT_SAMPLE_FREQ_HZ
    #define INPUT_SAMPLE_FREQ_HZ 48000L
#endif //INPUT_SAMPLE_FREQ_HZ

#ifndef OUTPUT_CHANNEL
    #define OUTPUT_CHANNEL 2
#endif //OUTPUT_CHANNEL

#ifndef CNOFIG_INPUT_AUDIO_TYPE
//    #define CNOFIG_INPUT_AUDIO_TYPE T_AUDIO_LPCM
    // #define CNOFIG_INPUT_AUDIO_TYPE T_AUDIO_NLPCM
    // #define CNOFIG_INPUT_AUDIO_TYPE T_AUDIO_HBR
#endif //CNOFIG_INPUT_AUDIO_TYPE

#ifndef CONFIG_INPUT_AUDIO_SPDIF
//    #define CONFIG_INPUT_AUDIO_SPDIF I2S
     #define CONFIG_INPUT_AUDIO_SPDIF  SPDIF
#endif //CONFIG_INPUT_AUDIO_SPDIF

#ifndef INPUT_SIGNAL_TYPE
#define INPUT_SIGNAL_TYPE 0 // 24 bit sync seperate
#endif

////////////////////////////////////////////////////////////////////////////////
// Internal Data Type
////////////////////////////////////////////////////////////////////////////////

typedef enum tagHDMI_Video_Type {
    HDMI_Unkown = 0 ,
    HDMI_640x480p60 = 1 ,
    HDMI_480p60,
    HDMI_480p60_16x9,
    HDMI_720p60,
    HDMI_1080i60,
    HDMI_480i60,
    HDMI_480i60_16x9,
    HDMI_1080p60 = 16,
    HDMI_576p50,
    HDMI_576p50_16x9,
    HDMI_720p50,
    HDMI_1080i50,
    HDMI_576i50,
    HDMI_576i50_16x9,
    HDMI_1080p50 = 31,
    HDMI_1080p24,
    HDMI_1080p25,
    HDMI_1080p30,
    HDMI_720p30 = 61,
    HDMI_0x55 = 0x55,
} HDMI_Video_Type ;

typedef enum tagHDMI_Aspec {
    HDMI_4x3 ,
    HDMI_16x9
} HDMI_Aspec;

typedef enum tagHDMI_OutputColorMode {
    HDMI_RGB444,
    HDMI_YUV444,
    HDMI_YUV422
} HDMI_OutputColorMode ;

typedef enum tagHDMI_Colorimetry {
    HDMI_ITU601,
    HDMI_ITU709
} HDMI_Colorimetry ;

struct VideoTiming {
    UINT32 VideoPixelClock ;
    UINT8 VIC ;
    UINT8 pixelrep ;
	UINT8 outputVideoMode ;
} ;



typedef enum _TXVideo_State_Type {
    TXVSTATE_Unplug = 0,
    TXVSTATE_HPD,
    TXVSTATE_WaitForMode,
    TXVSTATE_WaitForVStable,
    TXVSTATE_VideoInit,
    TXVSTATE_VideoSetup,
    TXVSTATE_VideoOn,
    TXVSTATE_Reserved
} TXVideo_State_Type ;


typedef enum _TXAudio_State_Type {
    TXASTATE_AudioOff = 0,
    TXASTATE_AudioPrepare,
    TXASTATE_AudioOn,
    TXASTATE_AudioFIFOFail,
    TXASTATE_Reserved
} TXAudio_State_Type ;
/////////////////////////////////////////
// RX Capability.
/////////////////////////////////////////
typedef struct {
    UINT8 b16bit:1 ;
    UINT8 b20bit:1 ;
    UINT8 b24bit:1 ;
    UINT8 Rsrv:5 ;
} LPCM_BitWidth ;

typedef enum {
    AUD_RESERVED_0 = 0 ,
    AUD_LPCM,
    AUD_AC3,
    AUD_MPEG1,
    AUD_MP3,
    AUD_MPEG2,
    AUD_AAC,
    AUD_DTS,
    AUD_ATRAC,
    AUD_ONE_BIT_AUDIO,
    AUD_DOLBY_DIGITAL_PLUS,
    AUD_DTS_HD,
    AUD_MAT_MLP,
    AUD_DST,
    AUD_WMA_PRO,
    AUD_RESERVED_15
} AUDIO_FORMAT_CODE ;

typedef union {
    struct {
        UINT8 channel:3 ;
        UINT8 AudioFormatCode:4 ;
        UINT8 Rsrv1:1 ;

        UINT8 b32KHz:1 ;
        UINT8 b44_1KHz:1 ;
        UINT8 b48KHz:1 ;
        UINT8 b88_2KHz:1 ;
        UINT8 b96KHz:1 ;
        UINT8 b176_4KHz:1 ;
        UINT8 b192KHz:1 ;
        UINT8 Rsrv2:1 ;
        UINT8 ucCode ;
    } s ;
    UINT8 uc[3] ;
} AUDDESCRIPTOR ;

typedef union {
    struct {
        UINT8 FL_FR:1 ;
        UINT8 LFE:1 ;
        UINT8 FC:1 ;
        UINT8 RL_RR:1 ;
        UINT8 RC:1 ;
        UINT8 FLC_FRC:1 ;
        UINT8 RLC_RRC:1 ;
        UINT8 Reserve:1 ;
        UINT8 Unuse[2] ;
    } s ;
    UINT8 uc[3] ;
} SPK_ALLOC ;

#define CEA_SUPPORT_UNDERSCAN (1<<7)
#define CEA_SUPPORT_AUDIO (1<<6)
#define CEA_SUPPORT_YUV444 (1<<5)
#define CEA_SUPPORT_YUV422 (1<<4)
#define CEA_NATIVE_MASK 0xF


#define HDMI_DC_SUPPORT_AI (1<<7)
#define HDMI_DC_SUPPORT_48 (1<<6)
#define HDMI_DC_SUPPORT_36 (1<<5)
#define HDMI_DC_SUPPORT_30 (1<<4)
#define HDMI_DC_SUPPORT_Y444 (1<<3)
#define HDMI_DC_SUPPORT_DVI_DUAL 1

typedef union _tag_DCSUPPORT {
    struct {
        UINT8 DVI_Dual:1 ;
        UINT8 Rsvd:2 ;
        UINT8 DC_Y444:1 ;
        UINT8 DC_30Bit:1 ;
        UINT8 DC_36Bit:1 ;
        UINT8 DC_48Bit:1 ;
        UINT8 SUPPORT_AI:1 ;
    } info ;
    UINT8 uc ;
} DCSUPPORT ;

typedef union _LATENCY_SUPPORT{
    struct {
        UINT8 Rsvd:6 ;
        UINT8 I_Latency_Present:1 ;
        UINT8 Latency_Present:1 ;
    } info ;
    UINT8 uc ;
} LATENCY_SUPPORT ;

#define HDMI_IEEEOUI 0x0c03
#define MAX_VODMODE_COUNT 32
#define MAX_AUDDES_COUNT 4

typedef struct _RX_CAP{
    UINT8 VideoMode ;
    UINT8 NativeVDOMode ;
    UINT8 VDOMode[8] ;
    UINT8 AUDDesCount ;
    AUDDESCRIPTOR AUDDes[MAX_AUDDES_COUNT] ;
    UINT8 PA[2] ;
    UINT32 IEEEOUI ;
    DCSUPPORT dc ;
    UINT8 MaxTMDSClock ;
    LATENCY_SUPPORT lsupport ;
    SPK_ALLOC   SpeakerAllocBlk ;
    UINT8 ValidCEA:1 ;
    UINT8 ValidHDMI:1 ;
    UINT8 Valid3D:1 ;
} RX_CAP ;

///////////////////////////////////////////////////////////////////////
// Output Mode Type
///////////////////////////////////////////////////////////////////////

#define RES_ASPEC_4x3 0
#define RES_ASPEC_16x9 1
#define F_MODE_REPT_NO 0
#define F_MODE_REPT_TWICE 1
#define F_MODE_REPT_QUATRO 3
#define F_MODE_CSC_ITU601 0
#define F_MODE_CSC_ITU709 1
#if 0
void InitHDMITX_Variable();
void HDMITX_ChangeDisplayOption(HDMI_Video_Type VideoMode, HDMI_OutputColorMode OutputColorMode);
void HDMITX_SetOutput();
void HDMITX_DevLoopProc();
void ConfigfHdmiVendorSpecificInfoFrame(UINT8 _3D_Stru);
void HDMITX_ChangeAudioOption(UINT8 Option, UINT8 channelNum, UINT8 AudioFs);
void HDMITX_SetAudioOutput();
void HDMITX_ChangeColorDepth(UINT8 colorDepth);
#endif
#endif // _HDMITX_SYS_H_

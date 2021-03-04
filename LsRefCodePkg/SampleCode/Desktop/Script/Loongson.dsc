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
##
################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################


[Defines]
  PLATFORMPKG_NAME               = LoongsonPlatform
  PLATFORM_GUID                  = 4a5475ee-5b27-4ed9-8147-862797bb7fe3
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  SUPPORTED_ARCHITECTURES        = LOONGARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  PLATFORM_DIRECTORY             = LsRefCodePkg/SampleCode/Desktop/
  FLASH_DEFINITION               = $(PLATFORM_DIRECTORY)/Script/Loongson.fdf
  CONFIG_FILE                    = $(PLATFORM_DIRECTORY)/Script/Desktop.cfg

!include $(CONFIG_FILE)
!include Loongson.fdf.inc

!if ($(CPU_TYPE) == 3A5000 || $(CPU_TYPE) == 3B5000)
  PLATFORM_NAME                  = Ls3a50007a
  CPU_TYPE                       = 3A5000
!endif
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)

  DEFINE COMMON_BUILD_OPTIONS_FLAGS = -DLS$(CPU_TYPE) -DCLK_BASE=$(BASE_CLK) -DHIGH_MEM_BASE_ADDRESS=$(HIGH_MEM_BASE_ADDRESS) -DDDR_FREQ=$(MEM_FREQ) -DCPU_FREQ=$(CPU_FREQ) -DLS$(SOUTH_BRIDGE) -DFLASH_CODE_SECFV_BASE_ADDRESS=$(FLASH_CODE_SECFV_BASE_ADDRESS) -DCACHE_CODE_SECFV_BASE_ADDRESS=$(CACHE_CODE_SECFV_BASE_ADDRESS) -DFLASH_CODE_PEIFV_BASE_ADDRESS=$(FLASH_CODE_PEIFV_BASE_ADDRESS) -DCACHE_CODE_PEIFV_BASE_ADDRESS=$(CACHE_CODE_PEIFV_BASE_ADDRESS) -DSECFV_SIZE=$(SECFV_SIZE) -DPEIFV_SIZE=$(PEIFV_SIZE)  -DCACHEASRAM_RAM_SIZE=$(CACHEASRAM_RAM_SIZE) -DCLK_DDR=$(DDR_CLK) -DCLK_REF=$(REF_CLK) -DTOT_NODE_NUM="1" -DMPS_ADDR=$(MPS_ADDR) -DCACHED_MEMORY_ADDR=$(CACHED_MEMORY_ADDR) -DUNCACHED_MEMORY_ADDR=$(UNCACHED_MEMORY_ADDR)

  !if $(BONITO_100M) == TRUE
    COMMON_BUILD_OPTIONS_FLAGS   = -DBONITO_100M $(COMMON_BUILD_OPTIONS_FLAGS)
  !else
    COMMON_BUILD_OPTIONS_FLAGS   = -DBONITO_25M $(COMMON_BUILD_OPTIONS_FLAGS)
  !endif

  !if $(SPI_DUAL_LINE) == TRUE
    COMMON_BUILD_OPTIONS_FLAGS   = -DSPI_DUAL_LINE $(COMMON_BUILD_OPTIONS_FLAGS)
  !endif

  !if $(COMP_TYPE) == DEBUG
    COMMON_BUILD_OPTIONS_FLAGS   = -DASM_DEBUG $(COMMON_BUILD_OPTIONS_FLAGS)
  !endif

  !if $(LS7A_PHYS48_TO_HT40) == TRUE
    COMMON_BUILD_OPTIONS_FLAGS   = -DLS7A_PHYS48_TO_HT40 $(COMMON_BUILD_OPTIONS_FLAGS)
  !endif

  !if $(CODEC_VERBTABLE_SUPPORT) == TRUE
    COMMON_BUILD_OPTIONS_FLAGS   = -DFIX_VERB_TABLE $(COMMON_BUILD_OPTIONS_FLAGS)
  !endif
############################################################################
#
# Defines for default states.  These can be changed on the command line.
# -D FLAG=VALUE
############################################################################

[BuildOptions]
  GCC:RELEASE_*_*_CC_FLAGS       = -DMDEPKG_NDEBUG -DSPEEDUP

  GCC:*_*_*_CC_FLAGS             = $(COMMON_BUILD_OPTIONS_FLAGS)

  !if $(HDMI_SIL9022A_SUPPORT) == TRUE
    GCC:*_*_*_CC_FLAGS             = -DHDMI_SIL9022A_SUPPORT
  !endif

  !if $(HDMI_IT66121_SUPPORT) == TRUE
    GCC:*_*_*_CC_FLAGS             = -DHDMI_IT66121_SUPPORT
  !endif

  !if $(VGA_CH7034_SUPPORT) == TRUE
    GCC:*_*_*_CC_FLAGS             = -DVGA_CH7034_SUPPORT
  !endif

  *_*_LOONGARCH64_PP_FLAGS       = $(COMMON_BUILD_OPTIONS_FLAGS)

[BuildOptions.LOONGARCH64.EDKII.SEC]
  *_*_LOONGARCH64_ASM_FLAGS      = $(COMMON_BUILD_OPTIONS_FLAGS)

  GCC:*_*_*_CC_FLAGS             = -DHEAP_SIZE=$(HEAP_SIZE)
################################################################################
#
# SKU Identification section - list of all SKU IDs supported by this Platform.
#
################################################################################
[SkuIds]
  0|DEFAULT

################################################################################
#
# Library Class section - list of all Library Classes needed by this Platform.
#
################################################################################
[LibraryClasses]
  PcdLib                           | MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  TimerLib                         | LsRefCodePkg/Core/Library/StableTimerLib/TimerLib.inf
  PrintLib                         | MdePkg/Library/BasePrintLib/BasePrintLib.inf
  BaseMemoryLib                    | MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  BaseLib                          | MdePkg/Library/BaseLib/BaseLib.inf
  UefiLib                          | MdePkg/Library/UefiLib/UefiLib.inf
  SynchronizationLib               | MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
  CpuLib                           | MdePkg/Library/BaseCpuLib/BaseCpuLib.inf
  PerformanceLib                   | MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
  PeCoffLib                        | MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf
  CacheMaintenanceLib              | MdePkg/Library/BaseCacheMaintenanceLib/BaseCacheMaintenanceLib.inf
  UefiDecompressLib                | MdePkg/Library/BaseUefiDecompressLib/BaseUefiDecompressLib.inf
  UefiHiiServicesLib               | MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf
  HiiLib                           | MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  GenericBdsLib                    | LsRefCodePkg/Library/GenericBdsLib/GenericBdsLib.inf
!if $(CAPSULE_ENABLE)
!if $(OPENSSL_COMPILE_ENABLE)
  OpensslLib                       | LsRefCodePkg/Universal/Capsule/Library/OpensslLib/OpensslLibCode.inf
!else
  OpensslLib                       | LsRefCodePkg/Universal/Capsule/Library/OpensslLib/OpensslLib.inf
!endif
  IntrinsicLib                     | LsRefCodePkg/Universal/Capsule/Library/IntrinsicLib/IntrinsicLib.inf
  BaseCryptLib                     | LsRefCodePkg/Universal/Capsule/Library/BaseCryptLib/BaseCryptLib.inf
  CapsuleLib                       | LsRefCodePkg/Universal/Capsule/Library/DxeCapsuleLibFmp/DxeCapsuleLib.inf
  EdkiiSystemCapsuleLib            | SignedCapsulePkg/Library/EdkiiSystemCapsuleLib/EdkiiSystemCapsuleLib.inf
  FmpAuthenticationLib             | MdeModulePkg/Library/FmpAuthenticationLibNull/FmpAuthenticationLibNull.inf
  IniParsingLib                    | SignedCapsulePkg/Library/IniParsingLib/IniParsingLib.inf
  PlatformFlashAccessLib           | LsRefCodePkg/Universal/Capsule/Library/PlatformFlashAccessLib/PlatformFlashAccessLibDxe.inf
!else
  CapsuleLib                       | MdeModulePkg/Library/DxeCapsuleLibNull/DxeCapsuleLibNull.inf
!endif
  DxeServicesLib                   | MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  DxeServicesTableLib              | MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf
  PeCoffGetEntryPointLib           | MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
  PciLib                           | MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
  IoLib                            | MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  OemHookStatusCodeLib             | MdeModulePkg/Library/OemHookStatusCodeLibNull/OemHookStatusCodeLibNull.inf
  SerialPortLib                    | LsRefCodePkg/Library/SerialPortLib/SerialPortLib.inf
  I2cBusLib                        | LsRefCodePkg/Core/Library/I2cBusLib/I2cBusLib.inf
  CpuGpioLib                       | LsRefCodePkg/Core/Library/CpuGpioLib/GpioLib.inf
  EfiResetSystemLib                | LsRefCodePkg/Library/ResetSystemLib/ResetSystemLib.inf
  UefiBootServicesTableLib         | MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib      | MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  UefiDriverEntryPoint             | MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiApplicationEntryPoint        | MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  DevicePathLib                    | MdePkg/Library/UefiDevicePathLibDevicePathProtocol/UefiDevicePathLibDevicePathProtocol.inf
  NvVarsFileLib                    | OvmfPkg/Library/NvVarsFileLib/NvVarsFileLib.inf
  FileHandleLib                    | MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
  SecurityManagementLib            | MdeModulePkg/Library/DxeSecurityManagementLib/DxeSecurityManagementLib.inf
  NetLib                           | MdeModulePkg/Library/DxeNetLib/DxeNetLib.inf
  IpIoLib                          | MdeModulePkg/Library/DxeIpIoLib/DxeIpIoLib.inf
  UdpIoLib                         | MdeModulePkg/Library/DxeUdpIoLib/DxeUdpIoLib.inf
  DpcLib                           | MdeModulePkg/Library/DxeDpcLib/DxeDpcLib.inf
  UefiUsbLib                       | MdePkg/Library/UefiUsbLib/UefiUsbLib.inf
  SerializeVariablesLib            | OvmfPkg/Library/SerializeVariablesLib/SerializeVariablesLib.inf
  CustomizedDisplayLib             | MdeModulePkg/Library/CustomizedDisplayLib/CustomizedDisplayLib.inf
  ResetSystemLib                   | LsRefCodePkg/Library/ResetSystemLib/ResetSystemLib.inf
  DebugPrintErrorLevelLib          | MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
  TpmMeasurementLib                | MdeModulePkg/Library/TpmMeasurementLibNull/TpmMeasurementLibNull.inf
  AuthVariableLib                  | MdeModulePkg/Library/AuthVariableLibNull/AuthVariableLibNull.inf
  VarCheckLib                      | MdeModulePkg/Library/VarCheckLib/VarCheckLib.inf
  SortLib                          | MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf
  SpiLib                           | LsRefCodePkg/Core/Library/SpiLib/SpiLib.inf
  LoadFileLib                      | LsRefCodePkg/Library/LoadFileLib/LoadFileLib.inf

!if $(HTTP_BOOT_ENABLE) == TRUE
  HttpLib                          | MdeModulePkg/Library/DxeHttpLib/DxeHttpLib.inf
!endif
  UefiBootManagerLib               | MdeModulePkg/Library/UefiBootManagerLib/UefiBootManagerLib.inf
  SmbusLib                         | MdePkg/Library/BaseSmbusLibNull/BaseSmbusLibNull.inf
  OrderedCollectionLib             | MdePkg/Library/BaseOrderedCollectionRedBlackTreeLib/BaseOrderedCollectionRedBlackTreeLib.inf
  HobLib                           | MdePkg/Library/PeiHobLib/PeiHobLib.inf
  ReportStatusCodeLib              | MdePkg/Library/BaseReportStatusCodeLibNull/BaseReportStatusCodeLibNull.inf

  PlatformBootManagerLib           | LsRefCodePkg/Library/PlatformBootManagerLib/PlatformBootManagerLib.inf
#PlatformHookLib                  | CorebootPayloadPkg/Library/PlatformHookLib/PlatformHookLib.inf
  CbParseLib                       | CorebootModulePkg/Library/CbParseLib/CbParseLib.inf
  PciCf8Lib                        | LsRefCodePkg/Core/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  FileExplorerLib                  | MdeModulePkg/Library/FileExplorerLib/FileExplorerLib.inf

[LibraryClasses.common]
  PeCoffGetEntryPointLib           | MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
  PeCoffExtraActionLib             | MdePkg/Library/BasePeCoffExtraActionLibNull/BasePeCoffExtraActionLibNull.inf
  DebugAgentLib                    | MdeModulePkg/Library/DebugAgentLibNull/DebugAgentLibNull.inf
  CpuExceptionHandlerLib           | MdeModulePkg/Library/CpuExceptionHandlerLibNull/CpuExceptionHandlerLibNull.inf

  SysPowerLib                      | LsRefCodePkg/Core/Library/SysPowerLib/SysPowerLib.inf
  Ls7aGpioLib                      | LsRefCodePkg/Core/Library/Ls7aGpioLib/Ls7aGpio.inf
  GrubLib                          | LsRefCodePkg/Library/GrubLib/GrubLib.inf

[LibraryClasses.common.SEC]
  DebugLib                         | MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  DebugAgentLib                    | MdeModulePkg/Library/DebugAgentLibNull/DebugAgentLibNull.inf
  ReportStatusCodeLib              | MdeModulePkg/Library/PeiReportStatusCodeLib/PeiReportStatusCodeLib.inf
  HobLib                           | MdePkg/Library/PeiHobLib/PeiHobLib.inf
  PeiServicesLib                   | MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  MemoryAllocationLib              | MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  PeCoffLib                        | MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf

[LibraryClasses.common.PEI_CORE]
  HobLib                           | MdePkg/Library/PeiHobLib/PeiHobLib.inf
  PeiServicesLib                   | MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  PeiServicesTablePointerLib       | MdePkg/Library/PeiServicesTablePointerLib/PeiServicesTablePointerLib.inf
  MemoryAllocationLib              | MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  PeiCoreEntryPoint                | MdePkg/Library/PeiCoreEntryPoint/PeiCoreEntryPoint.inf
  ReportStatusCodeLib              | MdeModulePkg/Library/PeiReportStatusCodeLib/PeiReportStatusCodeLib.inf
  OemHookStatusCodeLib             | MdeModulePkg/Library/OemHookStatusCodeLibNull/OemHookStatusCodeLibNull.inf
  PeCoffGetEntryPointLib           | MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
  DebugLib                         | MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  PeCoffLib                        | MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf

[LibraryClasses.common.PEIM]
  PcdLib                           | MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  HobLib                           | MdePkg/Library/PeiHobLib/PeiHobLib.inf
  PeiServicesLib                   | MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  PeiServicesTablePointerLib       | MdePkg/Library/PeiServicesTablePointerLib/PeiServicesTablePointerLib.inf
  MemoryAllocationLib              | MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  PeimEntryPoint                   | MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
  ReportStatusCodeLib              | MdeModulePkg/Library/PeiReportStatusCodeLib/PeiReportStatusCodeLib.inf
  OemHookStatusCodeLib             | MdeModulePkg/Library/OemHookStatusCodeLibNull/OemHookStatusCodeLibNull.inf
  PeCoffGetEntryPointLib           | MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
  DebugLib                         | MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  PeCoffLib                        | MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf
  PeiResourcePublicationLib        | MdePkg/Library/PeiResourcePublicationLib/PeiResourcePublicationLib.inf
  ExtractGuidedSectionLib          | MdePkg/Library/PeiExtractGuidedSectionLib/PeiExtractGuidedSectionLib.inf
  MpInitLib                        | LsRefCodePkg/Core/Library/MpInitLib/PeiMpInitLib.inf
  PeiLsPtPtrLib                    | LsRefCodePkg/Library/PeiLsPlatformTablePointerLib/PeiLsPlatformTablePointer.inf

[LibraryClasses.common.DXE_CORE]
  HobLib                           | MdePkg/Library/DxeCoreHobLib/DxeCoreHobLib.inf
  DxeCoreEntryPoint                | MdePkg/Library/DxeCoreEntryPoint/DxeCoreEntryPoint.inf
  MemoryAllocationLib              | MdeModulePkg/Library/DxeCoreMemoryAllocationLib/DxeCoreMemoryAllocationLib.inf
  ReportStatusCodeLib              | MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
  DebugLib                         | MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf

[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  PcdLib                           | MdePkg/Library/DxePcdLib/DxePcdLib.inf
  HobLib                           | MdePkg/Library/DxeHobLib/DxeHobLib.inf
  DxeCoreEntryPoint                | MdePkg/Library/DxeCoreEntryPoint/DxeCoreEntryPoint.inf
  MemoryAllocationLib              | MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  ReportStatusCodeLib              | MdeModulePkg/Library/RuntimeDxeReportStatusCodeLib/RuntimeDxeReportStatusCodeLib.inf
  DebugLib                         | MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  UefiRuntimeLib                   | MdePkg/Library/UefiRuntimeLib/UefiRuntimeLib.inf
  ExtractGuidedSectionLib          | MdePkg/Library/PeiExtractGuidedSectionLib/PeiExtractGuidedSectionLib.inf
!if $(CAPSULE_ENABLE)
  CapsuleLib                       | LsRefCodePkg/Universal/Capsule/Library/DxeCapsuleLibFmp/DxeRuntimeCapsuleLib.inf
  BaseCryptLib                     | LsRefCodePkg/Universal/Capsule/Library/BaseCryptLib/RuntimeCryptLib.inf
!endif


[LibraryClasses.common.UEFI_DRIVER]
  PcdLib                           | MdePkg/Library/DxePcdLib/DxePcdLib.inf
  HobLib                           | MdePkg/Library/DxeHobLib/DxeHobLib.inf
  DxeCoreEntryPoint                | MdePkg/Library/DxeCoreEntryPoint/DxeCoreEntryPoint.inf
  MemoryAllocationLib              | MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  ReportStatusCodeLib              | MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
  DebugLib                         | MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  UefiScsiLib                      | MdePkg/Library/UefiScsiLib/UefiScsiLib.inf
  ExtractGuidedSectionLib          | MdePkg/Library/PeiExtractGuidedSectionLib/PeiExtractGuidedSectionLib.inf

[LibraryClasses.common.DXE_DRIVER]
  PcdLib                           | MdePkg/Library/DxePcdLib/DxePcdLib.inf
  HobLib                           | MdePkg/Library/DxeHobLib/DxeHobLib.inf
  MemoryAllocationLib              | MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  ReportStatusCodeLib              | MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
  UefiScsiLib                      | MdePkg/Library/UefiScsiLib/UefiScsiLib.inf
  DebugLib                         | MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  NetLib                           | MdeModulePkg/Library/DxeNetLib/DxeNetLib.inf
  IpIoLib                          | MdeModulePkg/Library/DxeIpIoLib/DxeIpIoLib.inf
  UdpIoLib                         | MdeModulePkg/Library/DxeUdpIoLib/DxeUdpIoLib.inf
  DpcLib                           | MdeModulePkg/Library/DxeDpcLib/DxeDpcLib.inf
  CpuExceptionHandlerLib           | UefiCpuPkg/Library/CpuExceptionHandlerLib/DxeCpuExceptionHandlerLib.inf
  ExtractGuidedSectionLib          | MdePkg/Library/DxeExtractGuidedSectionLib/DxeExtractGuidedSectionLib.inf
#Int10ProtocolLib                 | LsRefCodePkg/Library/int10/int10_protocol.inf
  MpInitLib                        | LsRefCodePkg/Core/Library/MpInitLib/DxeMpInitLib.inf

[LibraryClasses.common.UEFI_APPLICATION]
  PcdLib                           | MdePkg/Library/DxePcdLib/DxePcdLib.inf
  HobLib                           | MdePkg/Library/DxeHobLib/DxeHobLib.inf
  MemoryAllocationLib              | MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  DebugLib                         | MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  ExtractGuidedSectionLib          | MdePkg/Library/DxeExtractGuidedSectionLib/DxeExtractGuidedSectionLib.inf


################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform.
#
################################################################################
[PcdsFeatureFlag]
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial                | TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseMemory                | TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdDxeIplSupportUefiDecompress        | TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutGopSupport                   | TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutUgaSupport                   | FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdPciBusHotplugDeviceSupport         | FALSE
  gUefiLoongsonPlatformPkgTokenSpaceGuid.PcdSupportGop                 | TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdVariableCollectStatistics          | FALSE
  gEfiMdePkgTokenSpaceGuid.PcdUefiVariableDefaultLangDeprecate         | FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSupportUpdateCapsuleReset          | TRUE

[PcdsFixedAtBuild]
  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerMenuFile                | { 0x21, 0xaa, 0x2c, 0x46, 0x14, 0x76, 0x03, 0x45, 0x83, 0x6e, 0x8a, 0xb6, 0xf4, 0x66, 0x23, 0x31 }
  gEfiLoongsonFrameworkModulePkgTokenSpaceGuid.PcdShellFile            | { 0x83, 0xA5, 0x04, 0x7C, 0x3E, 0x9E, 0x1C, 0x4F, 0xAD, 0x65, 0xE0, 0x52, 0x68, 0xD0, 0xB4, 0xD1 }
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingBase64     | 0
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeMemorySize               | 1
  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange | FALSE
  gEfiMdePkgTokenSpaceGuid.PcdMaximumGuidedExtractHandler              | 0x10
  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreMaxFvSupported              | 6
  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreMaxPeimPerFv                | 32
  gEfiMdeModulePkgTokenSpaceGuid.PcdVpdBaseAddress                     | 0x0
  gEmbeddedTokenSpaceGuid.PcdPrePiCpuMemorySize                        | 64
  gEmbeddedTokenSpaceGuid.PcdPrePiCpuIoSize                            | 64
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask             | 0x07
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel                     | 0x8000004F
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask                        | 0x2F
  gEmbeddedTokenSpaceGuid.PcdEmbeddedAutomaticBootCommand              | ""
  gEfiMdePkgTokenSpaceGuid.PcdPciIoBase                                | $(UNCACHED_MEMORY_ADDR) + 0xefdfc000000
  gEfiMdePkgTokenSpaceGuid.PcdPciIoSize                                | 0x2000000

  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingBase
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingSize
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareBase
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareSize
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableSize         |0x00010000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableBase64       |$(UNCACHED_MEMORY_ADDR) + 0x1c001000
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVariableSize                    |0x2000
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxHardwareErrorVariableSize       |0x400
  gEfiMdeModulePkgTokenSpaceGuid.PcdVariableStoreSize                  |0xffb8
  gEfiMdeModulePkgTokenSpaceGuid.PcdHwErrStorageSize                   |0x400
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxUserNvVariableSpaceSize         |0xb000
  gEfiMdeModulePkgTokenSpaceGuid.PcdBoottimeReservedNvVariableSpaceSize|0x4000
  gUefiLoongsonPlatformPkgTokenSpaceGuid.PcdFlashFdBase                |$(UNCACHED_MEMORY_ADDR) + 0x1c000000
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiExposedTableVersions           |0x04
  gUefiLoongsonPlatformPkgTokenSpaceGuid.FvHeaderOffset                |0x1000

################################################################################
#
# Pcd Dynamic Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsDynamicDefault]
  gEfiMdeModulePkgTokenSpaceGuid.PcdEmuVariableNvStoreReserved          | 0
  gEfiMdeModulePkgTokenSpaceGuid.PcdPciDisableBusEnumeration            | FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution           | 800
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution             | 600
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut                       | 3
  gUefiLoongsonPlatformPkgTokenSpaceGuid.PcdFlashFvHeaderOffset         | 0

  # Set video resolution for text setup.
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution      | 640
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution        | 480
  gEfiMdeModulePkgTokenSpaceGuid.PcdSmbiosVersion                       | 0x0302
  gEfiMdeModulePkgTokenSpaceGuid.PcdSmbiosDocRev                        | 0x0

[PcdsDynamicExDefault.common.DEFAULT]
!if $(CAPSULE_ENABLE)
  gEfiSignedCapsulePkgTokenSpaceGuid.PcdEdkiiSystemFirmwareImageDescriptor|{0x0}|VOID*|0x100
  gEfiMdeModulePkgTokenSpaceGuid.PcdSystemFmpCapsuleImageTypeIdGuid     | { 0x6B, 0x19, 0x17, 0x5F, 0xC3, 0x7B, 0x16, 0xC8, 0x7E, 0xA8, 0xC8, 0x37, 0xD3, 0x3E, 0x07, 0x15 }
  gEfiSignedCapsulePkgTokenSpaceGuid.PcdEdkiiSystemFirmwareFileGuid     | { 0x74, 0xA2, 0x51, 0x51, 0x14, 0x26, 0xA7, 0x4E, 0xC6, 0x60, 0x2D, 0xBE, 0x3C, 0xA2, 0xCC, 0xB3 }
!endif

[Components]

  #
  # SEC Phase modules
  #
  LsRefCodePkg/Core/SecMain/SecMain.inf

!if $(CAPSULE_ENABLE)
  # FMP image decriptor
  LsRefCodePkg/Universal/Capsule/SystemFirmwareDescriptor/SystemFirmwareDescriptor.inf
!endif

  #
  # PEI Phase modules
  #
  MdeModulePkg/Core/Pei/PeiMain.inf
  MdeModulePkg/Universal/PCD/Pei/Pcd.inf  {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }
  LsRefCodePkg/Universal/StatusCode/Pei/StatusCodePei.inf
  MdePkg/Library/PeiExtractGuidedSectionLib/PeiExtractGuidedSectionLib.inf
  MdeModulePkg/Core/DxeIplPeim/DxeIpl.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  }
  LsRefCodePkg/SampleCode/Desktop/Ls3aPlatformTable/Pei/PeiLs3aPlatformTableInit.inf
  LsRefCodePkg/SampleCode/Desktop/Ls7aPlatformTable/Pei/PeiLs7aPlatformTableInit.inf
  LsRefCodePkg/SampleCode/Desktop/HdaVerbTable/HdaVerbTablePpi.inf
!if $(MRC_BINARY)
  LsRefCodePkg/MRC/Binary/MemConfigPpi.inf
!else
  LsRefCodePkg/MRC/source/MemConfigPpi.inf
!endif
  LsRefCodePkg/Core/RcCorePpi/RcCorePpi.inf
  LsRefCodePkg/SampleCode/Desktop/CustomedPpi/CustomedPpi.inf
  LsRefCodePkg/SampleCode/PlatformInitPei/PlatformInitPei.inf
  LsRefCodePkg/Universal/Variable/VariablePei/VariablePei.inf
  LsRefCodePkg/Universal/Capsule/CapsulePei/CapsulePei.inf
  #
  # DXE Phase modules
  #
  MdeModulePkg/Core/Dxe/DxeMain.inf {
    <LibraryClasses>
      NULL                             | MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
      DevicePathLib                    | MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
      ExtractGuidedSectionLib          | MdePkg/Library/DxeExtractGuidedSectionLib/DxeExtractGuidedSectionLib.inf
  }

  LsRefCodePkg/Universal/StatusCode/RuntimeDxe/StatusCodeRuntimeDxe.inf
  MdeModulePkg/Universal/PCD/Dxe/Pcd.inf  {
   <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }

  MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  LsRefCodePkg/Core/Arch/Ls3a/CpuDxe/CpuDxe.inf
  LsRefCodePkg/Drivers/PowerButtonEventDxe/PowerButtonEventDxe.inf
  MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  MdeModulePkg/Universal/MonotonicCounterRuntimeDxe/MonotonicCounterRuntimeDxe.inf
  LsRefCodePkg/Drivers/ResetRuntimeDxe/ResetRuntimeDxe.inf
  MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  LsRefCodePkg/Core/Arch/Ls3a/StableTimerDxe/TimerDxe.inf
  MdeModulePkg/Universal/Metronome/Metronome.inf
  LsRefCodePkg/Universal/RealTimeClockRuntimeDxe/LoongArchRealTimeClockRuntimeDxe.inf
  MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf {
    <LibraryClasses>
      DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }
  UefiCpuPkg/CpuIo2Dxe/CpuIo2Dxe.inf

  #
  # run command
  #
  LsRefCodePkg/Drivers/LoadRawFile/LoadRawFile.inf

  #
  # Loongson Service
  #
  LsRefCodePkg/Core/LsServiceDxe/LsServiceDxe.inf
  LsRefCodePkg/Core/LsServiceDxe/LsRTServiceDxe.inf

  #
  # Variable
  #
  LsRefCodePkg/Drivers/FlashService/FlashService.inf
  LsRefCodePkg/Drivers/FvBlockService/FvBlockService.inf
  LsRefCodePkg/Universal/Variable/VariableDxe/VariableRuntimeDxe.inf

  #
  # PCI
  #
  LsRefCodePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf
  LsRefCodePkg/Bus/Pci/PciHostBridgeDxe/PciHostBridgeDxe.inf{
    <LibraryClasses>
     PciSegmentLib|MdePkg/Library/BasePciSegmentLibPci/BasePciSegmentLibPci.inf
     PciCf8Lib|LsRefCodePkg/Core/Library/BasePciCf8Lib/BasePciCf8Lib.inf
     PciHostBridgeLib|LsRefCodePkg/Library/PciHostBridgeLib/PciHostBridgeLib.inf
  }
  LsRefCodePkg/Bus/Pci/PciDeviceDxe/PciDeviceDxe.inf

  #
  # InitLs7aPcieVariable
  #
  LsRefCodePkg/Drivers/InitLs7aPcieVariableDxe/InitLs7aPcieVariableDxe.inf

  #
  # ISA Support
  #
  LsRefCodePkg/Drivers/IsaAcpiDxe/IsaAcpi.inf
  MdeModulePkg/Universal/SerialDxe/SerialDxe.inf

  #
  # Console
  #
  MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  LsRefCodePkg/Drivers/Console/ConSplitterDxe/ConSplitterDxe.inf
!if $(COMP_TYPE) == DEBUG
  MdeModulePkg/Universal/Console/TerminalDxe/TerminalDxe.inf
!endif
  MdeModulePkg/Universal/PrintDxe/PrintDxe.inf

  #
  # DC
  #
  LsRefCodePkg/Drivers/LsGraphics/Graphics.inf
  #
  # Video
  #
  LsRefCodePkg/Drivers/BiosVideoThunkDxe/BiosVideo.inf{
    <LibraryClasses>
      Int10ProtocolLib|LsRefCodePkg/Library/int10/int10_protocol.inf
  }
  LsRefCodePkg/Library/int10/x86emu.inf{
    <LibraryClasses>
      Int10ProtocolLib|LsRefCodePkg/Library/int10/int10_protocol.inf
  }
  MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }


  #
  # IDE
  #
  MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  MdeModulePkg/Bus/Pci/SataControllerDxe/SataControllerDxe.inf
  MdeModulePkg/Bus/Ata/AtaAtapiPassThru/AtaAtapiPassThru.inf
  MdeModulePkg/Bus/Ata/AtaBusDxe/AtaBusDxe.inf
  MdeModulePkg/Bus/Scsi/ScsiBusDxe/ScsiBusDxe.inf
  MdeModulePkg/Bus/Scsi/ScsiDiskDxe/ScsiDiskDxe.inf
  MdeModulePkg/Bus/Pci/NvmExpressDxe/NvmExpressDxe.inf

  #
  # Usb Support
  #
  LsRefCodePkg/Bus/Pci/OhciDxe/OhciDxe.inf
  MdeModulePkg/Bus/Pci/XhciDxe/XhciDxe.inf
  MdeModulePkg/Bus/Pci/EhciDxe/EhciDxe.inf
  LsRefCodePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  MdeModulePkg/Bus/Usb/UsbMouseDxe/UsbMouseDxe.inf
  MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  #
  #BDS
  #
  LsRefCodePkg/Universal/BdsDxe/BdsDxe.inf{
    <LibraryClasses>
!if $(CAPSULE_ENABLE)
      FmpAuthenticationLib|LsRefCodePkg/Universal/Capsule/Library/FmpAuthenticationLibPkcs7/FmpAuthenticationLibPkcs7.inf
!else
      FmpAuthenticationLib|MdeModulePkg/Library/FmpAuthenticationLibNull/FmpAuthenticationLibNull.inf
!endif

  }
  LsRefCodePkg/Universal/Logo/LogoDxe.inf
  LsRefCodePkg/Application/UiApp/UiApp.inf {
    <LibraryClasses>
      NULL|LsRefCodePkg/Universal/Library/DeviceManagerUiLib/DeviceManagerUiLib.inf
      NULL|LsRefCodePkg/Universal/Library/BootManagerUiLib/BootManagerUiLib.inf
      NULL|LsRefCodePkg/Universal/Library/BootMaintenanceManagerUiLib/BootMaintenanceManagerUiLib.inf
  }
  MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
  LsRefCodePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf
  LsRefCodePkg/Universal/DisplayEngineDxe/DisplayEngineDxe.inf
  LsRefCodePkg/Drivers/DeviceManagerDxe/DeviceManagerDxe.inf
  LsRefCodePkg/Drivers/ShowTimeDxe/ShowTime.inf
  MdeModulePkg/Universal/MemoryTest/NullMemoryTestDxe/NullMemoryTestDxe.inf
  LsRefCodePkg/Drivers/BootParamsInterfaceDxe/BootParamsInterfaceDxe.inf
  LsRefCodePkg/Universal/SmbiosDxe/SmbiosDxe.inf
  LsRefCodePkg/SampleCode/Desktop/SmbiosTable/SmbiosTable.inf

  #
  # ACPI Support
  #
  LsRefCodePkg/SampleCode/Desktop/Acpi/AcpiTableDxe/AcpiTableDxe.inf
  LsRefCodePkg/SampleCode/Desktop/Acpi/AcpiPlatformDxe/AcpiPlatformDxe.inf
  LsRefCodePkg/SampleCode/Desktop/Acpi/AcpiTables/AcpiTables.inf
  #MdeModulePkg/Universal/Acpi/S3SaveStateDxe/S3SaveStateDxe.inf
  #MdeModulePkg/Universal/Acpi/BootScriptExecutorDxe/BootScriptExecutorDxe.inf
  #MdeModulePkg/Universal/Acpi/BootGraphicsResourceTableDxe/BootGraphicsResourceTableDxe.inf

  #
  # Network Support
  #

!if $(NETWORK_SUPPORT) == TRUE
  MdeModulePkg/Universal/Network/SnpDxe/SnpDxe.inf
  MdeModulePkg/Universal/Network/DpcDxe/DpcDxe.inf
  MdeModulePkg/Universal/Network/MnpDxe/MnpDxe.inf
  MdeModulePkg/Universal/Network/VlanConfigDxe/VlanConfigDxe.inf
  MdeModulePkg/Universal/Network/ArpDxe/ArpDxe.inf
  MdeModulePkg/Universal/Network/Dhcp4Dxe/Dhcp4Dxe.inf
  MdeModulePkg/Universal/Network/Ip4Dxe/Ip4Dxe.inf
  MdeModulePkg/Universal/Network/Mtftp4Dxe/Mtftp4Dxe.inf
  MdeModulePkg/Universal/Network/Udp4Dxe/Udp4Dxe.inf
  MdeModulePkg/Universal/Network/Tcp4Dxe/Tcp4Dxe.inf
  MdeModulePkg/Universal/Network/UefiPxeBcDxe/UefiPxeBcDxe.inf
  MdeModulePkg/Universal/Network/IScsiDxe/IScsiDxe.inf
  LsRefCodePkg/Drivers/GmacUndiDxe/GmacUndiDxe.inf
  GigUndiDxe/GigUndiDxe.inf
#RtkUndiDxe/RtkUndiDxe.inf
!if $(HTTP_BOOT_ENABLE) == TRUE
  NetworkPkg/DnsDxe/DnsDxe.inf
  NetworkPkg/HttpDxe/HttpDxe.inf
  NetworkPkg/HttpBootDxe/HttpBootDxe.inf
!endif
!endif

  #
  #app
  #
  ShellPkg/Application/Shell/Shell.inf {
    <LibraryClasses>
      ShellCommandLib|ShellPkg/Library/UefiShellCommandLib/UefiShellCommandLib.inf
      NULL|ShellPkg/Library/UefiShellLevel2CommandsLib/UefiShellLevel2CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel1CommandsLib/UefiShellLevel1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel3CommandsLib/UefiShellLevel3CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDriver1CommandsLib/UefiShellDriver1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDebug1CommandsLib/UefiShellDebug1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellInstall1CommandsLib/UefiShellInstall1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellNetwork1CommandsLib/UefiShellNetwork1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellSpiCommandsLib/UefiShellSpiCommandsLib.inf
      NULL|ShellPkg/Library/UefiShellSignalTestCommandsLib/UefiShellSignalTestCommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLoonsongTestCommandLib/UefiLoongsonTestCommandLib.inf
      NULL|ShellPkg/Library/UefiVersCommandLib/UefiVersCommandLib.inf
!if $(CAPSULE_ENABLE)
      NULL|ShellPkg/Library/UefiShellCapsuleCommandsLib/UefiCapsuleCommandsLib.inf
!endif
      HandleParsingLib|ShellPkg/Library/UefiHandleParsingLib/UefiHandleParsingLib.inf
      ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
      FileHandleLib|MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
      SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf
      PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
      BcfgCommandLib|ShellPkg/Library/UefiShellBcfgCommandLib/UefiShellBcfgCommandLib.inf
<PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0xFF
      gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE
      gEfiMdePkgTokenSpaceGuid.PcdUefiLibMaxPrintBufferSize|8000
  }
  ShellPkg/Application/ShellExecTestApp/SA.inf

!if $(CAPSULE_ENABLE)
  MdeModulePkg/Universal/EsrtDxe/EsrtDxe.inf

  SignedCapsulePkg/Universal/SystemFirmwareUpdate/SystemFirmwareReportDxe.inf {
    <LibraryClasses>
      FmpAuthenticationLib|LsRefCodePkg/Universal/Capsule/Library/FmpAuthenticationLibPkcs7/FmpAuthenticationLibPkcs7.inf
  }
  SignedCapsulePkg/Universal/SystemFirmwareUpdate/SystemFirmwareUpdateDxe.inf {
    <LibraryClasses>
      FmpAuthenticationLib|LsRefCodePkg/Universal/Capsule/Library/FmpAuthenticationLibPkcs7/FmpAuthenticationLibPkcs7.inf
  }

  MdeModulePkg/Application/CapsuleApp/CapsuleApp.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
!endif

  #
  # File system
  #
  FatPkg/EnhancedFatDxe/Fat.inf
  LsRefCodePkg/Drivers/FsDxe/FsDxe.inf

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
DEFINE CACHED_MEMORY_ADDR         = 0x9000000000000000
DEFINE UNCACHED_MEMORY_ADDR       = 0x8000000000000000

DEFINE STARTCODE_OFFSET           = 0x00000000
DEFINE STARTCODE_SIZE             = 0x1000

DEFINE VARIABLE_OFFSET            = $(STARTCODE_OFFSET) + $(STARTCODE_SIZE)
DEFINE VARIABLE_SIZE              = 0x10000

DEFINE RESERVE1_OFFSET            = $(VARIABLE_OFFSET) + $(VARIABLE_SIZE)
DEFINE RESERVE1_SIZE              = 0xB000

DEFINE RESERVE2_OFFSET            = $(RESERVE1_OFFSET) + $(RESERVE1_SIZE)
DEFINE RESERVE2_SIZE              = 0x24000

DEFINE SPARE_OFFSET               = $(RESERVE2_OFFSET) + $(RESERVE2_SIZE)
DEFINE SPARE_SIZE                 = 0x10000

DEFINE FWTWORKING_OFFSET          = $(SPARE_OFFSET) + $(SPARE_SIZE)
DEFINE FWTWORKING_SIZE            = 0x8000

DEFINE EVENT_LOG_OFFSET           = $(FWTWORKING_OFFSET) + $(FWTWORKING_SIZE)
DEFINE EVENT_LOG_SIZE             = 0x8000

DEFINE FD_BASE_ADDRESS            = 0x800000001c000000
DEFINE BLOCK_SIZE                 = 0x1000

DEFINE FD_BLOCKS                  = 0x400
DEFINE FD_SIZE                    = 0x400000

#Set Sec base address and size in flash
DEFINE SECFV_OFFSET               = 0x00060000
DEFINE SECFV_SIZE                 = 0x00020000

#Set Pei base address and size in flash
DEFINE PEIFV_OFFSET               = 0x00090000
DEFINE PEIFV_SIZE                 = 0x000e0000

#Set Dxe base address and size in flash
DEFINE DXEFV_OFFSET               = 0x00170000

DEFINE DXEFV_SIZE                 = 0x00280000

# Set cache as ram address
DEFINE HIGH_MEM_BASE_ADDRESS      = 0x9000000090000000
DEFINE CACHEASRAM_BASE_ADDRESS    = $(HIGH_MEM_BASE_ADDRESS)
DEFINE CACHEASRAM_RAM_SIZE        = 0x20000
DEFINE HEAP_SIZE                  = 0x8000

DEFINE FLASH_CODE_SECFV_BASE_ADDRESS = 0x800000001c060000
DEFINE FLASH_CODE_PEIFV_BASE_ADDRESS = 0x800000001c090000
DEFINE CACHE_CODE_SECFV_BASE_ADDRESS = 0x9000000090100000
DEFINE CACHE_CODE_PEIFV_BASE_ADDRESS = 0x9000000090120000
DEFINE MEM_CODE_DXEFV_BASE_ADDRESS   = 0x90000000c00C0000

DEFINE LOONGSON_MEM_REGIONHOB_BOTTOM = 0x90000000c0000000
DEFINE LOONGSON_MEM_REGIONHOB_TOP    = 0x90000000ff000000

DEFINE MEM_CODE_BASE_ADDRESS      = 0x90000000c0000000
DEFINE MEM_CODE_START             = 0x00000000

DEFINE MEM_CODE_HANDLERTABLESIZE  = 0x20000

DEFINE CACHE_CODE_PEIFV_OFFSET      = 0x40000
DEFINE CACHE_CODE_PEIFV_SIZE        = 0xe0000

DEFINE MEM_CODE_DXEFV_OFFSET      = 0x120000
DEFINE MEM_CODE_DXEFV_SIZE        = 0xb00000

DEFINE MEM_CODE_SIZE              = 0xc20000
DEFINE MEM_CODE_BLOCKS            = 0xc2

# Front Side Bus
DEFINE FRONT_SIDE_BUS_MEM_ADDRESS          = 0x80000e0000000000
DEFINE CHIPSET_CONFBUS_BASE_ADDRESS_OFFSET = 0x10010000
DEFINE CHIPSET_MISC_BASE_ADDRESS_OFFSET    = 0x10080000

# ACPI
DEFINE SUSPEND_OFFSET             = 0x700
DEFINE ACPI_REG_BASE_OFFSET       = 0x50000

# Chipset Integrated GPU PCI memory size
!if $(SOUTH_BRIDGE) == 7A1000
DEFINE INTEGRATED_GRAPHICES_PCI_MEM_SIZE  = 0x7FFFFFF
!if $(EJ188_SUPPORT) == TRUE
DEFINE INTEGRATED_GRAPHICES_PCI_MEM_SIZE  = 0x3FFFFFF
!endif
!endif

# Set Front Side Bus Base
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdFrontSideBusMemBase  = $(FRONT_SIDE_BUS_MEM_ADDRESS)
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdChipsetMiscBase  = $(FRONT_SIDE_BUS_MEM_ADDRESS) + $(CHIPSET_MISC_BASE_ADDRESS_OFFSET)

# Set ACPI Base and Register
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdAcpiRegBase  = $(FRONT_SIDE_BUS_MEM_ADDRESS) + $(CHIPSET_MISC_BASE_ADDRESS_OFFSET) + $(ACPI_REG_BASE_OFFSET)


# Set Variable
SET gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableBase64 = $(FD_BASE_ADDRESS) + $(VARIABLE_OFFSET)
SET gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableSize = $(VARIABLE_SIZE)

# Set FtwSpare
SET gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareBase64 = $(FD_BASE_ADDRESS) + $(SPARE_OFFSET)
SET gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareSize = $(SPARE_SIZE)

# Set FtwWorking
SET gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingBase64 = gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareBase64 + gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareSize
SET gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingSize = $(FWTWORKING_SIZE)

# Set Event log
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdFlashEventLogBase = gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingBase64 + gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingSize
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdFlashEventLogSize = $(EVENT_LOG_SIZE)

# FD
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdFlashFdBase    = $(FD_BASE_ADDRESS)
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdFlashFdSize    = $(FD_SIZE)
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdFlashBlockSize = $(BLOCK_SIZE)

# SEC FV
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdFlashSecModuleBase = $(FD_BASE_ADDRESS) + $(SECFV_OFFSET)
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdFlashSecModuleSize = $(SECFV_SIZE)

# PEI FV
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdFlashPeiFvBase = $(FD_BASE_ADDRESS) + $(PEIFV_OFFSET)
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdFlashPeiFvSize = $(PEIFV_SIZE)

# DXE FV
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdFlashDxeFvBase = $(MEM_CODE_DXEFV_BASE_ADDRESS)
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdFlashDxeFvSize = $(DXEFV_SIZE)
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdFlashDxeFvOffset = $(DXEFV_OFFSET)

# CACHE AS RAM
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdSecCacheAsRamBase = $(CACHEASRAM_BASE_ADDRESS)
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdSecCacheAsRamSize = $(CACHEASRAM_RAM_SIZE)

SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdPeiCodeRamBase = $(CACHE_CODE_PEIFV_BASE_ADDRESS)
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdPeiCodeRamSize = $(PEIFV_SIZE)

# BDS FV
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdFlashSuspendOffset = $(SUSPEND_OFFSET)

# Integrated Graphics GPU memory size
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdIntegratedGraphicsPciMemSize = $(INTEGRATED_GRAPHICES_PCI_MEM_SIZE)

# RamRegions Hob
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdRamRegionsBottom = $(LOONGSON_MEM_REGIONHOB_BOTTOM)
SET gUefi$(PLATFORMPKG_NAME)PkgTokenSpaceGuid.PcdRamRegionsTop = $(LOONGSON_MEM_REGIONHOB_TOP)

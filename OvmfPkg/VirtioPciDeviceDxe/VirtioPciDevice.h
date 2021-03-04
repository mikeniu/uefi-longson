/** @file

  Internal definitions for the VirtIo PCI Device driver

  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
  All intellectual property rights(Copyright, Patent and Trademark) reserved.
  Copyright (C) 2013, ARM Ltd
  Copyright (c) 2017, AMD Inc, All rights reserved.<BR>

  Any violations of copyright or other intellectual property rights of the Loongson Technology
  Corporation Limited will be held accountable in accordance with the law,
  if you (or any of your subsidiaries, corporate affiliates or agents) initiate
  directly or indirectly any Intellectual Property Assertion or Intellectual Property Litigation:
  (i) against Loongson Technology Corporation Limited or any of its subsidiaries or corporate affiliates,
  (ii) against any party if such Intellectual Property Assertion or Intellectual Property Litigation arises
  in whole or in part from any software, technology, product or service of Loongson Technology Corporation
  Limited or any of its subsidiaries or corporate affiliates, or (iii) against any party relating to the Software.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR
  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION).

**/

#ifndef _VIRTIO_PCI_DEVICE_DXE_H_
#define _VIRTIO_PCI_DEVICE_DXE_H_

#include <Protocol/ComponentName.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/PciIo.h>
#include <Protocol/VirtioDevice.h>

#include <IndustryStandard/Virtio.h>

#define VIRTIO_PCI_DEVICE_SIGNATURE   SIGNATURE_32 ('V', 'P', 'C', 'I')

typedef struct {
  UINT32                 Signature;
  VIRTIO_DEVICE_PROTOCOL VirtioDevice;
  EFI_PCI_IO_PROTOCOL   *PciIo;
  UINT64                 OriginalPciAttributes;
  UINT32                 DeviceSpecificConfigurationOffset;
} VIRTIO_PCI_DEVICE;

#if defined(MDE_CPU_MIPS)
  #define PHYADDR(a) (((a & 0xfffffffff0000000) == 0xffffffff80000000) ? (a & 0xfffffff) : (a & 0xffffffffffff))
#else
  #define PHYADDR(a) a
#endif
#define VIRTIO_PCI_DEVICE_FROM_VIRTIO_DEVICE(Device) \
    CR (Device, VIRTIO_PCI_DEVICE, VirtioDevice, VIRTIO_PCI_DEVICE_SIGNATURE)


EFI_STATUS
EFIAPI
VirtioPciIoRead (
  IN  VIRTIO_PCI_DEVICE         *Dev,
  IN  UINTN                      FieldOffset,
  IN  UINTN                      FieldSize,
  IN  UINTN                      BufferSize,
  OUT VOID                       *Buffer
  );

EFI_STATUS
EFIAPI
VirtioPciIoWrite (
  IN  VIRTIO_PCI_DEVICE         *Dev,
  IN UINTN                       FieldOffset,
  IN UINTN                       FieldSize,
  IN UINT64                      Value
  );

/********************************************
 * PCI Functions for VIRTIO_DEVICE_PROTOCOL
 *******************************************/
EFI_STATUS
EFIAPI
VirtioPciDeviceRead (
  IN  VIRTIO_DEVICE_PROTOCOL     *This,
  IN  UINTN                      FieldOffset,
  IN  UINTN                      FieldSize,
  IN  UINTN                      BufferSize,
  OUT VOID                       *Buffer
  );

EFI_STATUS
EFIAPI
VirtioPciDeviceWrite (
  IN VIRTIO_DEVICE_PROTOCOL      *This,
  IN UINTN                       FieldOffset,
  IN UINTN                       FieldSize,
  IN UINT64                      Value
  );

EFI_STATUS
EFIAPI
VirtioPciGetDeviceFeatures (
  IN VIRTIO_DEVICE_PROTOCOL *This,
  OUT UINT64                *DeviceFeatures
  );

EFI_STATUS
EFIAPI
VirtioPciGetQueueSize (
  IN  VIRTIO_DEVICE_PROTOCOL  *This,
  OUT UINT16                  *QueueNumMax
  );

EFI_STATUS
EFIAPI
VirtioPciSetQueueAlignment (
  IN  VIRTIO_DEVICE_PROTOCOL         *This,
  IN  UINT32                         Alignment
  );

EFI_STATUS
EFIAPI
VirtioPciSetPageSize (
  IN  VIRTIO_DEVICE_PROTOCOL         *This,
  IN  UINT32                         PageSize
  );

EFI_STATUS
EFIAPI
VirtioPciGetDeviceStatus (
  IN  VIRTIO_DEVICE_PROTOCOL  *This,
  OUT UINT8                   *DeviceStatus
  );

EFI_STATUS
EFIAPI
VirtioPciSetGuestFeatures (
  IN VIRTIO_DEVICE_PROTOCOL  *This,
  IN UINT64                   Features
  );

EFI_STATUS
EFIAPI
VirtioPciSetQueueAddress (
  IN VIRTIO_DEVICE_PROTOCOL  *This,
  IN VRING                   *Ring,
  IN UINT64                  RingBaseShift
  );

EFI_STATUS
EFIAPI
VirtioPciSetQueueSel (
  IN  VIRTIO_DEVICE_PROTOCOL         *This,
  IN  UINT16                         Sel
  );

EFI_STATUS
EFIAPI
VirtioPciSetQueueNotify (
  IN  VIRTIO_DEVICE_PROTOCOL         *This,
  IN  UINT16                         Index
  );

EFI_STATUS
EFIAPI
VirtioPciSetQueueSize (
  IN  VIRTIO_DEVICE_PROTOCOL         *This,
  IN  UINT16                         Size
  );

EFI_STATUS
EFIAPI
VirtioPciSetDeviceStatus (
  IN  VIRTIO_DEVICE_PROTOCOL         *This,
  IN  UINT8                          DeviceStatus
  );

EFI_STATUS
EFIAPI
VirtioPciAllocateSharedPages (
  IN  VIRTIO_DEVICE_PROTOCOL        *This,
  IN  UINTN                         NumPages,
  OUT VOID                          **HostAddress
  );

VOID
EFIAPI
VirtioPciFreeSharedPages (
  IN  VIRTIO_DEVICE_PROTOCOL        *This,
  IN  UINTN                         NumPages,
  IN  VOID                          *HostAddress
  );

EFI_STATUS
EFIAPI
VirtioPciMapSharedBuffer (
  IN      VIRTIO_DEVICE_PROTOCOL        *This,
  IN      VIRTIO_MAP_OPERATION          Operation,
  IN      VOID                          *HostAddress,
  IN OUT  UINTN                         *NumberOfBytes,
  OUT     EFI_PHYSICAL_ADDRESS          *DeviceAddress,
  OUT     VOID                          **Mapping
  );

EFI_STATUS
EFIAPI
VirtioPciUnmapSharedBuffer (
  IN  VIRTIO_DEVICE_PROTOCOL        *This,
  IN  VOID                          *Mapping
  );
#endif // _VIRTIO_PCI_DEVICE_DXE_H_

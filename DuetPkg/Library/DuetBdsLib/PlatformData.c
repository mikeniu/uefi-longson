/*++

Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
All intellectual property rights(Copyright, Patent and Trademark) reserved.
Copyright (c) 2006 - 2009, Intel Corporation. All rights reserved.<BR>

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

Module Name: 

  PlatformData.c

Abstract:
  
  Defined the platform specific device path which will be used by
  platform Bbd to perform the platform policy connect.

--*/

#include "BdsPlatform.h"

//
// Predefined platform default time out value
//
UINT16   gPlatformBootTimeOutDefault = 3;

ACPI_HID_DEVICE_PATH       gPnpPs2KeyboardDeviceNode  = gPnpPs2Keyboard;
ACPI_HID_DEVICE_PATH       gPnp16550ComPortDeviceNode = gPnp16550ComPort;
UART_DEVICE_PATH           gUartDeviceNode            = gUart;
VENDOR_DEVICE_PATH         gTerminalTypeDeviceNode    = gPcAnsiTerminal;

#define SERIAL_DXE_FILE_GUID { \
          0xD3987D4B, 0x971A, 0x435F, \
          { 0x8C, 0xAF, 0x49, 0x67, 0xEB, 0x62, 0x72, 0x41 } \
          }


typedef struct {
  VENDOR_DEVICE_PATH         SerialDxe;
  UART_DEVICE_PATH           Uart;
  VENDOR_DEFINED_DEVICE_PATH TermType;
  EFI_DEVICE_PATH_PROTOCOL   End;
} PLATFORM_SERIAL_CONSOLE;

#define DP_NODE_LEN(Type) { (UINT8)sizeof (Type), (UINT8)(sizeof (Type) >> 8) }

STATIC PLATFORM_SERIAL_CONSOLE mSerialConsole = {
  //
  // VENDOR_DEVICE_PATH SerialDxe
  //
  {
    { HARDWARE_DEVICE_PATH, HW_VENDOR_DP, DP_NODE_LEN (VENDOR_DEVICE_PATH) },
    SERIAL_DXE_FILE_GUID
  },

  //
  // UART_DEVICE_PATH Uart
  //
  {
    { MESSAGING_DEVICE_PATH, MSG_UART_DP, DP_NODE_LEN (UART_DEVICE_PATH) },
    0,                                      // Reserved
    115200, // BaudRate
    8,  // DataBits
    1,    // Parity
    1   // StopBits
  },

  //
  // VENDOR_DEFINED_DEVICE_PATH TermType
  //
  {
    {
      MESSAGING_DEVICE_PATH, MSG_VENDOR_DP,
      DP_NODE_LEN (VENDOR_DEFINED_DEVICE_PATH)
    },
    EFI_TTY_TERM_GUID
    //
    // Guid to be filled in dynamically
    //
  },

  //
  // EFI_DEVICE_PATH_PROTOCOL End
  //
  {
    END_DEVICE_PATH_TYPE, END_ENTIRE_DEVICE_PATH_SUBTYPE,
    DP_NODE_LEN (EFI_DEVICE_PATH_PROTOCOL)
  }
};

//
// Predefined platform root bridge
//
PLATFORM_ROOT_BRIDGE_DEVICE_PATH  gPlatformRootBridge0 = {
  gPciRootBridge,
  gEndEntire
};

EFI_DEVICE_PATH_PROTOCOL          *gPlatformRootBridges[] = {
  (EFI_DEVICE_PATH_PROTOCOL *) &gPlatformRootBridge0,
  NULL
};

USB_CLASS_FORMAT_DEVICE_PATH gUsbClassKeyboardDevicePath = {
  {
    {
      MESSAGING_DEVICE_PATH,
      MSG_USB_CLASS_DP,
      {
        (UINT8) (sizeof (USB_CLASS_DEVICE_PATH)),
        (UINT8) ((sizeof (USB_CLASS_DEVICE_PATH)) >> 8)
      }
    },
    0xffff,           // VendorId 
    0xffff,           // ProductId 
    CLASS_HID,        // DeviceClass 
    SUBCLASS_BOOT,    // DeviceSubClass
    PROTOCOL_KEYBOARD // DeviceProtocol
  },

  { 
    END_DEVICE_PATH_TYPE, 
    END_ENTIRE_DEVICE_PATH_SUBTYPE, 
    {
      END_DEVICE_PATH_LENGTH, 
      0
    }
  }
};

/*
//
// Platform specific Dummy ISA keyboard device path
//
PLATFORM_DUMMY_ISA_KEYBOARD_DEVICE_PATH     gDummyIsaKeyboardDevicePath = {
  gPciRootBridge,
  gPciIsaBridge,
  gPnpPs2Keyboard,
  gEndEntire
};

//
// Platform specific Dummy ISA serial device path
//
PLATFORM_DUMMY_ISA_SERIAL_DEVICE_PATH   gDummyIsaSerialDevicePath = {
  gPciRootBridge,
  gPciIsaBridge,
  gPnp16550ComPort,
  gUart,
  gPcAnsiTerminal,
  gEndEntire
};

//
// Platform specific Dummy PCI VGA device path
//
PLATFORM_DUMMY_PCI_VGA_DEVICE_PATH gDummyPciVgaDevicePath = {
  gPciRootBridge,
  PCI_DEVICE_PATH_NODE(0, 0x2),
  gEndEntire
};

//
// Platform specific Dummy PCI serial device path
//
PLATFORM_DUMMY_PCI_SERIAL_DEVICE_PATH gDummyPciSerialDevicePath = {
  gPciRootBridge,
  gP2PBridge,
  PCI_DEVICE_PATH_NODE(0, 0x0),
  gUart,
  gPcAnsiTerminal,
  gEndEntire
};
*/
//
// Predefined platform default console device path
//
BDS_CONSOLE_CONNECT_ENTRY         gPlatformConsole[] = {
  //
  // need update dynamically
  //
//  {
//    (EFI_DEVICE_PATH_PROTOCOL *) &gDummyIsaSerialDevicePath,
//    (CONSOLE_OUT | CONSOLE_IN | STD_ERROR)
//  },
//  {
//    (EFI_DEVICE_PATH_PROTOCOL *) &gDummyIsaKeyboardDevicePath,
//    (CONSOLE_IN | STD_ERROR)
//  },
//  {
//    (EFI_DEVICE_PATH_PROTOCOL *) &gDummyPciVgaDevicePath,
//    CONSOLE_OUT
//  },
//  {
//    (EFI_DEVICE_PATH_PROTOCOL *) &gDummyPciSerialDevicePath,
//    (CONSOLE_OUT | CONSOLE_IN | STD_ERROR)
//  },
  {
    (EFI_DEVICE_PATH_PROTOCOL *) &mSerialConsole,
    (CONSOLE_OUT | CONSOLE_IN | STD_ERROR)
  },
  {
    (EFI_DEVICE_PATH_PROTOCOL*) &gUsbClassKeyboardDevicePath, 
    CONSOLE_IN
  },
  {
    NULL,
    0
  }
};

//
// Predefined platform specific driver option
//
EFI_DEVICE_PATH_PROTOCOL    *gPlatformDriverOption[] = { NULL };

//
// Predefined platform connect sequence
//
EFI_DEVICE_PATH_PROTOCOL    *gPlatformConnectSequence[] = { NULL };


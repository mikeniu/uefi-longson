/** @file
  This protocol provides services to display a popup window.
  The protocol is typically produced by the forms browser and consumed by a driver callback handler.

  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
  All intellectual property rights(Copyright, Patent and Trademark) reserved.
  Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>

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

#ifndef __HII_POPUP_H__
#define __HII_POPUP_H__

#define EFI_HII_POPUP_PROTOCOL_GUID \
  {0x4311edc0, 0x6054, 0x46d4, {0x9e, 0x40, 0x89, 0x3e, 0xa9, 0x52, 0xfc, 0xcc}}

#define EFI_HII_POPUP_PROTOCOL_REVISION 1

//typedef struct _EFI_HII_POPUP_PROTOCOL EFI_HII_POPUP_PROTOCOL;

typedef enum {
  EfiHiiPopupStyleInfo,
  EfiHiiPopupStyleWarning,
  EfiHiiPopupStyleError
} EFI_HII_POPUP_STYLE;

typedef enum {
  EfiHiiPopupTypeOk,
  EfiHiiPopupTypeOkCancel,
  EfiHiiPopupTypeYesNo,
  EfiHiiPopupTypeYesNoCancel
} EFI_HII_POPUP_TYPE;

typedef enum {
  EfiHiiPopupSelectionOk,
  EfiHiiPopupSelectionCancel,
  EfiHiiPopupSelectionYes,
  EfiHiiPopupSelectionNo
} EFI_HII_POPUP_SELECTION;

/**
  Displays a popup window.

  @param  This           A pointer to the EFI_HII_POPUP_PROTOCOL instance.
  @param  PopupStyle     Popup style to use.
  @param  PopupType      Type of the popup to display.
  @param  HiiHandle      HII handle of the string pack containing Message
  @param  Message        A message to display in the popup box.
  @param  UserSelection  User selection.

  @retval EFI_SUCCESS            The popup box was successfully displayed.
  @retval EFI_INVALID_PARAMETER  HiiHandle and Message do not define a valid HII string.
  @retval EFI_INVALID_PARAMETER  PopupType is not one of the values defined by this specification.
  @retval EFI_OUT_OF_RESOURCES   There are not enough resources available to display the popup box.

**/
/*
typedef
EFI_STATUS
(EFIAPI * EFI_HII_CREATE_POPUP) (
  IN  EFI_HII_POPUP_PROTOCOL  *This,
  IN  EFI_HII_POPUP_STYLE     PopupStyle,
  IN  EFI_HII_POPUP_TYPE      PopupType,
  IN  EFI_HII_HANDLE          HiiHandle,
  IN  EFI_STRING_ID           Message,
  OUT EFI_HII_POPUP_SELECTION *UserSelection OPTIONAL
);

typedef struct _EFI_HII_POPUP_PROTOCOL {
  UINT64                Revision;
  EFI_HII_CREATE_POPUP  CreatePopup;
} EFI_HII_POPUP_PROTOCOL;
*/

struct _EFI_HII_POPUP_PROTOCOL {
  UINT64                Revision;
	EFI_STATUS
	(EFIAPI * CreatePopup) (
	  IN  struct _EFI_HII_POPUP_PROTOCOL  *This,
	  IN  EFI_HII_POPUP_STYLE     PopupStyle,
	  IN  EFI_HII_POPUP_TYPE      PopupType,
	  IN  EFI_HII_HANDLE          HiiHandle,
	  IN  EFI_STRING_ID           Message,
	  OUT EFI_HII_POPUP_SELECTION *UserSelection OPTIONAL
	);
};

typedef struct _EFI_HII_POPUP_PROTOCOL EFI_HII_POPUP_PROTOCOL;

extern EFI_GUID gEfiHiiPopupProtocolGuid;

#endif


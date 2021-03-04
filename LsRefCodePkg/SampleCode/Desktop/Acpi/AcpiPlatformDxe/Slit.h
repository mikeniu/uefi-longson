/** @file
  This file describes the contents of the ACPI System Locality Distance Information Table (SLIT).
  Some additional ACPI values are defined in Acpi10.h, Acpi20.h, and Acpi30.h
  All changes to the SLIT contents should be done in this file.

  Copyright (c) 2013-2015 Intel Corporation.
  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
  All intellectual property rights(Copyright, Patent and Trademark) reserved.

  Any violations of copyright or other intellectual property rights of the Loongson
  Technology Corporation Limited will be held accountable in accordance with the law,
  if you (or any of your subsidiaries, corporate affiliates or agents) initiate directly
  or indirectly any Intellectual Property Assertion or Intellectual Property Litigation:
  (i) against Loongson Technology Corporation Limited or any of its subsidiaries or corporate affiliates,
  (ii) against any party if such Intellectual Property Assertion or Intellectual Property Litigation arises
  in whole or in part from any software, technology, product or service of Loongson Technology Corporation Limited
  or any of its subsidiaries or corporate affiliates, or (iii) against any party relating to the Software.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION).

**/

#ifndef _SLIT_H_
#define _SLIT_H_

#include <IndustryStandard/Acpi30.h>

//
// ACPI table information used to initialize tables.
//

#if (TOT_NODE_NUM == 1)
#define NODE_COUNT   1
#else
#if (TOT_NODE_NUM == 2)
#define NODE_COUNT   2
#elif (TOT_NODE_NUM == 4)
#define NODE_COUNT   4
#endif
#endif


EFI_STATUS
EFIAPI
InstallAcpiSlitTable (
  IN   EFI_ACPI_TABLE_PROTOCOL       *AcpiProtocol
  );

#endif

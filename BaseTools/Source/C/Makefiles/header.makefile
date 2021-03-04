## @file
#
# The makefile can be invoked with
# HOST_ARCH = x86_64 or x64 for EM64T build
# HOST_ARCH = ia32 or IA32 for IA32 build
# HOST_ARCH = ia64 or IA64 for IA64 build
# HOST_ARCH = Arm or ARM for ARM build
# HOST_ARCH = LOONGARCH or LOONGARCH64 for LOONGARCH build
#
# Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
# All intellectual property rights(Copyright, Patent and Trademark) reserved.
# Copyright (c) 2007 - 2018, Intel Corporation. All rights reserved.<BR>

# Any violations of copyright or other intellectual property rights of the Loongson
# Technology Corporation Limited will be held accountable in accordance with the law,
# if you (or any of your subsidiaries, corporate affiliates or agents) initiate directly
# or indirectly any Intellectual Property Assertion or Intellectual Property Litigation:
# (i) against Loongson Technology Corporation Limited or any of its subsidiaries or corporate affiliates,
# (ii) against any party if such Intellectual Property Assertion or Intellectual Property Litigation arises
# in whole or in part from any software, technology, product or service of Loongson Technology Corporation Limited
# or any of its subsidiaries or corporate affiliates, or (iii) against any party relating to the Software.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION).


ifndef HOST_ARCH
  #
  # If HOST_ARCH is not defined, then we use 'uname -m' to attempt
  # try to figure out the appropriate HOST_ARCH.
  #
  uname_m = $(shell uname -m)
  $(info Attempting to detect HOST_ARCH from 'uname -m': $(uname_m))
  ifneq (,$(strip $(filter $(uname_m), x86_64 amd64)))
    HOST_ARCH=X64
  endif
  ifeq ($(patsubst i%86,IA32,$(uname_m)),IA32)
    HOST_ARCH=IA32
  endif
  ifneq (,$(findstring aarch64,$(uname_m)))
    HOST_ARCH=AARCH64
  endif
  ifneq (,$(findstring arm,$(uname_m)))
    HOST_ARCH=ARM
  endif
  ifndef HOST_ARCH
    $(info Could not detected HOST_ARCH from uname results)
    $(error HOST_ARCH is not defined!)
  endif
  $(info Detected HOST_ARCH of $(HOST_ARCH) using uname.)
endif

CYGWIN:=$(findstring CYGWIN, $(shell uname -s))
LINUX:=$(findstring Linux, $(shell uname -s))
DARWIN:=$(findstring Darwin, $(shell uname -s))

BUILD_CC ?= gcc
BUILD_CXX ?= g++
BUILD_AS ?= gcc
BUILD_AR ?= ar
BUILD_LD ?= ld
LINKER ?= $(BUILD_CC)
ifeq ($(HOST_ARCH), IA32)
ARCH_INCLUDE = -I $(MAKEROOT)/Include/Ia32/

else ifeq ($(HOST_ARCH), X64)
ARCH_INCLUDE = -I $(MAKEROOT)/Include/X64/

else ifeq ($(HOST_ARCH), ARM)
ARCH_INCLUDE = -I $(MAKEROOT)/Include/Arm/

else ifeq ($(HOST_ARCH), AARCH64)
ARCH_INCLUDE = -I $(MAKEROOT)/Include/AArch64/

else ifeq ($(HOST_ARCH), LOONGARCH64)
ARCH_INCLUDE = -I $(MAKEROOT)/Include/LoongArch/

else
$(error Bad HOST_ARCH)
endif

INCLUDE = $(TOOL_INCLUDE) -I $(MAKEROOT) -I $(MAKEROOT)/Include/Common -I $(MAKEROOT)/Include/ -I $(MAKEROOT)/Include/IndustryStandard -I $(MAKEROOT)/Common/ -I .. -I . $(ARCH_INCLUDE) 
BUILD_CPPFLAGS = $(INCLUDE) -O2
ifeq ($(DARWIN),Darwin)
# assume clang or clang compatible flags on OS X
BUILD_CFLAGS = -MD -fshort-wchar -fno-strict-aliasing -Wall -Wno-deprecated-declarations -Wno-self-assign -Wno-unused-result -nostdlib -c -g
else
BUILD_CFLAGS = -MD -fshort-wchar -fno-strict-aliasing -Wall -Wno-deprecated-declarations -Wno-unused-result -nostdlib -c -g
endif
BUILD_LFLAGS =
BUILD_CXXFLAGS = -Wno-unused-result

ifeq ($(HOST_ARCH), IA32)
#
# Snow Leopard  is a 32-bit and 64-bit environment. uname -m returns i386, but gcc defaults 
#  to x86_64. So make sure tools match uname -m. You can manual have a 64-bit kernal on Snow Leopard
#  so only do this is uname -m returns i386.
#
ifeq ($(DARWIN),Darwin)
  BUILD_CFLAGS   += -arch i386
  BUILD_CPPFLAGS += -arch i386
  BUILD_LFLAGS   += -arch i386
endif
endif

  
.PHONY: all
.PHONY: install
.PHONY: clean

all:

$(MAKEROOT)/libs:
	mkdir $(MAKEROOT)/libs 

$(MAKEROOT)/bin:
	mkdir $(MAKEROOT)/bin

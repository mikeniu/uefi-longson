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

#!/bin/bash

#######################################################################
#
# 1: make BaseTools and copy BuildEnv.sh build_rule.txt tools_def.txt to
#   Conf directory
# 2: set environment path
# 3: source edksetup.sh BaseTools
# 4: make souce code
# 5: generate *efi file
# 6: generate *fd file
#
#######################################################################
set -x
cd ../../../../
export WORKSPACE=$PWD
export EDK_TOOLS_PATH=$PWD/BaseTools/

COMPILER=loongarch64-linux-gnu-gcc
COMPILER_PATH=`which $COMPILER`
COMPILE_DIR=$PWD/LsRefCodePkg/SampleCode/Desktop/Script
COMPILE_FILE=Loongson
PROJECT_IMAGE_NAME=Loongson.fd
PROJECT_CONFIG_FILE=$COMPILE_DIR/Desktop.cfg
PROJECT_PLATFORM_NAME=LOONGSON
PROJECT_BUILD_DIR=$WORKSPACE/Build/$PROJECT_PLATFORM_NAME/$VERSION
VERSDIR=$WORKSPACE/LsRefCodePkg/Include
CUSTOMED_SEC_DIR=$WORKSPACE/LsRefCodePkg/SampleCode/Desktop/CustomedSec
CUSTOMED_SEC_DIR_S=$WORKSPACE/LsRefCodePkg/Core/SecMain/CustomedSec
COMPILER_VERSION=GCC83

if [ "$1" == "clean" ]
then
  rm -rf $WORKSPACE/LsRefCodePkg/Core/SecMain/CustomedSec
  rm -rf $WORKSPACE/Build/
  rm -rf $COMPILE_DIR/LS*.fd
  exit 0
fi

# Link the CustomedSec to core
rm -rf $WORKSPACE/LsRefCodePkg/Core/SecMain/CustomedSec
ln -s $CUSTOMED_SEC_DIR $CUSTOMED_SEC_DIR_S

# Detect the host processor, and determine the GCC version.
case $('uname') in
  Linux*) echo Linux
    case $(uname -m) in
      x86_64)
         if [[ $COMPILER_VERSION == GCC83 ]];
         then
           export GCC83_LOONGARCH64_PREFIX=`dirname ${COMPILER_PATH}`/${COMPILER%g*}
           GCC_PATH=$GCC83_LOONGARCH64_PREFIX
           echo Use cross compiler version: $COMPILER_VERSION
         elif [[ $COMPILER_VERSION == GCC49 ]];
         then
           export GCC49_LOONGARCH64_PREFIX=`dirname ${COMPILER_PATH}`/${COMPILER%g*}
           GCC_PATH=$GCC49_LOONGARCH64_PREFIX
           echo Use cross compiler version: $COMPILER_VERSION
         else
           echo "Unsupport corss compiler!!"
           exit 1
         fi
         ;;
      mips64)
        GCC_VERSION=$(gcc -v 2>&1 | tail -1 | awk '{print $3}')
        case $GCC_VERSION in
          4.4.*)
            export GCC44_LOONGARCH64_PREFIX=/usr/bin/
            GCC_PATH=$GCC83_LOONGARCH64_PREFIX
            COMPILER_VERSION=GCC83
            echo Use compiler version: $COMPILER_VERSION
            ;;
          4.9.*)
            export GCC49_LOONGARCH64_PREFIX=/usr/bin/
            GCC_PATH=$GCC49_LOONGARCH64_PREFIX
            COMPILER_VERSION=GCC49
            echo Use compiler version: $COMPILER_VERSION
            ;;
          *)
            echo Only support the version of compiler is GCC4.4 and GCC4.9 for now.
            ;;
        esac
        ;;
    esac
    ;;
  *)
    echo Only support Linux.
    exit 1
    ;;
esac

# check build version and show the config info
grep -v "^[[:blank:]]*#" $PROJECT_CONFIG_FILE | grep -v "^$" > ./.cfg
sed -i "s/DEFINE//g" ./.cfg

CPUTYPE=`grep  "CPU_TYPE" ./.cfg|sed 's/^.*= *//g'`
NORTHBRIDGE=`grep  "SOUTH_BRIDGE" ./.cfg|sed 's/^.*= *//g'`
PLATFORMTYPE=`grep  "PLATFORM_TYPE" ./.cfg|sed 's/^.*= *//g'`

echo "#####################################"
echo "Compile configure is:"
while read LINE
do
 echo $LINE
done < ./.cfg
echo "#####################################"
STR=$(sed -n '/COMP_TYPE/'p ./.cfg; rm .cfg -rf)
VERSION=${STR##*=}
VERSION=$(echo $VERSION |sed 's/[[:space:]]//g')

echo WORKSPACE:${WORKSPACE}
echo EDK_TOOLS_PATH:${EDK_TOOLS_PATH}
echo GCC_LOONGARCH64_PREFIX:${GCC_PATH}
source edksetup.sh BaseTools
echo PROJECT_BUILD_DIR:${PROJECT_BUILD_DIR}

# Generate version
#
if [[ ${CPUTYPE} == 3A5000 || ${CPUTYPE} == 3B5000 ]]
then
PLATFORM_NAME=Ls3a50007a
fi

PLATFORM=${CPUTYPE}${NORTHBRIDGE}"_"${PLATFORMTYPE}
echo "PLATFORM = $PLATFORM"
$COMPILE_DIR/Vers.sh $PLATFORM $VERSDIR

#
# Build By GenFv
#
if  [[ ! -f `which build` || ! -f `which GenFv` ]];
then
  echo Make BaseTools....
  make -C $WORKSPACE/BaseTools
elif [[ ( -f `which build` ||  -f `which GenFv` )  && ! -d  $EDK_TOOLS_PATH/Source/C/bin ]];
then
  echo Building tools no $EDK_TOOLS_PATH/Source/C/bin directory
  make -C $WORKSPACE/BaseTools
else
  echo Tools already exiting
fi


if [ -z "$PLATFORMFILE" ]
then
  PLATFORMFILE=${COMPILE_DIR}/${COMPILE_FILE}.dsc
fi

set -e

echo Runing Edk2 Build for Loongson...
build -a LOONGARCH64 -p  $PLATFORMFILE -t $COMPILER_VERSION -b $VERSION -j log.txt

if [ -z "$OUTPUT_DIR" ]
then
MRC_OUT_DIR=`find $WORKSPACE/Build -name MemConfigPpi`
if [ -f "$MRC_OUT_DIR/OUTPUT/MemConfigPpi.efi" ]
then
MRC_BIN_DIR=$WORKSPACE/LsRefCodePkg/MRC/Binary
cp $MRC_OUT_DIR/OUTPUT/MemConfigPpi.efi $MRC_BIN_DIR/
cp $MRC_OUT_DIR/OUTPUT/MemConfigPpi.depex $MRC_BIN_DIR/
fi
OUTPUT_DIR=$WORKSPACE/Build/$PLATFORM_NAME/${VERSION}_${COMPILER_VERSION}/FV
cp $OUTPUT_DIR/LS*.fd $COMPILE_DIR
fi

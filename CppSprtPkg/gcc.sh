#!/bin/bash
iscpp=0
for i in "$@" ;do
  if [ "-" != "${i:0:1}" ]; then
    if [ "cpp" = "${i##*.}" ]; then
      iscpp=1
    fi
  fi
done
if [ $iscpp = 0 ]; then
  /opt/gcc-4.4.7-7215-n64-loongson/usr/bin/mips64el-redhat-linux-gcc $@
else
  /opt/gcc-4.4.7-7215-n64-loongson/usr/bin/mips64el-redhat-linux-g++ -fno-rtti $@
fi

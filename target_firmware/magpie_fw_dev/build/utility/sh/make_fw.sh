#!/bin/sh
# Copyright (c) 2008 Atheros Communications Inc.
# All rights reserved.
# $ATH_LICENSE_TARGET_MK$

# This script constructs an AR6002 ROM firmware binary image
# for FPGA(if FPGA_FLAG is "1") or for actual silicon if FPGA_FLAG
# is not "1".
#
# It takes two things as input:
#   sw.rom.out, which is an ELF image of ROM software
#        that was built for actual hardware or for FPGA
#   data.rom.hw.bin OR data.rom.fpga.bin, which contains
#        DataSets and other data that is to be included
#        at the end of the firmware image
#
# It produces fw.rom.bin, a firmware image appropriate for
# use with AR6002 ROM.
#
# The image is constructed by converting the sw.rom.out ELF
# image to binary, padding at the end of that binary,
# and appending the data binary from data.rom.*.bin.  So the
# final fw.rom.bin image is a binary image that's exactly
# the same size as the ROM hardware and which contains ROM
# software (including literals, read-only data, data that
# will be copied to RAM in order to initialize read/write data),
# DataSet Metadata, and DataSets.


sw_image=${sw_image:-$PRJ_ROOT/build/image/magpie/rom.out}
sw_bin=${sw_bin:-$PRJ_ROOT/build/image/magpie/rom.bin}


ds_in_rom="0x`xt-nm $sw_image | grep " _data_start_in_rom" | cut -b -8`"

# Place data binary at the end of ROM.
#data_start=$((9168*1024-`stat --format='%s' $data_bin`))

xt-objcopy  \
  --change-section-lma .lit4+0x400000 \
  --change-section-vma .lit4+0x400000 \
  --change-section-lma .rodata+0x400000 \
  --change-section-vma .rodata+0x400000 \
  --change-section-lma .dram0.literal+0x400000 \
  --change-section-vma .dram0.literal+0x400000 \
  --remove-section     .dport0.data \
  --change-section-lma .data=$((ds_in_rom)) \
  --change-section-vma .data=$((ds_in_rom)) \
  -O binary $sw_image $sw_bin

#cat $sw_bin $data_bin > $fw_bin

#if [ `stat --format='%s' $fw_bin` -ne $((96*1024)) ]
#then
#  echo "$0 ERROR: firmware $fw_bin is NOT the expected size."
#fi

exit 0

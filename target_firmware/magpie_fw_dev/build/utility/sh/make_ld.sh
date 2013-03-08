#!/bin/sh

# This script is intended for use with separately linked ROM and RAM.
# It creates EITHER a linker script that satisfies references made by RAM
# applications to ROM symbols OR a linker script that forces symbols to
# be included in a ROM image.
#
# The option "--addrs" causes the RAM linkage script to be generated
# and the option "--externs" causes the ROM linkage script to be generated.
#
# Example usage:
#     make_ld.sh --addrs athos.rom.out athos.rom.symbols > rom.addrs.ld
#     make_ld.sh --externs athos.rom.symbols > rom.externs.ld

eval XTNM=xt-nm

Usage() {
    echo Usage:
    echo $progname '{--addrs ROM_ELF_Image | --externs} symbol_file'
}

Provide() {
    addr0=`echo $1 | sed 's/$//'`
	addr=0x`nm $image_file | grep -w $addr0 | cut -d ' ' -f 1`
    if [ "$addr" != "0x" ]
    then
    	echo PROVIDE \( $addr0 = $addr \)\;
    fi
}

Extern() {
    echo EXTERN \( $1 \)\;
}

progname=$0
script_choice=$1

if [ "$script_choice"=="--addrs" ]
then
    action=Provide
    image_file=$2

    if [ ! -r "$image_file" ]
    then
        echo "Cannot read ELF image: $image_file"
        Usage
    fi
    symbol_file=$3
elif [ "$script_choice"=="--externs" ]
then
    action=Extern
    symbol_file=$2

	if [ ! -r "$symbol_file" ]
	then
	    echo "Cannot read symbol list from: $symbol_file"
	    Usage
	fi
else
    Usage
fi

for i in `cat $symbol_file`
do
    $action $i
done

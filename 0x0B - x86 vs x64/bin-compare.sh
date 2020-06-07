#!/bin/bash

if [ $# -ne 2 ];  then
    echo "Usage: ./bin-compare.sh [source file] [function]"
    exit 1;
fi

TMP_FOLDER=/tmp/bin-compare

SOURCE=$1
FUNCTION=$2

CC=gcc
ARCH_32=-m32
ARCH_64=-m64
CFLAGS="-O0 -ggdb3 --std=c99 -Wall -Wextra -pedantic"

function require_package()
{
    dpkg -s $1 &> /dev/null
    if [ $? -ne 0 ]; then
        echo "Package '$1' is q requirement."
        exit 1
    fi
}

function compile()
{
    mkdir -p $TMP_FOLDER
    cp "$1" "$TMP_FOLDER/$1"
    $CC $CFLAGS $2 "$TMP_FOLDER/$1" -o "$TMP_FOLDER/$1-$3.out"

    if [ $? -ne 0 ]; then
        echo "Compilation failed."
        exit 1
    fi
}

function decompile()
{
    gdb -batch \
        -ex "set disassembly-flavor intel" \
        -ex "disassemble/rs $3" \
        "$TMP_FOLDER/$1-$2.out" > "$TMP_FOLDER/$1-$2.asm"

    if [ $? -ne 0 ]; then
        echo "Decompilation failed."
        exit 1
    fi
}

require_package "libc6-dev-i386"

echo "Compiling $SOURCE (x86)"
compile $SOURCE $ARCH_32 "x86"

echo "Compiling $SOURCE (x64)"
compile $SOURCE $ARCH_64 "x64"

echo "Decompiling $SOURCE (x86)"
decompile $SOURCE "x86" $FUNCTION

echo "Decompiling $SOURCE (x64)"
decompile $SOURCE "x64" $FUNCTION

vim -d $TMP_FOLDER/*.asm

rm -rf $TMP_FOLDER/*.asm
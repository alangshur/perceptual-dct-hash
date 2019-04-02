#!/bin/bash
SRC_PATH="./src"
SRC_EXT=".cpp"
MOD_EXT=".o"
MODULES=""

# cut src tokens
for token in $SRC_PATH/*$SRC_EXT; do
    TEMP=$(basename ${token} | cut -f 1 -d '.')
    TEMP+=$MOD_EXT
    MODULES+=" "
    MODULES+=$TEMP
done

# print module tokens
echo $MODULES


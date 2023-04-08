#!/bin/bash
# SPDX-License-Identifier: GPL-2.0

die () {
    echo >&2 "$@"
    exit 1
}

[ "$#" -eq 1 ] || die "Usage convert.sh <filename.png>"

#python3 img2bin.py $1 image.h
new_name=$(echo "$1" | cut -f 1 -d '.')
python3 img2bin.py $1 $new_name

#!/bin/bash

# read the fields into the array `v`:
bedfile=$1
hgfile=$2
while read -ra v
do
    # print the 7 first elements in the array + the output from `grep -c`:
    echo "${v[@]:0:7}" "$(grep -Fc "${v[6]}" "$hgfile")"
done < $bedfile

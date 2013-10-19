#!/opt/local/bin/bash
##!/bin/bash

echo "Bash version: ${BASH_VERSION}..."

unset mshfile
mshfile="$(pwd)/$1"

if [ -f $mshfile ]; then
    echo "file: $mshfile exists, proceed to solve..."
    unset Nd
    #for Nd in 10 20 30 40 50 60 70 80 90 100 110 120 130 140 150 160 170 180 190 200
    for Nd in {10..120..10}
    do
      echo "solving Nd=$Nd,phis=$2..."
      ./batchsolve.m $1 $Nd $2
    done
    unset Nd
else
    echo "file: $mshfile does not exist, abort..."
    exit
fi 

unset mshfile 

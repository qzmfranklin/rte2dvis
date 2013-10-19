#!/bin/bash

unset phis
phis=0
echo "Bash version: ${BASH_VERSION}..."

for Ns in 4 26 42 90 162 242 346 518 614 782 948 1126 1384
do
	for Nd in 10 20 30 40 50 60 70 80 90 100 110 120
	do
		  ./fix-niter-bug.m $Ns $Nd $phis
	  done
  done

unset phis

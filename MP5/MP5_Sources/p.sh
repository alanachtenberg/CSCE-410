#!/bin/bash
echo "Building -> Running "
make -f makefile clean &&\
make -f  makefile -j 4  &&\
./copykernel.sh                &&\
bochs -qf bochsrc.txt

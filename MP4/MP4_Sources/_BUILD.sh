#!/bin/bash
echo "Building -> Running "
make -f makefile.linux64 clean &&\
make -f makefile.linux64 -j 4  &&\
./copykernel.sh                &&\
bochs -qf bochsrc.bxrc

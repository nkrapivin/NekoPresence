#!/bin/sh
rm libnekopresence-32bit.so
c++ -m32 -std=c++11 -fPIC -g -c -o np.o NekoPresence/nekopresence.cpp
c++ -m32 -std=c++11 np.o -shared -o libnekopresence-32bit.so -L. -ldiscord-rpc-32bit
rm np.o

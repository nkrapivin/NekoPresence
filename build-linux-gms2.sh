#!/bin/sh
rm libnekopresence.so
c++ -fPIC -g -c -o np.o NekoPresence/nekopresence.cpp
c++ np.o -shared -o libnekopresence.so -L. -ldiscord-rpc
rm np.o
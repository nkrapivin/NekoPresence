#!/bin/sh
rm libnekopresence.so
g++ -fPIC -g -c -o np.o NekoPresence/nekopresence.cpp
g++ np.o -shared -o libnekopresence.so -L. -ldiscord-rpc
rm np.o
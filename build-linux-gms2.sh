#!/bin/sh
g++ -fPIC -g -c -o np.o NekoPresence/nekopresence-lin.cpp
g++ np.o -shared -o libnekopresence.so -Wl,-rpath,assets/ -L. -ldiscord-rpc
rm np.o
#!/bin/sh
echo 'Make sure you replace libdiscord-rpc.so with a 32bit version!'
g++ -m32 -fPIC -g -c -o np.o NekoPresence/nekopresence-lin.cpp
g++ -m32 np.o -shared -o libnekopresence.so -Wl,-rpath,assets/ -L. -ldiscord-rpc
rm np.o
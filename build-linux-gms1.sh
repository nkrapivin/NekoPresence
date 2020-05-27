#!/bin/sh
rm libnekopresence-32bit.so
mv libdiscord-rpc.so temp.so
mv libdiscord-rpc-32bit.so libdiscord-rpc.so
g++ -m32 -fPIC -g -c -o np.o NekoPresence/nekopresence-lin.cpp
g++ -m32 np.o -shared -o libnekopresence-32bit.so -Wl,-rpath,assets/ -L. -ldiscord-rpc
mv libdiscord-rpc.so libdiscord-rpc-32bit.so
mv temp.so libdiscord-rpc.so
rm np.o
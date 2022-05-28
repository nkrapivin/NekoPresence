# NekoPresence, nya~

![nekopresencenyalogo](https://user-images.githubusercontent.com/33228822/83038805-d33f6400-a056-11ea-86bf-c37cea78f41b.png)

An open source, and neko (!) alternative to rousr's Dissonance.

## Installing and using NekoPresence
Simply import extension package ( link here: https://github.com/nkrapivin/NekoPresence/releases/latest ) into your project and then refer to NekoPresence wiki for more information https://github.com/nkrapivin/NekoPresence/wiki

## Building NekoPresence
I provide pre-compiled versions of NekoPresence for Windows and 32/64-bit Linux (tested on Ubuntu).

But in some rare cases you may want to build this extension yourself.

#### Windows
1. Open `NekoPresence.sln` in Visual Studio 2019.
2. Build it.
3. Replace `NekoPresence.dll` in extension files with your new DLL.

#### Linux (64-Bit, Debian-based, GMS 2 only)
1. Make sure you have `build-essential` package installed.
2. Execute `./build-linux-gms2.sh` in the repo's directory.
3. Replace `libnekopresence.so` in extension files with your new library.

#### Linux (32-Bit, Debian-based, GM:S 1.4 only)
I did not test the 32-bit version at all, because GM:S 1.4 is dying and I don't have a 32-bit WSL to test that.

Anyway...
1. Make sure you have `gcc-multilib` and `g++-multilib` installed if you're on a 64-Bit system.
2. Execute `./build-linux-gms1.sh` in repo's directory.
3. Copy `libnekopresence-32bit.so` as `libnekopresence.so` to GM:S 1.4 extension files.

#### macOS (64-Bit Only)
<s>
1. Make sure you have Xcode and an Apple Dev account.
2. Create an Xcode Library project, framework MUST BE `STL (C++ library)`.
3. Add CoreServices, CoreFoundation and AppKit frameworks into your project.
4. Add the `libdiscord-rpc-macOS.a` file as a "framework" too.
5. Add `NekoPresence.cpp`, `discord_register.h`, `discord_rpc.h` as source files into the project.
6. Make sure that for frameworks you select `Do Not Embed`, so that the dylib will load them from macOS.
7. Setup any code signing you might need.
8. Build the library and copy out the dylib.
9. Add/Replace it in the project folder.
</s>

Disregard all of this, good luck.

## Credits
[me](https://twitter.com/nkrapivindev/) - Making this awesome thing.

[AceKiron](https://twitter.com/AceKiron/) - Added demo project to the source code and added a 5 second timer to see if Discord is even running.

[YellowAfterlife](https://yal.cc/) - cool awesome amazing beautiful wonderful adorable cute logo

[TabularElf](https://twitter.com/TabularElf) - apple code signing. (fuck apple btw)

nya~

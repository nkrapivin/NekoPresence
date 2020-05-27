# NekoPresence, nya~

![nekopresencenyalogo](https://user-images.githubusercontent.com/33228822/83038805-d33f6400-a056-11ea-86bf-c37cea78f41b.png)

An open source, and neko(!) alternative to rousr's Dissonance.

## Installing and using NekoPresence
Simply import extension package into your project and then refer to NekoPresence wiki for more information https://github.com/nkrapivin/NekoPresence/wiki

## Building NekoPresence
I provide pre-compiled versions of NekoPresence for Windows and 64-bit Linux for GameMaker Studio 2.

But in some rare cases you may want to build this extension yourself.

#### Windows
1. Open `NekoPresence.sln` in Visual Studio 2019.
2. Build it.
3. Replace `Release\NekoPresence.dll` in extension files with your new DLL.
PS: (Build directory of the project is set to `Release\`, DLL in root of this repo won't be replaced! It's for people who just want latest prebuilt libs.)

#### Linux (64-Bit, Debian-based)
1. Make sure you have `build-essential` package installed.
2. Execute `./build-linux-gms2.sh` in the repo's directory.
3. Replace `libnekopresence.so` in extension files with your new library.

#### Linux (32-Bit, Debian-based, GM:S 1.4 only)
I did not test the 32-bit version at all, because GM:S 1.4 is dying and I don't have a 32-bit WSL to test that.

But anyway...
1. Make sure you have `gcc-multilib` and `g++-multilib` installed if you're on a 64-Bit system.
2. Execute `./build-linux-gms1.sh` in repo's directory.
3. Copy `libdiscord-rpc-32bit.so` and `libnekopresence-32bit.so` to some directory.
4. Rename them to `libdiscord-rpc.so` and `libnekopresence.so`.
5. Replace these files in your extension files.

#### macOS (64-Bit Only)
Coming soon... I hope... I *will* make a macOS version eventually...

## Credits
[me](https://twitter.com/nkrapivindev/) - Making this awesome thing.

[YellowAfterlife](https://yal.cc/) - cool awesome amazing beautiful wonderful adorable cute logo

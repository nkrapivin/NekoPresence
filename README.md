# NekoPresence, nya~
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

#### Linux (32-Bit, Debian-based)
I did not test the 32-bit version at all (thus it's not included in the Extension Package), because GM:S 1.4 is dying and I don't have a 32-bit WSL to test that.

But anyway...
1. Replace `libdiscord-rpc.so` in repo's directory with a 32-Bit version of this library. (you can build it yourself https://github.com/discord/discord-rpc)
2. (if you're on a 64-Bit system make sure you have multiarch `g++` installed!)
2. Execute `./build-linux-gms1.sh` in repo's directory.
3. Replace `libnekopresence.so` in extension files with your new library.

#### macOS (64-Bit Only)
Coming soon... I hope... I *will* make a macOS version eventually...

## Credits
[me](https://twitter.com/nkrapivindev/) - Making this awesome thing.

[YellowAfterlife](https://yal.cc/) - cool awesome amazing beautiful wonderful adorable cute logo

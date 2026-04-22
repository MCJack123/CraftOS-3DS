# CraftOS-3DS
A ComputerCraft emulator for 3DS. Based on [craftos-base](https://github.com/MCJack123/craftos-base).

## Building
Requires devkitPro with 3DS support (devkitARM), and a shell with Python.

Clone the entire repo, including submodules (`git clone --recurse-submodules`/`git submodule update --init --recursive`). Then run `catnip -T 3DS` from devkitPro in the repo root. The output 3DSX will be in `build/main.release`.

## License
MIT

Includes code from [HBKB](https://gbatemp.net/threads/hbkblib-a-3ds-keyboard-library.397568/), which has no license but I'm not worried about retribution for using it.

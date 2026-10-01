# CS202_Final_Project
Parody of Legend of Zelda: A Link to the Past.

Built from scratch using C++ and the SDL2 library (SDL2, SDL2_image, SDL2_ttf, SDL2_mixer).

All source code and assets live in `src/`.

## Building and running

The game loads `images/`, `sound/`, `maps/` and `fonts/` by relative path, so it
must be run from inside the `src` directory.

### Command line (Linux or macOS)

Install the SDL2 libraries:

```
# Linux (Debian/Ubuntu)
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev

# macOS (Homebrew)
brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer
```

Then build and run:

```
cd src
g++ -std=c++11 main.cpp -o game $(pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer)
./game
```

### Windows 11

**Option 1: WSL2 (easiest).** Windows 11 runs Linux GUI apps and audio through WSLg,
so the Linux instructions above work unchanged. In PowerShell run `wsl --install`,
reboot, open Ubuntu, then:

```
sudo apt update
sudo apt install g++ git pkg-config libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
git clone https://github.com/j-guerrero/CS202_Final_Project
cd CS202_Final_Project/src
g++ -std=c++11 main.cpp -o game $(pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer)
./game
```

**Option 2: MSYS2 / MinGW (native `.exe`).** Install [MSYS2](https://www.msys2.org), open
the "MSYS2 UCRT64" shell, and run:

```
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-pkgconf \
  mingw-w64-ucrt-x86_64-SDL2 mingw-w64-ucrt-x86_64-SDL2_image \
  mingw-w64-ucrt-x86_64-SDL2_ttf mingw-w64-ucrt-x86_64-SDL2_mixer
cd "/path/to/CS202_Final_Project/src"
g++ -std=c++11 main.cpp -o game.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
./game.exe
```

To launch the `.exe` from Explorer instead, copy the SDL DLLs from
`C:\msys64\ucrt64\bin` next to it (and keep it inside `src`).

**Option 3: Visual Studio.** Create an empty C++ console project containing `main.cpp`,
install the libraries with `vcpkg install sdl2 sdl2-image sdl2-ttf sdl2-mixer` and
`vcpkg integrate install`, and set the debugging working directory to `src`.
This has not been tested; MSVC may flag things GCC accepts.

The code is verified to cross-compile and link for 64-bit Windows with MinGW-w64, but
it has not been run on a real Windows machine.

### Xcode (macOS)

1. Install the SDL2, SDL2_image, SDL2_ttf and SDL2_mixer frameworks from
   [libsdl.org](https://libsdl.org) into `/Library/Frameworks`.
2. Open `GameTest.xcodeproj`.
3. In Product > Scheme > Edit Scheme > Run > Options, set the working directory
   to the `src` folder (the saved path points at the original author's machine).
4. Build and run.

## Releases

Pre-built Windows (`game-windows-x64.zip`) and Linux (`game-linux-x64.tar.gz`) packages are
built by GitHub Actions (`.github/workflows/release.yml`) and attached to each GitHub Release.
To publish one, push a version tag from `master`:

```
git tag v1.0
git push origin v1.0
```

The workflow can also be run manually from the Actions tab (it uploads the zips as
workflow artifacts without creating a Release). Run the game from inside the extracted
folder; the Linux package needs the SDL2 runtime libraries installed (see its `README.txt`).

## Notes

- `images/pointer.bmp` is referenced in `menu.h` but is not included in the repository.

# CS202_Final_Project
Parody of Legend of Zelda: A Link to the Past.

Built from scratch using C++ and the SDL2 library (SDL2, SDL2_image, SDL2_ttf, SDL2_mixer).

> **Update (2026):** This 2015 project was a class group project (CS202) made while studying at
> UAF. It has been updated to build and run correctly on current systems (modern SDL2 libraries,
> Windows 11, and Linux).

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

**Option 2: Visual Studio (native `.exe`).** The project is in `vs/` (`vs/vs-app.slnx`, built with
the Visual Studio 2026 / v145 toolset; retarget the platform toolset in the project properties for
older versions).

1. Install Visual Studio with the "Desktop development with C++" workload and set up vcpkg
   (`vcpkg integrate install`; the copy bundled with Visual Studio works).
2. Install the libraries. Debug uses the DLL versions and Release links them statically:

   ```
   vcpkg install sdl2:x64-windows sdl2-image:x64-windows sdl2-ttf:x64-windows sdl2-mixer:x64-windows
   vcpkg install sdl2:x64-windows-static sdl2-image:x64-windows-static sdl2-ttf:x64-windows-static sdl2-mixer:x64-windows-static
   ```
3. Open `vs/vs-app.slnx`, choose **x64** and Debug or Release, and build/run. Only x64 is set up;
   the x86 configurations will not build.

The project already defines `_USE_MATH_DEFINES` and `SDL_MAIN_HANDLED`. Debug builds read the
assets from `src/` (the debugger's working directory) and copy them next to the `.exe` in
`vs/build/x64/Debug/`, with vcpkg copying the SDL DLLs.

#### Publishing a single-file executable with Visual Studio

A **Release** build produces one self-contained `.exe`: SDL is linked statically and the images,
music, sound effects, font and maps are embedded in it as Windows resources, so there are no
DLLs or asset folders to ship.

1. Open `vs/vs-app.slnx`.
2. In the toolbar set the configuration to **Release** and the platform to **x64**.
3. Build with Build > Rebuild Solution (Ctrl+Shift+B). The first build is slower because the
   assets are compiled into the executable.
4. Take `vs/publish/VIDEO GAME THE MOVIE THE GAME 3 (Windows x64).exe`. It is about 12 MB. (This is
   the window title without its colons, which Windows filenames cannot contain, plus the platform.)
5. To check it, copy it to an empty folder and run it there, away from the project folders.
   Running it from Visual Studio does not prove the assets are embedded, because Visual Studio
   starts it in `src/`, where the files exist on disk. The console prints
   `Embedded assets found: 21 of 21` at startup when they are embedded.
6. To share it, send the `.exe`. It runs from anywhere.

If something goes wrong:

- **Linker errors about missing SDL libraries or `unresolved external` symbols:** Release needs the
  `x64-windows-static` libraries from step 2. In Project > Properties > vcpkg, check that
  "Use Static Libraries" is Yes for Release|x64.
- **A DLL is still needed to run the Release `.exe`:** the static libraries were not picked up, so
  the build fell back to the DLL versions. Check the same "Use Static Libraries" setting.
- **The game starts but shows no images or plays no sound in Release** (the console says
  `Embedded assets found: 0 of 21`, or a gray screen with "Couldn't open images/..." errors): the
  assets did not get embedded. Check that `src/assets.rc` is in the project (Resource Files) and
  not excluded for Release|x64, and that the build output shows `assets.rc` being compiled.
  The console also prints the executable's folder; if the assets are not embedded, they are loaded
  from the working directory or from next to the `.exe`.
- **The build fails on the resource script:** every file listed in `src/assets.rc` must exist.

If you add or rename an asset file, add or change it in both `src/assets.rc` (a plain
`id RCDATA "path"` line) and `src/assets_list.h`, using the same id and path. Debug and
non-Windows builds read the files from disk and need no entry. The startup line
`Embedded assets found: N of M` shows if the two lists ever disagree.

The game has been run on Windows 11 under WSL2, and built, run and published as a single-file
executable with Visual Studio.

### Xcode (macOS)

1. Install the SDL2, SDL2_image, SDL2_ttf and SDL2_mixer frameworks from
   [libsdl.org](https://libsdl.org) into `/Library/Frameworks`.
2. Open `GameTest.xcodeproj`.
3. In Product > Scheme > Edit Scheme > Run > Options, set the working directory
   to the `src` folder (the saved path points at the original author's machine).
4. Build and run.

## Notes

- `images/pointer.bmp` is referenced in `menu.h` but is not included in the repository.

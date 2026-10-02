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

**Option 2: Visual Studio (native `.exe`).** The project is in `vs/` (`vs/vs-app.slnx`, built with
the Visual Studio 2026 / v145 toolset; retarget the platform toolset in the project properties for
older versions).

1. Install Visual Studio with the "Desktop development with C++" workload and set up vcpkg
   (`vcpkg integrate install`; the copy bundled with Visual Studio works).
2. Install the libraries: `vcpkg install sdl2:x64-windows sdl2-image:x64-windows sdl2-ttf:x64-windows sdl2-mixer:x64-windows`
3. Open `vs/vs-app.slnx`, choose **x64** and Debug or Release, and build/run. Only x64 is set up;
   the x86 configurations will not build.

The project already defines `_USE_MATH_DEFINES` and `SDL_MAIN_HANDLED`, and debugs with `src/` as
the working directory so the game finds its assets. Build output goes to `vs/build/x64/<Config>/`
with the assets copied next to the `.exe` (vcpkg copies the SDL DLLs).

#### Publishing an executable with Visual Studio

A Release build assembles a folder you can share:

1. Open `vs/vs-app.slnx`.
2. In the toolbar set the configuration to **Release** and the platform to **x64**.
3. Build with Build > Rebuild Solution (Ctrl+Shift+B). The first build is slower because the
   sound files are copied.
4. Take the result from `vs/publish/`. It contains `vs-app.exe`, the SDL DLLs and the `images`,
   `sound`, `maps` and `fonts` folders.
5. To check it, run `vs/publish/vs-app.exe` (double-click it) outside of Visual Studio.
6. To share it, zip the whole `publish` folder. Whoever receives it unzips it and runs
   `vs-app.exe` from inside the folder, since the game loads its assets by relative path.

If something is missing from `vs/publish/`:

- **No DLLs:** vcpkg copies them into `vs/build/x64/Release/` first, and the publish step copies
  from there. Make sure `vcpkg integrate install` has been run and that Project > Properties >
  vcpkg > "Use AppLocal Deps" is Yes.
- **The build fails on an `xcopy` line:** the post-build commands are in the project's
  Properties > Build Events > Post-Build Event. Check the paths there and the Output window
  for the exact message.
- **The game starts but shows no images or sound:** the asset folders are not next to the
  `.exe`. Run it from inside `vs/publish/`.

The game has been run on Windows 11 under WSL2 and built, run and published with Visual Studio.

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

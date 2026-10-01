# CS202_Final_Project
Parody of Legend of Zelda: A Link to the Past.

Built from scratch using C++ and the SDL2 library (SDL2, SDL2_image, SDL2_ttf, SDL2_mixer).

All source code and assets live in `Final 1.0/`.

## Building and running

The game loads `images/`, `sound/`, `maps/` and `fonts/` by relative path, so it
must be run from inside the `Final 1.0` directory.

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
cd "Final 1.0"
g++ -std=c++11 main.cpp -o game $(pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer)
./game
```

### Xcode (macOS)

1. Install the SDL2, SDL2_image, SDL2_ttf and SDL2_mixer frameworks from
   [libsdl.org](https://libsdl.org) into `/Library/Frameworks`.
2. Open `GameTest.xcodeproj`.
3. In Product > Scheme > Edit Scheme > Run > Options, set the working directory
   to the `Final 1.0` folder (the saved path points at the original author's machine).
4. Build and run.

## Notes

- `images/pointer.bmp` is referenced in `menu.h` but is not included in the repository.

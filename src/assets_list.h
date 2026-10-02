// Every asset the game loads, as ASSET(id, "path relative to src/").
// This list feeds the lookup table in assets_win.cpp. assets.rc has to list the same
// ids and paths as plain `id RCDATA "path"` lines (rc.exe cannot use this macro list),
// so when you add a file, add it in both places.
// Maps/mobs/Mobs.mobs is empty and is deliberately left out: a missing file is
// read the same as an empty one.
ASSET(1001, "images/tilesheet.png")
ASSET(1002, "images/health.png")
ASSET(1003, "images/teamLogo.png")
ASSET(1004, "images/sheet1.png")
ASSET(1005, "images/sheet2.png")
ASSET(1006, "images/maryjane.png")
ASSET(1007, "images/theBoss.png")
ASSET(1008, "images/bullet.png")
ASSET(1101, "sound/WWtheme.ogg")
ASSET(1102, "sound/overworld.ogg")
ASSET(1103, "sound/snakeEater.ogg")
ASSET(1104, "sound/sabotage.ogg")
ASSET(1105, "sound/gun.wav")
ASSET(1106, "sound/boop.wav")
ASSET(1107, "sound/death.wav")
ASSET(1201, "fonts/RetGanon.ttf")
ASSET(1301, "maps/1.map")
ASSET(1302, "maps/2.map")
ASSET(1303, "maps/disco.map")
ASSET(1401, "maps/mobs/Mobs1.mobs")
ASSET(1402, "maps/mobs/Mobs2.mobs")

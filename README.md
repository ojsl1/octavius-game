# Slot game
Simple slot machine game based on SDL2.
Render implemented on OpenGL.

Screenshots:
----------
![Slotgame](https://germanaizek.github.io/screens/octavius.png "Slotgame")

Requirements
----------
  - CMake
  - Any compiler for CMake
  - libsdl2-dev, libsdl2-ttf-dev, libsdl2-image-dev (only Linux), freeglut3-dev (if not in system Linux)

Build on Linux
----------
       * sudo apt install cmake libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev freeglut3-dev g++
       * git clone https://github.com/GermanAizek/octavius-game.git
       * cd octavius-game
       * mkdir build
       * cd build
       * cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
       * make -j8
       * cp -rv ../res/gamedata .
       * ./octavius
       * Profit!

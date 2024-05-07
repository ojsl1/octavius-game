Screenshots:
----------
![Slotgame](https://germanaizek.github.io/screens/octavius.png "Slotgame")

Requirements
----------
    - cmake
    - linux: libsdl2-dev, libsdl2-ttf-dev, libsdl2-image-dev
    - linux: freeglut3-dev (if not in system Linux)

Build on Linux
----------
    * sudo apt install cmake libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev freeglut3-dev g++
    * git clone https://github.com/ojsl1/octavius-new.git
    * sudo chown -Rv $USER:USER octavius-new
    * sh -c "cd $_ && mkdir build"
    * sh -c "cd /opt/octavius-new/build && rm -rf /opt/octavius-new/build/* && cmake /opt/octavius-new/ -G 'Unix Makefiles' -DCMAKE_BUILD_TYPE=Debug && make -j2 && cp -rv /opt/octavius-new/res/gamedata /opt/octavius-new/build"

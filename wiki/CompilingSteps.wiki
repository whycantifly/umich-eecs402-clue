qmake -project (only if you added new files, not necessary otherwise)

qmake (only if you added new dependencies)

make

If you want to be able to debug, after the qmake -project command, open the .pro file in the directory and add the line "CONFIG += debug" after # Input with, then do qmake and make.
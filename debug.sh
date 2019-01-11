#! /usr/bin/env bash

SD=`pwd`
SD=${SD##/*/}
EN="SFML-$SD"

# sfml-system.pc
# sfml-all.pc
# sfml-graphics.pc
# sfml-window.pc
# sfml-audio.pc
# sfml-network.pc

clang `pkg-config --libs --cflags sfml-window sfml-graphics gl glu` -g -std=c++2a -lstdc++ -lm *.cpp -o "$EN"

if [[ $? -eq 0 ]]
    then
    nemiver ./$EN
    fi

echo "Executable Name: $EN"

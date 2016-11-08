#!/usr/bin/env bash

if [ $# -ne 3 ]; then
   usage;
fi

particle $1 $2 apps/$3

if [ $1 == "compile" ]; then
    rm -rf ./build/$2/$3
    mkdir -p ./build/$2/$3
    mv *.bin ./build/$2/$3/firmware.bin
fi

function usage {
   cat << EOF
Usage: particle-pozyx.sh <compile | flash> <platform | device_id > <app>

Compiles or flashes a Particle device with the specified app.
EOF
   exit 1
}

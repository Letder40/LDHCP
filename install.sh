#!/bin/bash

function make_ldhcp() {
   if [[ ! -e "./build" ]]; then
      mkdir build
      echo "[#] Created build directory"
      cd build
      cmake ..
      echo "[#] LDHCP successfully compiled"
      make
      echo "[#] binary: $(pwd)/ldhcp" 
      cd ..
   else
      cd build
      make
      echo "[#] LDHCP successfully compiled"
      echo "[#] binary: $(pwd)/ldhcp" 
      cd ..
   fi
}

make_ldhcp

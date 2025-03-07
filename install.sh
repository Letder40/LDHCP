#!/bin/bash

function make_ldhcp() {
    cmake -B build
    make -C build
}

function check_root() {
   if [[ $(id -u) != 0 ]]; then
      echo "[!] To install LDHCP you must be root"
      exit 1
   fi
}

function install() {
   path="/usr/bin"   
   read -p "[?] Default installation path: $path [Y/n] : " option
   option=$(echo "$option" | tr "[:upper:]" "[:lower:]")

   if [[ $option == 'n' ]]; then
      read -p "[?] Path : " path
   fi   

   cp ./build/ldhcp $path
   if [[ $? != 0 ]]; then
      echo "[!] Something went wrong, check installation path."
   else 
      echo "[#] Ldhcp has been successfully installed."
   fi
}

if [[ $1 == "-c" ]]; then
   make_ldhcp
   exit 0
fi

make_ldhcp
check_root
install

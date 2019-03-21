#!/bin/sh
# Filename: start_ledtris.sh
# Project: NES Ledtris
# Author: Koen van Vliet <8by8mail@gmail.com>
# 
# Startup script for fceux emulator
# usage:
#   ./start_ledtris.sh
#

fceux --xscale 2 --yscale 2 --loadlua ledtris.lua ${1:-"tetris.nes"} $2 $3 $4 $5 $6

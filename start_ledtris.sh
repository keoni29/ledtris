#!/bin/bash
# Filename: start_fceux.sh
# Project: NES Ledtris
# Author: Koen van Vliet
# 
# Startup script for fceux emulator
# usage:
#   ./start_ledtris.sh tetris.nes
#

fceux --xscale 2 --yscale 2 --loadlua ledtris.lua tetris.nes $1 $2 $3 $4 $5 $6

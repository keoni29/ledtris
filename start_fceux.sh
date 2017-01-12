#!/bin/bash
mknod fifo p

python2 displayemu.py  &  PIDIOS=$!
fceux --xscale 2 --yscale 2 --loadlua $1 $2 $3 &  PIDMIX=$!
wait $PIDIOS
wait $PIDMIX

rm fifo

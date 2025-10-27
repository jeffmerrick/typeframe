#!/bin/bash
# Decrease brightness by 20
BF=$(ls /sys/class/backlight/*/brightness)
CUR=$(cat $BF)
NEW=$((CUR-20))
[ $NEW -lt 0 ] && NEW=0
echo $NEW | sudo tee $BF
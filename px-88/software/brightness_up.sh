#!/bin/bash
# Increase brightness by 20
BF=$(ls /sys/class/backlight/*/brightness)
CUR=$(cat $BF)
NEW=$((CUR+20))
[ $NEW -gt 255 ] && NEW=255
echo $NEW | sudo tee $BF
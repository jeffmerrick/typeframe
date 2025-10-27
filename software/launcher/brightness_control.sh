#!/bin/bash

while true; do
    choice=$(dialog --clear --backtitle "Brightness Control" \
        --title "Adjust Brightness" \
        --menu "Choose an action:" 18 50 6 \
        1 "Increase Brightness" \
        2 "Decrease Brightness" \
        3 "Set Specific Value" \
        4 "Back to Main Menu" \
        3>&1 1>&2 2>&3)
    status=$?
    if [ $status -ne 0 ] || [ "$choice" == "4" ]; then
        break
    fi
    case $choice in
        1)
            curr=$(cat /sys/class/backlight/*/brightness)
            new=$((curr+20))
            if [ $new -gt 255 ]; then new=255; fi
            echo $new | sudo tee /sys/class/backlight/*/brightness
            ;;
        2)
            curr=$(cat /sys/class/backlight/*/brightness)
            new=$((curr-20))
            if [ $new -lt 0 ]; then new=0; fi
            echo $new | sudo tee /sys/class/backlight/*/brightness
            ;;
        3)
            value=$(dialog --inputbox "Enter brightness (0-255):" 8 50 3>&1 1>&2 2>&3)
            status=$?
            if [ $status -eq 0 ]; then
                if [[ $value =~ ^[0-9]+$ ]] && [ $value -ge 0 ] && [ $value -le 255 ]; then
                    echo $value | sudo tee /sys/class/backlight/*/brightness
                else
                    dialog --msgbox "Invalid value! Must be 0-255." 6 50
                fi
            fi
            ;;
    esac
done

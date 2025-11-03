#!/bin/bash

# Check if dialog is installed
if ! command -v dialog &> /dev/null; then
    echo "Error: dialog is not installed."
    echo "Please install it with: sudo apt-get install dialog"
    exit 1
fi

# Set dialog parameters
DIALOG_WIDTH=40
DIALOG_HEIGHT=15
DIALOG_MENU_HEIGHT=10

# Function to show the main menu
show_menu() {
    sleep 2
    dialog --clear --backtitle "Typeframe Launcher" \
           --title "Welcome Back" \
           --menu "Choose an option:" \
           $DIALOG_HEIGHT $DIALOG_WIDTH $DIALOG_MENU_HEIGHT \
           1 "Launch Editor" \
           2 "Launch Desktop" \
           3 "System Settings" \
           4 "Brightness Control" \
           5 "Battery Status" \
           6 "Exit to Terminal" \
           3>&1 1>&2 2>&3
}

# Function to launch editor
launch_editor() {
    clear
    echo "Opening editor..."
    echo "Starting labwc..."
    labwc &
    LABWC_PID=$!
    sleep 4  # Give labwc time to start
    export DISPLAY=:0
    echo "Starting browser in fullscreen..."
    bash chromium --start-fullscreen "https://docs.google.com/"
}

# Function to run raspi-config
run_raspi_config() {
    clear
    echo "Starting Raspberry Pi config..."
    echo "You may need to enter your password for sudo access."
    echo ""
    sudo raspi-config
    
    # After raspi-config exits, automatically return to launcher
    echo ""
    echo "Raspberry Pi config has been closed."
    echo "Returning to launcher..."
    sleep 1
}

# Function to launch desktop
launch_desktop() {
    clear
    echo "Launching desktop..."
    labwc-pi
    echo "Desktop closed. Returning to launcher..."
}

# Function to exit to terminal
exit_to_terminal() {
    clear
    echo "Exiting launcher..."
    echo "You are now back in the terminal."
    exit 0
}

# Brightness control function
brightness_control() {
    while true; do
        choice=$(dialog --clear --backtitle "Brightness Control" \
            --title "Adjust Brightness" \
            --menu "Choose an action:" 12 50 6 \
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
                echo $new | sudo tee /sys/class/backlight/*/brightness > /dev/null
                ;;
            2)
                curr=$(cat /sys/class/backlight/*/brightness)
                new=$((curr-20))
                if [ $new -lt 0 ]; then new=0; fi
                echo $new | sudo tee /sys/class/backlight/*/brightness > /dev/null
                ;;
            3)
                value=$(dialog --inputbox "Enter brightness (0-255):" 8 50 3>&1 1>&2 2>&3)
                status=$?
                if [ $status -eq 0 ]; then
                    if [[ $value =~ ^[0-9]+$ ]] && [ $value -ge 0 ] && [ $value -le 255 ]; then
                        echo $value | sudo tee /sys/class/backlight/*/brightness > /dev/null
                    else
                        dialog --msgbox "Invalid value! Must be 0-255." 6 50
                    fi
                fi
                ;;
        esac
    done
}

# Battery status function - reads UART directly
battery_status() {
    # Set UART parameters
    sudo stty -F /dev/ttyS0 115200 2>/dev/null
    
    # Read UART data for a few seconds and parse
    uart_data=$(sudo timeout 3 cat /dev/ttyS0 2>/dev/null)
    
    if [ -n "$uart_data" ]; then
        # Extract values using grep and cut
        power_state=$(echo "$uart_data" | grep "Power_State" | head -1 | grep -o '[0-9]*' | head -1)
        running_state=$(echo "$uart_data" | grep "Running_State" | head -1 | grep -o '[0-9]*' | head -1)
        vin_voltage=$(echo "$uart_data" | grep "Vin_Voltage" | head -1 | grep -o '[0-9]*\.[0-9]*' | head -1)
        vout_voltage=$(echo "$uart_data" | grep "Vout_Voltage" | head -1 | grep -o '[0-9]*\.[0-9]*' | head -1)
        vout_current=$(echo "$uart_data" | grep "Vout_Current" | head -1 | grep -o '[0-9]*\.[0-9]*' | head -1)
        
        # Calculate battery percentage using bash arithmetic
        if [ -n "$vin_voltage" ]; then
            # Convert voltage to integer for comparison (multiply by 100)
            vin_int=$(echo "$vin_voltage * 100" | bc 2>/dev/null | cut -d. -f1)
            
            if [ "$vin_int" -gt 410 ]; then
                percentage=100  # Charging/plugged in
                charging_status="Charging/Plugged In"
            else
                # Battery discharge curve calculation
                if [ "$vin_int" -ge 400 ]; then
                    percentage=100
                elif [ "$vin_int" -ge 380 ]; then
                    # 80 + ((vin-3.8)/0.2 * 20)
                    diff=$((vin_int - 380))
                    percentage=$((80 + diff / 10))
                elif [ "$vin_int" -ge 370 ]; then
                    # 60 + ((vin-3.7)/0.1 * 20)
                    diff=$((vin_int - 370))
                    percentage=$((60 + diff * 2))
                elif [ "$vin_int" -ge 360 ]; then
                    # 40 + ((vin-3.6)/0.1 * 20)
                    diff=$((vin_int - 360))
                    percentage=$((40 + diff * 2))
                elif [ "$vin_int" -ge 340 ]; then
                    # 20 + ((vin-3.4)/0.2 * 20)
                    diff=$((vin_int - 340))
                    percentage=$((20 + diff / 10))
                elif [ "$vin_int" -ge 320 ]; then
                    # 0 + ((vin-3.2)/0.2 * 20)
                    diff=$((vin_int - 320))
                    percentage=$((diff / 10))
                else
                    percentage=0
                fi
                
                # Ensure percentage is within bounds
                if [ "$percentage" -gt 100 ]; then percentage=100; fi
                if [ "$percentage" -lt 0 ]; then percentage=0; fi
                
                charging_status="Running on Battery"
            fi
            
            # Create status message
            status_msg="Battery Level: ${percentage}%
Voltage: ${vin_voltage}V
${charging_status}

Power State: ${power_state}
Running State: ${running_state}
Output Voltage: ${vout_voltage}V
Output Current: ${vout_current}mA"
            
            dialog --msgbox "$status_msg" 15 50
        else
            dialog --msgbox "Unable to read voltage data from UART." 8 40
        fi
    else
        dialog --msgbox "Unable to read battery status.\nCheck UART connection." 8 40
    fi
}

# Main loop
while true; do
    choice=$(show_menu)
    status=$?
    if [ $status -ne 0 ]; then
        exit_to_terminal
    fi
    case $choice in
        1)
            launch_editor
            ;;
        2)
            launch_desktop
            ;;
        3)
            run_raspi_config
            ;;
        4)
            brightness_control
            ;;
        5)
            battery_status
            ;;
        6)
            exit_to_terminal
            ;;
        *)
            # Invalid choice, show menu again
            ;;
    esac
done

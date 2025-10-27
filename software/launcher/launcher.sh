#!/bin/bash

# Check if dialog is installed
if ! command -v dialog &> /dev/null; then
    echo "Error: dialog is not installed."
    echo "Please install it with: sudo apt-get install dialog"
    exit 1
fi

# Set minimal black and white theme for dialog
export DIALOGRC=/tmp/dialogrc_minimal
cat > /tmp/dialogrc_minimal << 'EOF'
# Minimal black and white theme
screen_color = (WHITE,BLACK,ON)
dialog_color = (BLACK,WHITE,OFF)
title_color = (BLACK,WHITE,ON)
border_color = (BLACK,WHITE,ON)
button_active_color = (WHITE,BLACK,ON)
button_inactive_color = (BLACK,WHITE,OFF)
menubox_color = (BLACK,WHITE,OFF)
menubox_border_color = (BLACK,WHITE,ON)
item_color = (BLACK,WHITE,OFF)
item_selected_color = (WHITE,BLACK,ON)
tag_color = (BLACK,WHITE,ON)
tag_selected_color = (WHITE,BLACK,ON)
EOF

# Set dialog parameters
DIALOG_WIDTH=40
DIALOG_HEIGHT=15
DIALOG_MENU_HEIGHT=10

# Function to show the main menu
show_menu() {
    dialog --clear --backtitle "Typeframe Launcher" \
           --title "Welcome Back" \
           --menu "Choose an option:" \
           $DIALOG_HEIGHT $DIALOG_WIDTH $DIALOG_MENU_HEIGHT \
           1 "Launch Editor" \
           2 "Launch Desktop" \
           3 "System Settings" \
           4 "Brightness Control" \
           5 "Exit to Terminal" \
           3>&1 1>&2 2>&3
}

# Function to launch editor
launch_editor() {
    clear
    echo "Opening editor..."
    echo "Starting browser in fullscreen..."
    labwc &
    LABWC_PID=$!
    sleep 2  # Give labwc time to start
    chromium-browser --start-fullscreen --no-memcheck "https://docs.google.com/"
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

# Brightness control function now in its own file
brightness_control() {
    bash /home/jeff/launcher/brightness_control.sh
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
            exit_to_terminal
            ;;
        *)
            # Invalid choice, show menu again
            ;;
    esac
done

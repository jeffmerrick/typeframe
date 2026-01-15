---
title: Terminal
sidebar_position: 4
description: Customize your terminal style
---

# Customize the Terminal

Make everything a retro green terminal with a larger font and add some Nostromo-themed boot text.

## Font Configuration

### 1. Install Terminus Font

It might already be pre-installed, but just to be safe.

```bash
sudo apt update
sudo apt install fonts-terminus
```

### 2. Configure Console Font

Edit the console setup configuration:

```bash
sudo nano /etc/default/console-setup
```

Set the following values:

```
ACTIVE_CONSOLES="/dev/tty[1-6]"
CHARMAP="UTF-8"
CODESET="guess"
FONT=""
FONTFACE="Terminus"
FONTSIZE="12x24"
VIDEOMODE=
```

### 3. Apply the Configuration

Apply the font changes:

```bash
sudo setupcon
```

The font should now be active. However, it won't persist after reboot yet (see next step).

### 4. Fix Font Persistence

The font configuration doesn't seem to persist across reboots by default so we need to create a service to enforce it.

Create the service file:

```bash
sudo nano /etc/systemd/system/console-font-fix.service
```

Add the following content:

```ini
[Unit]
Description=Fix Console Font Persistence
After=getty.target systemd-vconsole-setup.service

[Service]
Type=oneshot
ExecStart=/bin/sh -c '/usr/bin/setupcon'
StandardOutput=journal

[Install]
WantedBy=multi-user.target
```

Enable and start the service:

```bash
sudo systemctl daemon-reload
sudo systemctl enable console-font-fix.service
sudo systemctl restart console-font-fix.service
sudo systemctl status console-font-fix.service
```

## Nostromo-Themed Boot Text

This displays some Nostromo-themed text in the terminal on boot. It's set up to work with the font size we configured above.

### 1. Create the Nostromo Display Script

Add the display function to your `.bashrc` file:

```bash
nano ~/.bashrc
```

Add this complete code at the end of the file:

```bash
# --- NOSTROMO LOGIN SCREEN SETUP ---

# Set green color for prompt
echo -ne "\e[92m"
PS1='\[\e[92m\]\u@\h:\w \$ '

# Apply font configuration
sudo setupcon
clear

function display_nostromo_screen() {
   # 1. Clear the screen and set text color to green
   tput clear
   tput setaf 2  # Set foreground color to green

   # 2. Define the dynamic date variable (Actual Time)
   CURRENT_DATE=$(date +'%e %^b' | sed 's/^ *//')

   # 3. Calculate FLIGHT_TIME (155 days ahead of CURRENT_DATE)
   FLIGHT_TIME=$(date -d "+155 days" +'%e %^b' | sed 's/^ *//')

   # 3. Print the top header section (Rows 0-2)

   # Row 0
   tput cup 0 0; echo "COMPUTER"
   tput cup 0 30; echo "12493.D"
   tput cup 0 74; echo "SHIP"

   # Row 1
   tput cup 1 0; echo "ACTUAL TIME: ${CURRENT_DATE}"
   tput cup 1 74; echo "WEYLAND YUTANI"

   # Row 2
   tput cup 2 0; echo "FLIGHT TIME: ${FLIGHT_TIME}"
   tput cup 2 74; echo "NOSTROMO 180924"

   # --- BOX START ---

   # Transmission Header (Row 4)
   tput cup 4 0; echo "+---// TRANSMISSION //-----------------------------------------------+"

   # Top Edge of the Box (Row 5)
   tput cup 5 0; echo "+--------------------------------------------------------------------+"

   # Vertical Sides (Rows 6-13)
   for R in {6..13}; do
      tput cup $R 0; echo "|"
      tput cup $R 69; echo "|"
   done

   # --- Directive Text (Inside Box) ---
   TEXT_COL=4 # Starting column for the text inside the border

   tput cup 8 $TEXT_COL; echo "PRIORITY ONE"
   tput cup 9 $TEXT_COL; echo "INSURE RETURN OF ORGANISM FOR ANALYSIS."
   tput cup 10 $TEXT_COL; echo "ALL OTHER CONSIDERATIONS SECONDARY."
   tput cup 11 $TEXT_COL; echo "CREW EXPENDABLE."

   # Bottom Edge of the Box (Row 14)
   tput cup 14 0; echo "+--------------------------------------------------------------------+"

   # 4. Print the main information sections (Right Side)

   # Row 4 (FUNCTION)
   tput cup 4 74; echo "FUNCTION:"
   tput cup 5 74; echo "TANKER/REFINERY"

   # Row 7 (CAPACITY)
   tput cup 7 74; echo "CAPACITY:"
   tput cup 8 74; echo "200 000 000 TONNES"

   # Row 10 (GALACTIC POSITION)
   tput cup 10 74; echo "GALACTIC POSITION:"
   tput cup 11 74; echo "270/RX683*"

   # Row 13 (VELOCITY STATUS)
   tput cup 13 74; echo "VELOCITY STATUS:"
   tput cup 14 74; echo "58 09/ 5DL"

   # 5. Reset the terminal properties for the user prompt
   tput sgr0    # Reset all attributes (color, etc.)
}

# Execute the function to display the screen before the prompt appears
display_nostromo_screen

# --- END NOSTROMO LOGIN SCREEN DISPLAY ---
```

### 2. Test the Setup

Source the updated `.bashrc` to test immediately:

```bash
source ~/.bashrc
```

If it displays correctly, reboot to verify persistence:

```bash
sudo reboot
```

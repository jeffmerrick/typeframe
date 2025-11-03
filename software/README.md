# Raspberry Pi 4B Setup Guide

This guide covers the complete setup process for a **Raspberry Pi 4B** running **Raspberry Pi OS Bookworm** with Waveshare 7.9" DSI display and Power Management HAT.

---

## 1. OS and Screen Setup (Waveshare 7.9inch DSI Touch LCD)

### Initial System Installation

1. **Flash Raspberry Pi OS Bookworm:**

   - Connect the SD card to your computer
   - Download and use [Raspberry Pi Imager](https://www.raspberrypi.com/software/) to flash the corresponding system image
   - I highly recommend configuring WiFi and SSH options in the imager under Use **"OS customization -> Edit Settings"**. This will allow the Pi to connect to your WiFi right after the first boot, and you can SSH in to more easily configure it. You can choose a hostname and username. For the purposes of this guide, we'll assume the hostname is `typeframe.local` and the username is `pi`. Leaving the username as `pi` will make things easier on you later as it's hardcoded in some scripts.

2. **Configure Display Support:**

   - After image flashing is completed, open the `config.txt` file in the root directory of the SD card
   - Add the following code at the end of `config.txt`, save and safely eject the SD card:

   ```
   dtoverlay=vc4-kms-v3d
   # DSI1 Use (recommended for Pi4B)
   dtoverlay=vc4-kms-dsi-waveshare-panel,7_9_inch
   # DSI0 Use (alternative)
   # dtoverlay=vc4-kms-dsi-waveshare-panel,7_9_inch,dsi0
   ```

   If you boot before adding this, you can SSH in to the Pi and edit the file in `/boot/firmware/config.txt`. Then reboot.

3. **First Boot:**
   - Insert the SD card into the Raspberry Pi 4B
   - Plug the USB power cable into the Pi, _not_ the Power Management HAT
   - The Raspberry Pi will power on - wait for a few seconds for the display to initialize
   - You're booted up - the touch screen should work normally

### SSH into the Raspberry Pi

If you configured WiFi and SSH in the imager, you can SSH into the Pi from your computer using the hostname you set (e.g., `typeframe.local`):

```bash
ssh pi@typeframe.local
```

This will allow you to copy and paste commands more easily than working directly on the Pi.

### Screen Rotation Configuration

The default display orientation is portrait. We want it to be landscape. You'll need to set this in two places, one for the desktop environment and one for the console.

#### 1: Desktop Configuration (Bookworm Touch Screen Rotation)

1. Open **"Control Center" → "Screens"** from the desktop menu. _In older versions of Raspberry Pi OS, this may be under "Preferences" → "Screen Configuration"._

2. Go to **"Screens" → "DSI-2" → "Orientation"**, choose "Right", and finally click **"Apply"** to complete the display and touch synchronous rotation

For detailed steps with images, see the [Waveshare wiki documentation](https://www.waveshare.com/wiki/7.9inch_DSI_LCD#Bookworm_Touch_Screen_Rotation).

#### 2: Disable the On-Screen Keyboard

By default the on-screen keyboard will pop up whenever you select a text field. You'll find the option to disable it under "Control Center" → "Display" → "On-Screen Keyboard". Select "Disabled"

#### 3: Terminal Configuration (Optional if you'll only use the desktop)

To rotate the display in the terminal, modify the boot command line:

```bash
sudo nano /boot/firmware/cmdline.txt
```

Add the following command to the **beginning** of the first line in `cmdline.txt`, followed by a space before the existing `console=` parameter:

```bash
video=DSI-1:400x1280e,rotate=270
```

**For example:** If your `cmdline.txt` currently starts with `console=tty1 root=PARTUUID=...`, change it to:

```
video=DSI-1:400x1280e,rotate=270 console=tty1 root=PARTUUID=...
```

Save the file and restart to apply the rotation.

---

## 2. Waveshare Power Management HAT (B) - Configuration

This section details the necessary steps to configure the HAT for **Button Control (Power ON/Soft Shutdown)** using a `systemd` service approach and compiled firmware.

I made some modifications to the official Waveshare instructions - this is what worked for me. Please review the official documentation as well.

For complete documentation, see the [Waveshare Power Management HAT (B) wiki](<https://www.waveshare.com/wiki/Power_Management_HAT_(B)>).

---

### 1. Set Up RP2040 Development Tools

This prepares the Raspberry Pi to compile and flash the firmware for the HAT's onboard RP2040 microcontroller.

1. **Configure Pico Compile Environment:**

   ```bash
   cd ~
   wget https://raw.githubusercontent.com/raspberrypi/pico-setup/master/pico_setup.sh
   chmod +x pico_setup.sh
   ./pico_setup.sh
   sudo reboot
   ```

2. **Install and Compile OpenOCD:**

   ```bash
   cd ~/pico
   sudo apt install automake autoconf build-essential texinfo libtool libftdi-dev libusb-1.0-0-dev
   git clone https://github.com/raspberrypi/openocd.git --recursive --branch rp2040-v0.12.0 --depth=1

   cd openocd
   ./bootstrap
   ./configure --enable-ftdi --enable-sysfsgpio --enable-bcm2835gpio
   make -j3
   sudo make install
   ```

---

### 2. Install Soft Shutdown Script

This ensures the Raspberry Pi OS runs a script to listen for the shutdown signal from the HAT.

1. **Download and Extract Files:**

   ```bash
   cd ~
   wget https://files.waveshare.com/upload/4/44/Power-Management-HAT.zip
   unzip Power-Management-HAT.zip

   # Move the Python script
   sudo mkdir -p $HOME/bin/PowerManagementHAT
   sudo mv -f ~/Power-Management-HAT/StatusDetection.py $HOME/bin/PowerManagementHAT/
   ```

2. **Create the `systemd` Service File:**

   ```bash
   sudo nano /etc/systemd/system/waveshare-shutdown.service
   ```

   Paste and save the following configuration (runs the script as **root** for system access):

   ```ini
   [Unit]
   Description=Waveshare Power HAT Shutdown Monitor
   After=multi-user.target

   [Service]
   Type=simple
   User=root
   Group=root
   ExecStart=/usr/bin/python3 /home/pi/bin/PowerManagementHAT/StatusDetection.py
   Restart=always
   RestartSec=5
   StandardOutput=journal
   StandardError=journal

   [Install]
   WantedBy=multi-user.target
   ```

3. **Enable UART Listening**

For the battery status light to work, enable UART on the Raspberry Pi:

```bash
sudo raspi-config nonint do_serial 1
```

You can also find this in nthe `raspi-config` interface under **"Interface Options" → "Serial Port"**.

3. **Enable and Start the Service:**

   ```bash
   sudo systemctl daemon-reload
   sudo systemctl enable waveshare-shutdown.service
   sudo systemctl start waveshare-shutdown.service
   ```

---

### 3. Compile and Flash the Button Firmware

This final step flashes firmware that enables button control.

1. **Get Demo Source Code:**

   ```bash
   cd ~
   wget https://files.waveshare.com/upload/2/27/Power-example.7z
   7z x ./Power-example.7z
   ```

2. **Edit Source Code to Enable Button Demo:**

   Open the main C file (`~/Power-example/Power_Management_HAT.c`) and enable the `Button_Ctr_` lines while commenting out the `Period_Time_` lines.

   ```bash
   cd ~/Power-example/build/
   rm -rf *
   export PICO_SDK_PATH=~/pico/pico-sdk
   cmake ../
   make -j
   ```

3. **Flash the Firmware to the HAT:**

   Place the HAT in **BOOTSEL mode**, then run:

   ```bash
   openocd -f interface/raspberrypi-swd.cfg -f target/rp2040.cfg -c "program ./Power_Management_HAT.elf verify reset exit"
   ```

4. ** Confirm the Firmware is Working**
   Unplug the USB cable from the Pi, reconnect it to the Power Management HAT. You can now use the PWR button to turn on the Pi.

---

### 4. Default Button Behavior

Once the **Button_Ctr Demo** firmware is running, the onboard PWR button will function as follows:

| Action             | Duration                                 | Result                                                            |
| :----------------- | :--------------------------------------- | :---------------------------------------------------------------- |
| **Power On**       | Short Press (< 2 seconds)                | The HAT powers the Raspberry Pi on.                               |
| **Soft Shutdown**  | Long Press (> 1 seconds but < 8 seconds) | Pi executes a graceful software shutdown, and the HAT cuts power. |
| **Hard Power Off** | Very Long Press (> 8 seconds)            | HAT immediately cuts power to the Pi.                             |

---

## Configure Brightness Control Shortcuts

If you're using labwc as your Wayland compositor, you can set up keyboard shortcuts for brightness control:

### 1. Copy Brightness Scripts

```bash
# Download launcher scripts from GitHub
mkdir ~/Typeframe
cd ~/Typeframe
wget [your-github-url]/brightness_up.sh
wget [your-github-url]/brightness_down.sh
```

Make sure these scripts are executable:

```bash
chmod +x ~/Typeframe/brightness_*.sh
```

### 2. Configure labwc Keyboard Shortcuts

Add the following keyboard shortcuts to your `~/.config/labwc/rc.xml` file, creating it if it doesn't exist:

```xml
<?xml version="1.0"?>
<openbox_config xmlns="http://openbox.org/3.4/rc">
	<!-- Your existing configuration -->

	<keyboard>
		<!-- Hardware brightness keys (if you can configure correctly in VIA) -->
		<keybind key="XF86MonBrightnessUp">
			<action name="Execute">
				<command>/bin/sh -c '$HOME/Typeframe/brightness_up.sh'</command>
			</action>
		</keybind>
		<keybind key="XF86MonBrightnessDown">
			<action name="Execute">
				<command>/bin/sh -c '$HOME/Typeframe/brightness_down.sh'</command>
			</action>
		</keybind>

		<!-- Alternative shortcuts using Super + Arrow keys -->
		<keybind key="W-Up">
			<action name="Execute">
				<command>/bin/sh -c '$HOME/Typeframe/brightness_up.sh'</command>
			</action>
		</keybind>
		<keybind key="W-Down">
			<action name="Execute">
				<command>/bin/sh -c '$HOME/Typeframe/brightness_down.sh'</command>
			</action>
		</keybind>
	</keyboard>
</openbox_config>
```

### 3. Reload labwc Configuration

After editing the `rc.xml` file, reload the labwc configuration:

```bash
labwc-pi --reconfigure
```

or reboot:

```bash
sudo reboot
```

### 4. Test Brightness Controls

You can now control brightness using:

- **Hardware brightness keys** (if configured in VIA)
- **Super + Up Arrow** - Increase brightness
- **Super + Down Arrow** - Decrease brightness

---

## Update Boot Splash Image

### 1. Change Boot Splash Image

To change the Raspberry Pi boot splash image to a custom Typeframe image:

1. Open a terminal and go to the Plymouth theme directory:
   ```bash
   cd /usr/share/plymouth/themes/pix/
   ```
2. (Optional) Back up the original splash image:
   ```bash
   sudo mv splash.png splash.png.bk
   ```
3. Copy your new splash image into place:
   ```bash
   sudo cp ~/Typeframe/images/typeframe-boot-simple.png ./splash.png
   ```
4. Refresh the Plymouth theme:
   ```bash
   sudo plymouth-set-default-theme -R pix
   ```

Note: Images need to be rotated 90 degrees clockwise since the display isn't rotated until after boot. Match the screen size of 1280x400 pixels for best results.

### 2. Hide Boot Text

Edit the Plymouth theme configuration to hide boot text:

```bash
sudo nano /usr/share/plymouth/themes/pix/pix.script
```

Remove or comment out the last lines:

```bash
#message_sprite = Sprite();
#message_sprite.SetPosition(screen_width * 0.1, screen_height * 0.9, 10000);
#
#fun message_callback (text) {
#       my_image = Image.Text(text, 1, 1, 1);
#       message_sprite.SetImage(my_image);
#       sprite.SetImage (resized_image);
#}
#
#Plymouth.SetUpdateStatusFunction(message_callback);
```

---

## Add Simple LED Status Light

You can add a simple LED status light to indicate system status using GPIO pins.

### 1. Test the LED

```bash
gpioset -c gpiochip0 13=1
sleep 1
gpioset -c gpiochip0 13=0
```

### 2. Edit Configuration

```bash
sudo nano /boot/firmware/config.txt
```

Add the following line at the end:

```bash
# Activity LED indicator on GPIO 13
dtoverlay=act-led,gpio=13
```

### 3. Reboot

```bash
sudo reboot
```

---

## Add Battery Status Notification Service

### 1. Copy battery status script from GitHub

```bash
# Download battery status scripts from GitHub
cd ~/Typeframe
wget [your-github-url]/battery_monitor.py
wget [your-github-url]/send_user_notification.sh
```

### 2. Make the scripts executable

```bash
chmod +x ~/Typeframe/battery_monitor.py
chmod +x ~/Typeframe/send_user_notification.sh
```

### 3. Create systemd service

```bash
sudo nano /etc/systemd/system/battery-monitor.service
```

Paste the following configuration, change the username from `pi` if necessary:

```ini
[Unit]
Description=Battery Monitor with Notifications
After=graphical-session.target
Wants=graphical-session.target

[Service]
Type=simple
ExecStart=/home/pi/Typeframe/battery_monitor.py
Restart=always
RestartSec=30
User=root
Environment=PYTHONUNBUFFERED=1
Environment=WAYLAND_DISPLAY=wayland-1
Environment=XDG_RUNTIME_DIR=/run/user/1000

[Install]
WantedBy=multi-user.target
```

```bash
# Reload systemd and enable service
sudo systemctl daemon-reload
sudo systemctl enable battery-monitor.service
sudo systemctl start battery-monitor.service
```

```bash
# Check service status
sudo systemctl status battery-monitor.service

# View service logs
sudo journalctl -u battery-monitor.service -f

# Test battery reading manually
sudo python3 /home/pi/Typeframe/battery_monitor.py
```

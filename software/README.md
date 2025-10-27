# Raspberry Pi 4B Setup Guide

This guide covers the complete setup process for a **Raspberry Pi 4B** running **Raspberry Pi OS Bookworm** with Waveshare 7.9" DSI display and Power Management HAT.

---

## 1. OS and Screen Setup (Waveshare 7.9inch DSI Touch LCD)

### Initial System Installation

1. **Flash Raspberry Pi OS Bookworm:**

   - Connect the SD card to your computer
   - Download and use [Raspberry Pi Imager](https://www.raspberrypi.com/software/) to flash the corresponding system image

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

3. **First Boot:**
   - Insert the SD card into the Raspberry Pi 4B
   - Plug the USB power cable into the Pi, _not_ the Power Management HAT
   - The Raspberry Pi will power on - wait for a few seconds for the display to initialize
   - You're booted up - the touch screen should work normally

### Screen Rotation Configuration

The default display orientation is **portrait**. We want it to be landscape. You'll need to set this in two places, one for the desktop environment and one for the console.

#### 1: Desktop Configuration (Bookworm Touch Screen Rotation)

1. Open the "Screen Configuration" application from the desktop menu

2. Go to **"Screen" → "DSI-2" → "Touchscreen"**, check **"11-005d Goodix Capacitive TouchScreen"**, and click **"Apply"** to complete the selection of the specified touchscreen

3. Go to **"Screen" → "DSI-2" → "Orientation"**, check the direction you need to rotate, and finally click **"Apply"** to complete the display and touch synchronous rotation

**Note:** This synchronization rotation method is supported in Bookworm.

For detailed steps with images, see the [Waveshare wiki documentation](https://www.waveshare.com/wiki/7.9inch_DSI_LCD#Bookworm_Touch_Screen_Rotation).

#### 2: Terminal Configuration (Optional if you'll only use the desktop)

To rotate the display in the terminal, modify the boot command line:

```bash
sudo nano /boot/firmware/cmdline.txt
```

Add the following command to the **beginning** of the first line in `cmdline.txt`, followed by a space before the existing `console=tty1` parameter:

```bash
# For 270 degree rotation:
video=DSI-1:400x1280e,rotate=270
```

**Example:** If your `cmdline.txt` currently starts with `console=tty1 root=PARTUUID=...`, change it to:

```
video=DSI-1:400x1280e,rotate=270 console=tty1 root=PARTUUID=...
```

Save the file and restart to apply the rotation.

---

## 2. Waveshare Power Management HAT (B) - Configuration

This section details the necessary steps to configure the HAT for reliable **Button Control (Power ON/Soft Shutdown)** using the modern `systemd` service approach and compiled firmware.

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

### 2. Install Soft Shutdown Script (The `systemd` Way)

This ensures the Raspberry Pi OS runs a script to listen for the shutdown signal from the HAT.

1. **Download and Extract Files:**

   ```bash
   cd ~
   wget https://files.waveshare.com/upload/4/44/Power-Management-HAT.zip
   unzip Power-Management-HAT.zip

   # Move the essential Python script to its final location
   sudo mkdir -p /home/$USER/bin/PowerManagementHAT
   sudo mv -f ~/Power-Management-HAT/StatusDetection.py /home/$USER/bin/PowerManagementHAT/
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
   ExecStart=/usr/bin/python3 /home/$USER/bin/PowerManagementHAT/StatusDetection.py

   [Install]
   WantedBy=multi-user.target
   ```

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

2. **Edit Source Code to Enable Button Demo (CRITICAL STEP):**

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

You can instead clone the entire Typeframe repository if you prefer, but the 3D print files will also be included.

```bash
cd ~
git clone [your-github-url]/Typeframe.git
```

Make sure these scripts are executable:

```bash
chmod +x ~/Typeframe/brightness_*.sh
```

### 2. Configure labwc Keyboard Shortcuts

Add the following keyboard shortcuts to your `~/.config/labwc/rc.xml` file, creating it if it doesn't exist:

```xml
<?xml version="1.0"?>
<labwc_config>
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
				<command>/bin/sh -c '$HOME/launcher/brightness_up.sh'</command>
			</action>
		</keybind>
		<keybind key="W-Down">
			<action name="Execute">
				<command>/bin/sh -c '$HOME/launcher/brightness_down.sh'</command>
			</action>
		</keybind>
	</keyboard>
</labwc_config>
```

### 3. Reload labwc Configuration

After editing the `rc.xml` file, reload the labwc configuration:

```bash
labwc --reconfigure
```

### 4. Test Brightness Controls

You can now control brightness using:

- **Hardware brightness keys** (if your device has them)
- **Super + Up Arrow** - Increase brightness
- **Super + Down Arrow** - Decrease brightness

---

## Update Boot Splash Image

To change the Raspberry Pi boot splash image to your custom Typeframe image:

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

On the next boot, the custom splash image will be displayed.

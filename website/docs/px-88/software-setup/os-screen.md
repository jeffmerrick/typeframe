---
title: OS & Screen
sidebar_position: 2
description: Installing Raspberry Pi OS and configuring the display.
---

## Flash OS & Boot

### 1.Flash Raspberry Pi OS Trixie

1. Connect the SD card to your computer
2. Download and use [Raspberry Pi Imager](https://www.raspberrypi.com/software/) to flash the latest Raspberry Pi OS (32-bit) - Trixie to the SD card.

:::tip[OS customization]

I highly recommend configuring WiFi and SSH options in the imager under Use **"OS customization -> Edit Settings"**. This will allow the Pi to connect to your WiFi right after the first boot, and you can SSH in to more easily configure it. You can choose a hostname and username. For the purposes of this guide, we'll assume the hostname is `typeframe.local` and the username is `pi`. Leaving the username as `pi` will make things easier on you later as it's hardcoded in some scripts.

:::

### 2. Configure Display Support

1. After image flashing is completed, open the `config.txt` file in the root directory of the SD card
2. Add the following code at the end of `config.txt`, save and safely eject the SD card:

   ```
   dtoverlay=vc4-kms-v3d
   # DSI1 Use (recommended for Pi4B)
   dtoverlay=vc4-kms-dsi-waveshare-panel,7_9_inch
   # DSI0 Use (alternative)
   # dtoverlay=vc4-kms-dsi-waveshare-panel,7_9_inch,dsi0
   ```

If you boot before adding this, you can SSH in to the Pi and edit the file in `/boot/firmware/config.txt`. Then reboot.

### 3. First Boot

1. Insert the SD card into the Raspberry Pi.
2. Plug the USB power cable into the Pi, _not_ the Power Management HAT if you have it installed.
3. The Raspberry Pi will power on - wait for a few seconds for the display to initialize.
4. You're booted up - the touch screen should work but you'll still need to rotate the screen.

## SSH into the Raspberry Pi

If you configured WiFi and SSH in the imager, you can SSH into the Pi from your computer using the hostname you set (e.g., `typeframe.local`):

```bash
ssh pi@typeframe.local
```

This will allow you to copy and paste commands more easily than working directly on the PX-88.

## Screen Rotation Configuration

The default display orientation is portrait. We want it to be landscape. You'll need to set this in two places, one for the desktop environment and one for the terminal.

### 1. Desktop Configuration

1. Open **"Control Centre" → "Screens"** from the desktop menu. _In older versions of Raspberry Pi OS, this may be under "Preferences" → "Screen Configuration"._

2. Go to **"Screens" → "DSI-2" → "Orientation"**, choose "Right", and finally click **"Apply"** to complete the display and touch synchronous rotation

For detailed steps with images, see the [Waveshare wiki documentation](https://www.waveshare.com/wiki/7.9inch_DSI_LCD#Bookworm_Touch_Screen_Rotation).

### 2. Disable the On-Screen Keyboard

By default the on-screen keyboard will pop up whenever you select a text field. You'll find the option to disable it under **"Control Centre" → "Display" → "On-Screen Keyboard"**. Select **"Disabled"**

### 3. Terminal Configuration

1. To rotate the display in the terminal, open the boot command line for editing.

   ```bash
   sudo nano /boot/firmware/cmdline.txt
   ```

2. Add the following command to the **beginning** of the first line in `cmdline.txt`, followed by a space before the existing `console=` parameter:

   ```bash
   video=DSI-1:400x1280e,rotate=270
   ```

   **For example:** If your `cmdline.txt` currently starts with `console=tty1 root=PARTUUID=...`, change it to:

   ```
   video=DSI-1:400x1280e,rotate=270 console=tty1 root=PARTUUID=...
   ```

3. Save the file and exit `(Ctrl+X, Y, Enter)`,

4. Restart to apply the rotation.

   ```bash
   sudo reboot
   ```

---
title: OS & Screen
sidebar_position: 2
description: Installing Raspberry Pi OS and configuring the display.
---

## Flash OS & Boot

### 1.Flash Raspberry Pi OS Trixie

1. Connect the SD card to your computer
2. Download and use [Raspberry Pi Imager](https://www.raspberrypi.com/software/) to flash the latest Raspberry Pi OS Lite (64-bit) - Trixie to the SD card.

:::tip[OS customization]

I highly recommend configuring WiFi and SSH options in the imager under **"OS customization -> Edit Settings"**. This will allow the Pi to connect to your WiFi right after the first boot, and you can SSH in to more easily configure it. You can choose a hostname and username. For the purposes of this guide, we'll assume the hostname is `typeframe-ps.local` and the username is `pi`. Leaving the username as `pi` will make things easier on you later as it's hardcoded in some scripts.

:::

### 2. Configure Display Support

1. After image flashing is completed, open the `config.txt` file in the root directory of the SD card.
2. Add the following code at the end of `config.txt`, save, and safely eject the SD card:

   ```
   hdmi_group=2
   hdmi_mode=87
   hdmi_timings=400 0 220 32 110 1280 0 10 10 10 0 0 0 60 0 59400000 3
   ```

If you boot before adding this, you can SSH into the Pi and edit the file in `/boot/firmware/config.txt`. Then reboot. (Side note: everything seems to work fine without this, but it's in the Waveshare documentation so I included it here.)

### 3. First Boot

1. Insert the SD card into the Raspberry Pi.
2. Plug the USB power cable into the Pi.
3. The Raspberry Pi will power on - wait for a few seconds for the display to initialize.
4. You're booted up and should be in the terminal.

## SSH into the Raspberry Pi

If you configured WiFi and SSH in the imager, you can SSH into the Pi from your computer using the hostname you set (e.g., `typeframe-ps.local`):

```bash
ssh pi@typeframe-ps.local
```

This will allow you to copy and paste commands more easily than working directly on the PX-88.

## Screen Rotation Configuration

The default display orientation is portrait. We want it to be landscape. You'll need to set this in two places: one for the desktop environment and one for the terminal.

### Terminal Configuration

1. To rotate the display in the terminal, open the boot command line for editing.

   ```bash
   sudo nano /boot/firmware/cmdline.txt
   ```

2. Add the following command to the **beginning** of the first line in `cmdline.txt`, followed by a space before the existing `console=` parameter:

   ```bash
   video=HDMI-A-1:400x1280M@60,rotate=270
   ```

   **For example:** If your `cmdline.txt` currently starts with `console=tty1 root=PARTUUID=...`, change it to:

   ```
   video=HDMI-A-1:400x1280M@60,rotate=270 console=tty1 root=PARTUUID=...
   ```

3. Save the file and exit `(Ctrl+X, Y, Enter)`.

4. Restart to apply the rotation.

   ```bash
   sudo reboot
   ```

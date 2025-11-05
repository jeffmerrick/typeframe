---
title: Power Management HAT
sidebar_position: 3
description: Flash the firmware for the Power Management HAT.
---

# Power Management HAT Setup

This section details the necessary steps to configure the HAT for Button Control (Power ON/Soft Shutdown) using a `systemd` service and the Waveshare provided example firmware.

I had to make some changes to the official Waveshare guide to get it working; this is what worked for me. For complete documentation, see the [Waveshare Power Management HAT (B) Wiki](<https://www.waveshare.com/wiki/Power_Management_HAT_(B)>).

## Set Up RP2040 Development Tools

This prepares the Raspberry Pi to compile and flash the firmware for the HAT's onboard RP2040 microcontroller.

1. Configure Pico Compile Environment:

   ```bash
   cd ~
   wget https://raw.githubusercontent.com/raspberrypi/pico-setup/master/pico_setup.sh
   chmod +x pico_setup.sh
   ./pico_setup.sh
   sudo reboot
   ```

2. Install and Compile OpenOCD:

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

## Install Soft Shutdown Script

This ensures the Raspberry Pi OS runs a script to listen for the shutdown signal from the HAT.

1. Download and Extract Files:

   ```bash
   cd ~
   wget https://files.waveshare.com/upload/4/44/Power-Management-HAT.zip
   unzip Power-Management-HAT.zip

   # Move the Python script
   sudo mkdir -p $HOME/bin/PowerManagementHAT
   sudo mv -f ~/Power-Management-HAT/StatusDetection.py $HOME/bin/PowerManagementHAT/
   ```

2. Create the `systemd` Service File:

   ```bash
   sudo nano /etc/systemd/system/waveshare-shutdown.service
   ```

   Paste and save (Ctrl+X, Y, Enter) the following configuration:

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

3. Enable and Start the Service:

   ```bash
   sudo systemctl daemon-reload
   sudo systemctl enable waveshare-shutdown.service
   sudo systemctl start waveshare-shutdown.service
   ```

## Enable UART Listening

For the battery status to work, enable UART on the Raspberry Pi. This will allow the RP2040 to send battery status data to the Pi.

```bash
sudo raspi-config nonint do_serial 1
```

You can also find this in the `raspi-config` interface under **"Interface Options" → "Serial Port"**.

## Compile and Flash the Button Firmware

This final step flashes firmware that enables button control.

1. Get Demo Source Code

   ```bash
   cd ~
   wget https://files.waveshare.com/upload/2/27/Power-example.7z
   7z x ./Power-example.7z
   ```

2. Edit Source Code to Enable Button Demo

   Open the main C file (`~/Power-example/Power_Management_HAT.c`) and enable the `Button_Ctr_` lines while commenting out the `Period_Time_` lines.

   ```c
      int main()
      {
      /* Initialize function */
      Button_Ctr_Init();
      //Period_Time_Init();
      //Cycle_Time_Init();

      /*Calls loop functions*/
      while (true)
      {
         Button_Ctr_Loop();
         //Period_Time_Loop();
         //Cycle_Time_Loop();
      }

      return 0;
      }
   ```

3. Flash the Firmware to the HAT

   Place the HAT in **BOOTSEL mode**, then run:

   ```bash
   openocd -f interface/raspberrypi-swd.cfg -f target/rp2040.cfg -c "program ./Power_Management_HAT.elf verify reset exit"
   ```

4. Confirm the Firmware is Working
   Unplug the USB cable from the Pi, reconnect it to the Power Management HAT. You can now use the PWR button to turn the Pi on and off.

---

## Default Button Behavior

Once the firmware is running, the onboard PWR button will function as follows:

| Action             | Duration                                 | Result                                               |
| :----------------- | :--------------------------------------- | :--------------------------------------------------- |
| **Power On**       | Short Press (< 2 seconds)                | The HAT powers the Raspberry Pi on.                  |
| **Soft Shutdown**  | Long Press (> 4 seconds but < 8 seconds) | Pi executes a soft shutdown, and the HAT cuts power. |
| **Hard Power Off** | Very Long Press (> 8 seconds)            | HAT immediately cuts power to the Pi.                |

---

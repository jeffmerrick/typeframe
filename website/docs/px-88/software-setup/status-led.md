---
title: Status LED
sidebar_position: 4
description: Set up the status LED.
---

# Status LED Setup

If you've installed the status LED as described in the assembly section, here's what you need to do on the software side to get it working. I found a [helpful guide](https://www.richinfante.com/2021/08/11/raspberry-pi-gpio-status-lights) that goes over how this all works.

### 1. Test the LED

This will turn the LED on for 1 second and then turn it off again.

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

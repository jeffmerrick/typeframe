---
title: MU/TH/UR LED Matrix
sidebar_position: 5
description: Light up the LED Matrix
---

# MU/TH/UR LED Matrix

If you opted to include the [MU/TH/UR](https://en.wikipedia.org/wiki/MU_/_TH_/_UR) LED Matrix in your PS-85 build, you can set it up to display a simple blinking pattern similar to the one seen in the Alien movies.

The CharlePlex LED Matrix Bonnet is well documented over on [Adafruit's website](https://learn.adafruit.com/adafruit-charlieplex-bonnet/python-examples) if you want to explore more functionality.

### 1. Installation

1. Make the MU/TH/UR Directory

   ```bash
   cd /home/pi
   mkdir muthur
   cd muthur
   ```

2. Download the Script and Bash Files

   ```bash
   wget https://raw.githubusercontent.com/jeffmerrick/typeframe/main/ps-85/software/muthur/lights.py
   wget https://raw.githubusercontent.com/jeffmerrick/typeframe/main/ps-85/software/muthur/start.sh
   wget https://raw.githubusercontent.com/jeffmerrick/typeframe/main/ps-85/software/muthur/stop.sh
   chmod +x start.sh stop.sh
   ```

3. Create Virtual Environment

   ```bash
   python3 -m venv venv
   ```

4. Activate Virtual Environment

   ```bash
   source venv/bin/activate
   ```

5. Install Dependencies

   ```bash
   pip install adafruit-circuitpython-is31fl3731 adafruit-circuitpython-framebuf RPi.GPIO Pillow
   ```

6. Enable I2C (if not already enabled)

   ```bash
   sudo raspi-config nonint do_i2c 0
   ```

### 2. Controlling the Lights

#### Manually with the Python script

With the virtual environment activated:

```bash
python3 lights.py
```

```bash
python3 lights.py off
```

When you're done:

```bash
deactivate
```

#### With the Bash scripts

To start the lights script:

```bash
./start.sh
```

```bash
./stop.sh
```

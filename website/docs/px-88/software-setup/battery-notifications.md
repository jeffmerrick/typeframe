---
title: Battery Notifications (WIP)
sidebar_position: 8
description: Set up desktop battery notifications.
---

# Battery Notifications Setup

:::warning[WIP]
This is a work in progress; it doesn't seem to be currently working correctly.
:::

The idea here is to have a background service that monitors the battery status from the Power Management HAT and sends desktop notifications when the battery is low.

### 1. Copy battery status scripts from GitHub

```bash
# Download battery status scripts from GitHub
cd ~/Typeframe
wget https://raw.githubusercontent.com/jeffmerrick/typeframe/refs/heads/main/px-88/software/battery_monitor.py
wget https://raw.githubusercontent.com/jeffmerrick/typeframe/refs/heads/main/px-88/software/send_user_notification.sh
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

---
title: Keyboard Brightness Control
sidebar_position: 5
description: Set up keyboard brightness shortcuts.
---

# Keyboard Brightness Control Setup

## Configure Brightness Control Shortcuts

### 1. Copy Brightness Scripts

```bash
# Download launcher scripts from GitHub
mkdir ~/Typeframe
cd ~/Typeframe
wget https://raw.githubusercontent.com/jeffmerrick/typeframe/refs/heads/main/px-88/software/brightness_up.sh
wget https://raw.githubusercontent.com/jeffmerrick/typeframe/refs/heads/main/px-88/software/brightness_down.sh
```

Make sure these scripts are executable:

```bash
chmod +x ~/Typeframe/brightness_*.sh
```

### 2. Configure labwc Keyboard Shortcuts

Add the following keyboard shortcuts to your `~/.config/labwc/rc.xml` file, creating it if it doesn't already exist:

```xml
<?xml version="1.0"?>
<openbox_config xmlns="http://openbox.org/3.4/rc">
	<!-- Your existing configuration -->

	<keyboard>
		<!-- Hardware brightness keys (if configured in VIA) -->
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

- **Hardware Brightness Keys** (I configured as `MO(1) + PgUp` and `MO(1) + PgDown` in VIA)
- **Super + Up Arrow** - Increase brightness
- **Super + Down Arrow** - Decrease brightness

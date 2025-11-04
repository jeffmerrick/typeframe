# Typeframe Launcher Setup Guide

## Add the Launcher

### 1. Configure Boot Behavior

First set the Pi to boot to terminal instead of desktop:

```bash
sudo raspi-config nonint do_boot_behaviour B2
```

Boot options:

- **B1** - Console (text console, requiring user to login)
- **B2** - Console Autologin (text console, automatically logged in as 'pi' user)
- **B3** - Desktop (desktop GUI, requiring user to login)
- **B4** - Desktop Autologin (desktop GUI, automatically logged in as 'pi' user)

### 2. Install Required Dependencies

```bash
sudo apt-get install dialog
```

### 3. Copy Launcher Script

Download or copy the launcher file to the Typeframe directory in your home directory.

```bash
# Download launcher script from GitHub
cd ~/Typeframe
mkdir ~/launcher
wget [your-github-url]/launcher/launcher.sh

# Make the shell script executable
sudo chmod +x launcher.sh
```

### 4. Auto-launch on Boot

To automatically start the launcher when you boot, run this command to add the launcher to your `.bashrc`:

```bash
echo '~/Typeframe/launcher/launcher.sh' >> ~/.bashrc
```

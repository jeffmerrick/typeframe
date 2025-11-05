---
title: Boot Splash Screen
sidebar_position: 6
description: Customize the boot splash screen.
---

# Customize the Boot Splash Screen

The Raspberry Pi displays a boot splash screen during startup. You can customize this screen to show a Typeframe-themed image or any other image of your choice. You can also hide it completely in `raspi-config` if you prefer.

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
3. Download the Typeframe boot splash image from GitHub:
   ```bash
   sudo wget https://raw.githubusercontent.com/jeffmerrick/typeframe/main/px-88/software/images/typeframe-boot-simple.png -O splash.png
   ```
4. Refresh the Plymouth theme:
   ```bash
   sudo plymouth-set-default-theme -R pix
   ```

:::tip[Custom Images]
Images need to be rotated 90 degrees clockwise since the display isn't rotated until after boot. Match the screen size of 1280x400 pixels for best results.
:::

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

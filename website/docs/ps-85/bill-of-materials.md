---
sidebar_position: 1
description: Everything you need to build the PS-85.
---

# Bill of Materials

These are the components you'll need to build the PS-85. I've linked to the manufacturer, Adafruit, or Amazon, where I purchased most of my parts. Often the Amazon link will be a set of multiple variants/sizes, so you may be able to find the same part individually elsewhere. If you can find the parts at [Adafruit](https://www.adafruit.com/), please consider supporting them.

## Keyboard

| Component                                          | Notes                                                                                                                                                                                                                                                                                                                            | Link                                                                                                                                                                                                                                                     |
| -------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **KPrepublic BM43 PCB**                            | I had the 43 key version from my earlier experiments, but an ortholinear layout would match the aesthetic better. No idea if the [BM40](https://kprepublic.com/products/bm40-rgb-40-hot-swap-custom-mechanical-keyboard-pcb-qmk-underglow-type-c-planck) would be able to be swapped in easily, you'd at least need a new plate. | [kprepublic.com](https://kprepublic.com/collections/bm43/products/bm43a-bm43-43-keys-40-custom-mechanical-keyboard-pcb-programmed-numpad-layouts-qmk-firmware-with-rgb-bottom-underglow-alps-mx) [amazon.com](https://www.amazon.com/dp/B09163VRTV?th=1) |
| **Low Profile Plate Mount Stabilizers**            |                                                                                                                                                                                                                                                                                                                                  | [amazon.com](https://www.amazon.com/dp/B0F48MHHDR?th=1)                                                                                                                                                                                                  |
| **Keyboard Switches - Cherry MX Compatible (x43)** | I had medium profile switches but you could get a different profile, just make sure you get the matching stabilizers.                                                                                                                                                                                                            | [amazon.com](https://www.amazon.com/dp/B0F4Y41SLJ)                                                                                                                                                                                                       |
| **Keycaps - MX Compatible (x43)**                  | Flat profiles like DSA or XDA match the aesthetic well and are more flexible since they don't need to go in a specific row. But any MX compatible keycaps will work. Because of the 40% layout you'll need some odd sizes - buying a full 80% TKL set should get you what you need.                                              | [spkeyboards.com](https://spkeyboards.com/collections/keycap-sets/products/g20-semiotic-keycaps)                                                                                                                                                         |
| **Short USB-C to Micro-USB Cable**                 | I had one in my hoarded cables that I was able to use. It just has to reach from the keyboard to the Pi, so it can be quite short.                                                                                                                                                                                               | [adafruit.com](https://www.adafruit.com/product/3879)                                                                                                                                                                                                    |

## Screen

| Component                                      | Notes                                                                                                                    | Link                                                            |
| ---------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------ | --------------------------------------------------------------- |
| **Waveshare 7.9inch HDMI LCD**                 |                                                                                                                          | [waveshare.com](https://www.waveshare.com/7.9inch-hdmi-lcd.htm) |
| **DIY Cable Parts - Straight HDMI Plug**       | In my photos I have an angled version but the straight one should work fine.                                             | [adafruit.com](https://www.adafruit.com/product/3548)           |
| **DIY Cable Parts - Straight Mini HDMI Plug**  |                                                                                                                          | [adafruit.com](https://www.adafruit.com/product/3552)           |
| **DIY Cable Parts - 20 cm Ribbon Cable**       |                                                                                                                          | [adafruit.com](https://www.adafruit.com/product/3561)           |
| **USB DIY Slim Connector Shell - MicroB Plug** | You'll actually need two, for the power to the screen and Pi. Get some extras in case you mess up the soldering like me. | [adafruit.com](https://www.adafruit.com/product/1826)           |

## Computer

| Component                                        | Notes                                                                                                                    | Link                                                                                                     |
| ------------------------------------------------ | ------------------------------------------------------------------------------------------------------------------------ | -------------------------------------------------------------------------------------------------------- |
| **Raspberry Pi Zero 2W with Header**             |                                                                                                                          | [adafruit.com](https://www.adafruit.com/product/6008)                                                    |
| **microSD Card - 16GB+**                         | Get a quality SD card from a reputable brand.                                                                            | [adafruit.com](https://www.adafruit.com/product/6010) [amazon.com](https://www.amazon.com/dp/B08L5HMJVW) |
| **3.7V 6000mAh 906090 LiPo JST PH2.0mm Battery** |                                                                                                                          | [amazon.com](https://www.amazon.com/dp/B0CRDMZQ3Q)                                                       |
| **PowerBoost 1000C**                             |                                                                                                                          | [adafruit.com](https://www.adafruit.com/product/2465)                                                    |
| **KCD3 SPST On-Off Switch, 2 Pin**               |                                                                                                                          | [amazon.com](https://www.amazon.com/dp/B082PNCL61)                                                       |
| **Dupont Female to Male Extension Wires (x2)**   | Optional, but recommended to make disconnecting the power switch easier.                                                 | [adafruit.com](https://www.adafruit.com/product/1954) [amazon.com](https://www.amazon.com/dp/B0BRTHR2RL) |
| **~150Ω Resistor (2x)**                          |                                                                                                                          |                                                                                                          |
| **5mm Green LED**                                | For power indicator                                                                                                      | [adafruit.com](https://www.adafruit.com/product/4203) [amazon.com](https://www.amazon.com/dp/B0CR886L92) |
| **5mm Red LED**                                  | For low battery indicator                                                                                                | [adafruit.com](https://www.adafruit.com/product/4203) [amazon.com](https://www.amazon.com/dp/B0CR886L92) |
| **USB DIY Slim Connector Shell - MicroB Plug**   | You'll actually need two, for the power to the screen and Pi. Get some extras in case you mess up the soldering like me. | [adafruit.com](https://www.adafruit.com/product/1826)                                                    |

## LED Matrix (Optional)

| Component                                                         | Notes | Link                                                  |
| ----------------------------------------------------------------- | ----- | ----------------------------------------------------- |
| **2x20 Socket Riser Header for Raspberry Pi HATs and Bonnets**    |       | [adafruit.com](https://www.adafruit.com/product/4079) |
| **Adafruit CharliePlex LED Matrix Bonnet - 8x16 Warm White LEDs** |       | [adafruit.com](https://www.adafruit.com/product/4122) |

## Screws and More

I used self-tapping M3 screws to avoid having to use heat-set inserts. You may be able to use normal M3 screws instead, but I have not tested that. There is enough room for socket head caps, so other heads should work as well.

| Component                         | Quantity | Notes                                             | Link                                                    |
| --------------------------------- | -------- | ------------------------------------------------- | ------------------------------------------------------- |
| **M3 x 10mm Self-tapping Screws** | 2        |                                                   |                                                         |
| **M3 x 16mm Self-tapping Screws** | 5        |                                                   |                                                         |
| **M3 x 20mm Self-tapping Screws** | 2        |                                                   |                                                         |
| **M3 x 25mm Self-tapping Screws** | 3        |                                                   |                                                         |
| **M2.5 x 4mm Screws**             | 4        |                                                   |                                                         |
| **M2.5 x 8mm Screws**             | 4        |                                                   |                                                         |
| **M2.5 x 20mm Screws**            | 4        |                                                   |                                                         |
| **M2 x 4mm Screws**               | 4        |                                                   |                                                         |
| **Wire**                          |          |                                                   | [adafruit.com](https://www.adafruit.com/product/290)    |
| **Matte White PLA Filament**      |          | If you're painting, any light color will be fine. | [amazon.com](https://www.amazon.com/dp/B089S1HB8K?th=1) |
| **Heat Resistant Tape**           |          | For securing the battery in the case.             | [amazon.com](https://www.amazon.com/dp/B07F8TZZ4N)      |

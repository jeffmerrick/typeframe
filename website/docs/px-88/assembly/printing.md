---
title: Printing
sidebar_position: 1
description: Tips for printing.
---

# Printing the Case

![PX-88 Exploded Wireframe](../img/typeframe-px-88-exploded-wireframe.png)

## Overview

The case is split into parts that can be printed on an FDM printer with at least a 232mm x 130mm bed.

## Print Files

- [**3MF**](https://github.com/jeffmerrick/typeframe/tree/main/px-88/hardware/print-files/3MF) - Saved from PrusaSlicer with painted-on supports. These should be ready to slice and print.
- [**STL**](https://github.com/jeffmerrick/typeframe/tree/main/px-88/hardware/print-files/STL) - Raw STL files for each part. Parts with multiples are suffixed with `_x#`.

## Source Files

- [**STEP**](https://github.com/jeffmerrick/typeframe/tree/main/px-88/hardware/source-files/typeframe-px-88.step) - Step file of the entire assembly.
- [**F3Z**](https://github.com/jeffmerrick/typeframe/tree/main/px-88/hardware/source-files/typeframe-px-88.f3z) - I can't seem to export a .f3d because of some linked components. Hopefully, the .f3z will work if you want to dig into it. Fair warning that the timeline is a bit of a mess, though I did try to keep components named and somewhat organized.

## Tolerances and Parameters

Tolerances are pretty tight - you may be able to change Parameters in the Fusion 360 file to make adjustments. Alternatively, you may just want to do a bit of sanding to make parts fit if things are too snug. There are two parameters of interest:

- **`closeFitTolerance`**: This is the one I'm using almost everywhere. Default is 0.05mm.
- **`hingeShaftOffset`**: This determines the resistance level of the hinges. You want this quite tight as they will loosen over time. The default is -0.07mm.

Other parameters include screw sizes and wall thickness. However, modify these with caution as they may break the design if changed.

## Supports

The files print pretty well with minimal supports. But you will need supports for some parts, mostly for the larger port holes. The 3MF files have supports painted on.

## Print Settings

I printed on a Prusa MK4 with a 0.4mm nozzle and 0.2mm layer height using PLA filament. I had the best results with a smooth PEI sheet for bed adhesion since there are some large parts.

## Credits

The ingenious friction hinge design is from [KatDelgado on Printables](https://www.printables.com/model/658393-friction-hinge-mechanism). Thanks to the detailed information there, I was able to adapt it for this project.

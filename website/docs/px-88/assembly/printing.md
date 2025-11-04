---
title: Printing
sidebar_position: 1
description: Tips for printing.
---

# Printing the Case

## Overview

The case is split into parts that can be printed on a FDM printer with at least a 232mm x 130mm bed.

## Print Files

- **3MF** - Saved from PrusaSlicer with painted on supports. These should be ready to slice and print.
- **STL** - Raw STL files for each part. Parts with multiples are suffixed with `x`.

## Source Files

- **STEP** - Step file of the entire assembly.
- **F3Z** - I unfortunately can't export a .f3d because of some linked components that I couldn't figure out how to unlink. Hopefully the .f3z will work if you want to dig into it. Fair warning that the timeline is a bit of a mess, though I did try to keep components named and somewhat organized.

## Tolerances and Parameters

Tolerances are pretty tight - you may be able to change Parameters in the Fusion 360 file to make adjustments. Alternatively you may just want to do a bit of sanding to make parts fit if things are too snug. There are two parameters of interest:

- **`closeFitTolerance`**: This is the one I'm using almost everywhere. Default is 0.05mm.
- **`hingeShaftOffset`**: This determines the resistance level of the hinges. You want this quite tight as they will loosen over time. Default is -0.07mm.

Other parameters include screw sizes and wall thickness. However modify these with caution as they may break the design if changed.

## Supports

The files print pretty well with minimal supports. But you will need supports for some parts, mostly for the larger port holes. The 3MF files have supports painted on so you can see where I added them.

## Print Settings

I printed on a Prusa MK4 with a 0.4mm nozzle and 0.2mm layer height using PLA filament.

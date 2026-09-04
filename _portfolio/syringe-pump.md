---
title: "Syringe Pump"
excerpt: "DIY syringe pump capable of 1 uL / min flow rate up to 10 mL / min"
header:
  image: /assets/img/syringe_pump_3.jpg
  teaser: /assets/img/syringe_pump_3.jpg
gallery:
  - image_path: assets/img/syringe_pump_1.jpg
  - image_path: assets/img/syringe_pump_2.jpg
  - image_path: assets/img/syringe_pump_3.jpg
   
---

# Features

* Stepper-motor-driven linear actuation using a lead screw
* Compatible with 10 mL and 20 mL syringes
* Arduino Uno control system
* A4988 stepper motor driver
* Adjustable volumetric flow rate through software
* Microstepping for smoother and lower-vibration operation
* Latching start/pause button
* Limit switch to detect when the syringe reaches the empty position
* RGB LED status indicator
 * Green — Pump running
 * Yellow — Pump paused
 * Red — Syringe empty
* Custom 3D-printed mechanical components
* Enclosed electronics designed to reduce exposure to accidental liquid spills
* Compact aluminum-extrusion-based mechanical structure

# Off-the-Shelf Parts

* NEMA 17 Stepper Motor
* 250 mm Lead Screw — 2 mm Lead	
* 250 mm Lead Screw — 8 mm
* 1/4" × 8 mm Flexible Coupling
* 200 mm × 8 mm Linear Rod
* LM8UU Linear Bearing
* 2040 Aluminum Extrusion — 1 ft
* Arduino Uno
* A4988 Stepper Motor Driver
* MEAN WELL RQ-65D Power Supply
* Small Breadboard
* Panel-Mount Latching Push Button
* Limit Switch
* RGB Common-Cathode LED
* Wires, Resistors & Fasteners
* Syringe

# 3D Printed Parts

* Syringe Support
* Stepper Motor Mounting
* Moving Carriage
* Electronics Enclosure
* Mechanical Brackets

# CAD Model

<iframe src="https://vanderbilt643.autodesk360.com/shares/public/SH90d2dQT28d5b602811eb00a10bd4522857?mode=embed" width="640" height="480" allowfullscreen="true" webkitallowfullscreen="true" mozallowfullscreen="true"  frameborder="0"></iframe>

# Arduino Code

[View Arduino Code](/assets/syringe_pump.ino)

# Operating Instructions

1. Install and secure the 10 mL or 20 mL syringe.
2. Set the desired flow rate in the Arduino code and upload it.
3. Power on the pump and press the latching button to start.
4. Green: running | Yellow: paused | Red: empty
5. The pump automatically stops when the limit switch detects the end of the syringe.
6. Unplug the system before replacing the syringe or servicing the electronics.

{% include gallery caption="Syringe Pump Gallery" %}

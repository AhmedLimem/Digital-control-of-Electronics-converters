# Digital Control of Power Electronics Converters via AVR Microcontrollers

## About
This is the work of my 2nd-engineering-year project about designing and programming embedded control systems for power electronics converters. 

More precisely, it aims to elaborate digital control (generation and processing of signals) of power electronics converters (Boost converter, Rectifier, single-phase and three-phase Dimmers) via Atmel AVR microcontrollers (ATmega328 and ATmega2560) with Arduino Board. \
The major part of this project is C/C++ programming for embedded systems.

## What It Is (more details)

This project involves modifying existing commands in analog mode of power electronics converters by a digital control with display. 

These commands are intended for:
- Chopper Voltage Lift
- Single-phase rectifier
- Single-phase and three-phase dimmer

For its AC converters, it is necessary to design a stable sector synchronization signal and then establish the phase delay control.
An offset of 120 ° then 240 ° will allow us to accurately determine the zero crossing times of the other synchronization phases.

#### For its embedded programming, AVR microcontrollers are used with all its features of GPIO, Timers and Interrupts, under the Arduino environment.

### Key words: C/C++ programming, Embedded systems, Digital control and signal processing, AVR microcontroller, GPIO, Timers, internal and external Interrupts.

## How To Use

### Required Tools
- Arduino Boards (arduino uno / arduino mega)
- Breadboard, Potentiometer and jumper wires
- Oscilloscope
- 16x2 LCD Display with I2C Interface

### Instructions

1/ Install Arduino IDE software

2/ Add Timers and Display libraries in Arduino IDE:
- Go to "Sketch" menu
- Then menu "Include Library"
- Click "Add .ZIP Library.."

Note: All needed Libraries are in the folder "arduino_libraries"

## Collaborators

### Ahmed LIMEM 
Electrical Engineering student (ahmed.limem@etudiant-enit.utm.tn)
### Tarek BEN DAHOU
Electrical Engineering student

## Contributing
Mr. Férid KOURDA - Supervisor, professor and director of Electrical Systems Laboratory (LSE), ENIT

## Copyright & Licensing
All the work and program code are published under Open-Source License but it is necessary to reference it for any use.

In particular, this project is held in the Electrical Systems Laboratory (LSE) at the National Engineering School of Tunis (ENIT), under the directions of its director Mr. Férid KOURDA and using its equipments.

References \
While this project is being prepared, it was inspired form some sources. All of them are mentioned at the end of my report.

## Contact
Don't hesitate to contact me for any clarification about this project :)

##### Best Regards

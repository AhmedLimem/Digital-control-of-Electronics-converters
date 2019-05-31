# About this work

## About
This is the work of my 2nd-engineering-year project about designing and programming embedded control systems for power electronics converters. 

More precisely, it's about Digital control (generation and processing of signals) of power electronics converters (Boost converter, Rectifier, single-phase and three-phase Dimmers) via Atmel AVR microcontrollers (ATmega328 and ATmega2560) with Arduino Board.

## What It Is (more details)

This project involves modifying existing commands of  in analog mode by a digital control with display. 

These commands are intended for:
- Chopper Voltage Lift.
- Single-phase rectifier.
- Single-phase and three-phase dimmer.

For its AC converters, it is necessary to design a stable sector synchronization signal and then establish the phase delay control.
An offset of 120 ° then 240 ° will allow us to accurately determine the zero crossing times of the other synchronization phases.

For its embedded programming, 

Key words: C/C++, embedded systems, digital control and signal processing, avr microcontroller, gpio, timers, intern and extern interrupts.

## How To Use

### Required Tools

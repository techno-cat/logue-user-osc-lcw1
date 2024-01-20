# LCW-1
**Notice: This project is working in progress.**

Lazy Cat Works presents.  
Dual osc module for NTS-1,  
can use as `USER OSCILLATORS`.

# Parameters
- shape (NTS-1: A, other: Shape)  
Detune of Sub osc. (Center is neutral.)
- shiftshape (NTS-1: B, other: Shift+Shape)  
Main/Sub mixing ratio.
- LFO destinaton  
-> Main/Sub mixing ratio.
- parameter 1  
Main osc type:
    1. Pulse (Duty:1/2)
    1. Pulse (Duty:1/4)
    1. Pulse (Duty:1/8)
    1. Pulse (Duty:1/16)
    1. Pulse (Duty:1/32)
    1. Pulse (Duty:1/64)
    1. Saw
    1. Tri
- parameter 2  
Sub osc type:
    1. Pulse (Duty:1/2)
    1. Pulse (Duty:1/4)
    1. Pulse (Duty:1/8)
    1. Pulse (Duty:1/16)
    1. Pulse (Duty:1/32)
    1. Pulse (Duty:1/64)
    1. Saw
    1. Tri

# How to build
1. Clone (or download) and setup [logue-sdk](https://github.com/korginc/logue-sdk).
1. Clone (or download) this project.
1. Change `PLATFORMDIR` of Makefile according to your environment.

# LICENSE
Copyright 2024 Tomoaki Itoh
This software is released under the MIT License, see LICENSE.txt.

# AUTHOR
Tomoaki Itoh (techno.cat.miau@gmail.com)  
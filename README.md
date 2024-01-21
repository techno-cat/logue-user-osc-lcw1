# LCW-1

Lazy Cat Works presents.  
Dual osc module for NTS-1,  
can use as `USER OSCILLATORS`.

# Parameters

- shape (NTS-1: A, other: Shape)  
Detune of Sub osc. (Center is neutral.)
- shiftshape (NTS-1: B, other: Shift+Shape)  
Main/Sub mixing ratio.
- parameter 1  
Main osc type (1..8)<ol type="1">
    <li>Pulse (Duty: 1/2)</li>
    <li>Pulse (Duty: 1/4)</li>
    <li>Pulse (Duty: 1/8)</li>
    <li>Pulse (Duty: 1/16)</li>
    <li>Pulse (Duty: 1/32)</li>
    <li>Pulse (Duty: 1/64)</li>
    <li>Saw</li>
    <li>Tri</li></ol>
- parameter 2  
Sub osc type (1..8)<ol type="1">
    <li>Pulse (Duty: 1/2)</li>
    <li>Pulse (Duty: 1/4)</li>
    <li>Pulse (Duty: 1/8)</li>
    <li>Pulse (Duty: 1/16)</li>
    <li>Pulse (Duty: 1/32)</li>
    <li>Pulse (Duty: 1/64)</li>
    <li>Saw</li>
    <li>Tri</li></ol>
- parameter 3  
LFO destinaton (1..2)<ol type="1">
    <li>Detune of Sub osc</li>
    <li>Main/Sub mixing ratio</li></ol>

# How to build
1. Clone (or download) and setup [logue-sdk](https://github.com/korginc/logue-sdk).
1. Clone (or download) this project.
1. Change `PLATFORMDIR` of Makefile according to your environment.

# LICENSE
Copyright 2024 Tomoaki Itoh
This software is released under the MIT License, see LICENSE.txt.

# AUTHOR
Tomoaki Itoh (techno.cat.miau@gmail.com)
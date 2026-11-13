An IoT-based Smart Energy Conservation System uses sensors and internet connectivity to monitor, 
control, and optimize energy consumption in real time. It helps reduce electricity wastage, improve 
energy efficiency, automate appliances, and lower electricity costs through smart monitoring and remote control.

---

## IoT Based Smart Energy Conservation System

Mini project - Dept. of EEE, Bapuji Institute of Engineering and Technology, Davanagere (2025)

---

## Overview

An ESP32-based system that switches classroom light and fan automatically using 
an IR sensor (occupancy) and an LDR sensor (ambient light). 
A built-in web dashboard shows live status on a phone over Wi-Fi. 

Logic: person present AND room dark -> light + fan ON. Room vacant for 10 s -> OFF.

---

## Hardware 

---

|Component | ESP32 Pin|
| --- | --- |
| IR sensor OUT | GPIO 2 (D2) |
| LDR module DO |  GPIO 12 (D12) |
| Relay IN (fan) | GPIO 5 (D5) |
| LED / bulb | GPIO 4 (D4) |

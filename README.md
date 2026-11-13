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

|Component | ESP32 Pin|
| --- | --- |
| IR sensor OUT | GPIO 2 (D2) |
| LDR module DO |  GPIO 12 (D12) |
| Relay IN (fan) | GPIO 5 (D5) |
| LED / bulb | GPIO 4 (D4) |

Relay module powered from 5 V (VIN), sensors from 3.3 V, common GND.

---

# Setup

1. Install Arduino IDE and the ESP32 board package . 
2. Open smart_energy_conservation.ino . 
3. Set WIFI_SSID and WIFI_PASS . 
4. Select board ESP32 Dev Module , upload. 
5. Open Serial Monitor (115200), note the IP, open it in a phone browser.

---


# Tuning

If the behaviour is reversed, flip IR_ACTIVE_LOW , or LDR_DARK_IS_HIGH RELAY_ACTIVE_LOW at the top of the code.

---

# Future scope

ML/AI automation, mobile app control, solar integration, voice assistant.

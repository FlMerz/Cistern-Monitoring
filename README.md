# Cistern-Monitoring
Monitoring System for different kind of cisterns

## Contents
 - [How it works](#how-it-works)
 - [Components](#Components)
 - [Contributors](#contributions-and-thanks)

## How It Works
With an ultrasonic sensor, which is placed above the water tank, the distance to the water surface is measured. Based on the previously entered measurements of the tank in the control system, the filling quantity is automatically calculated in liters and percent. 
These data are then graphically processed and displayed on a display. Optionally, the system can be connected to the Internet via WLAN. This data can then be accessed worldwide via the Cistern Monitoring Cloud using a progressive web app (https://github.com/SwonVIP/cistern-pwa). 

## Components

### Base Module
- 1.8" TFT Module
- WiFi 802.11 b/g/n (802.11n up to 150 Mbps)
- Ultra Low Power Modue (No quiescent current)
- LoRa transceiver (optional)
- Can be used with battery or with a power supply
- Wide Range of power supply support (3.3-12V)

### Sensor Module (optional)
- waterproof
- can send data via LoRa up to 2 km

### Cistern-Monitoring PWA App (optional)
- development are ongoing here (https://github.com/SwonVIP/cistern-pwa)

## Contributors
Many thanks to 
Manuel,
Lukas,
Sven (https://github.com/SwonVIP)

# Cistern-Monitoring
Monitoring System for different kind of cisterns

## Contents
 - [How it works](#how-it-works)
 - [Components](#Components)
 - [Contributors](#contributions-and-thanks)

## How It Works
With an ultrasonic sensor, which is placed above the water tank, the distance to the water surface is measured. Based on the previously entered measurements of the tank in the control system, the filling quantity is automatically calculated in liters and percent. 
These data are then graphically processed and displayed on a display. Optionally, the system can be connected to the Internet via WLAN. This data can then be accessed worldwide via the Cistern Monitoring Cloud using a progressive web app (https://github.com/SwonVIP/cistern-pwa). 
### Use Case 1
Waterlevel should be displayed directly on the tank
![UseCase2](https://user-images.githubusercontent.com/53577414/123510198-7871a500-d67a-11eb-9762-9ee3723824da.png)

### Use Case 2
Tank is outside of the house and waterlevel should be shown in inside the house
![UseCase1](https://user-images.githubusercontent.com/53577414/123510196-77407800-d67a-11eb-880e-91d9780e54db.png)

### Use Case 3
Waterlevel information should be retrieved all over the world
![UseCase3](https://user-images.githubusercontent.com/53577414/123510199-7871a500-d67a-11eb-915a-ef8011b7b183.png)

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

## Impressions
### Base Station
![base module_config](https://user-images.githubusercontent.com/53577414/123509885-6e4ea700-d678-11eb-9bab-d060301b7269.png)
![base_station-1](https://user-images.githubusercontent.com/53577414/123509888-70186a80-d678-11eb-9461-dba977ae32a6.png)

### Configuration
![wifi_selection_small](https://user-images.githubusercontent.com/53577414/123509631-dc926a00-d676-11eb-8b7e-fe4994de1a33.png)
![cistern_credentials_small](https://user-images.githubusercontent.com/53577414/123509632-dd2b0080-d676-11eb-947a-ed56a5360bc9.png)
![config_ssid_small](https://user-images.githubusercontent.com/53577414/123509633-dd2b0080-d676-11eb-8d02-ec8cc4c2faaf.png)
![save_screen_small](https://user-images.githubusercontent.com/53577414/123509634-ddc39700-d676-11eb-97ab-e1c0acc8067b.png)
![start_wifi_manager_small](https://user-images.githubusercontent.com/53577414/123509635-ddc39700-d676-11eb-969a-f99d631ae56f.png)

### Sensor Module
Prototyp for long term tests:

![IMG-20210522-WA0009](https://user-images.githubusercontent.com/53577414/123509970-0056af80-d679-11eb-9b74-b49b226ecaf5.jpg)

## Contributors
Many thanks to 
Manuel,
Lukas,
Sven (https://github.com/SwonVIP)

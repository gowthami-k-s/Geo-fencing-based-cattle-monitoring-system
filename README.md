# Geo-Fencing Based Cattle Monitoring System Using LoRa

## Project Overview

The Geo-Fencing Based Cattle Monitoring System using LoRa is an IoT-based project designed to monitor cattle movement and provide alerts when cattle move beyond predefined geographical boundaries.

The system uses GPS to track cattle location, LoRa for long-range wireless communication, GSM to send SMS notifications, and a buzzer for local alerts.

## Hardware Components

### Transmitter Unit
- ESP32 (ESP-WROOM-32)
- LoRa Module (SX1278)
- GPS Module (NEO-6M)

### Receiver Unit
- ESP32 (ESP-WROOM-32)
- LoRa Module (SX1278)
- GSM Module (SIM900A / SIM800L)
- Buzzer

## Working Principle

1. GPS obtains the cattle's latitude and longitude.
2. ESP32 processes the GPS coordinates.
3. Location data is transmitted to the receiver through LoRa.
4. Receiver calculates the distance from the predefined reference location.
5. Based on the distance, the system generates SMS notifications and buzzer alerts.

## Geo-Fencing Conditions

| Distance | Status | Action |
|---|---|---|
| Up to 5 meters | SAFE | No SMS or buzzer |
| Above 5 to 7 meters | IN RANGE | SMS notification |
| Above 7 to 10 meters | WARNING | Location SMS every 30 seconds; buzzer OFF |
| Above 10 meters | OUT OF RANGE | Live location SMS and buzzer ON |

## Technologies Used
- Embedded C / Arduino Programming
- ESP32
- GPS
- LoRa Communication
- GSM Communication
- IoT

## Applications
- Livestock monitoring
- Cattle safety and tracking
- Smart agriculture
- Prevention of cattle loss

## Future Enhancements
- Multiple cattle monitoring
- Mobile application integration
- Cloud-based location monitoring
- Solar-powered cattle collar

## Author
K S Gowthami

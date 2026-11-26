# Geo-Fencing Based Cattle Monitoring System Using LoRa

## Project Overview

The Geo-Fencing Based Cattle Monitoring System is an IoT-based project designed to monitor cattle movement and detect when cattle move outside a predefined boundary.

The system uses ESP32, GPS, LoRa communication, RSSI-based distance estimation, and GSM technology to monitor cattle and send SMS alerts.

GPS provides the cattle's location coordinates, while LoRa RSSI is used to estimate the distance between the transmitter and receiver.

## Objectives

- Monitor cattle movement within a predefined boundary.
- Estimate distance using LoRa RSSI values.
- Obtain cattle location using GPS coordinates.
- Send SMS notifications when cattle approach or cross the boundary.
- Activate a buzzer when cattle move outside the boundary.

## Hardware Components

- ESP32 Microcontroller – Transmitter
- ESP32 Microcontroller – Receiver
- LoRa Module (433 MHz)
- GPS Module
- GSM Module (SIM900A)
- Buzzer
- Power Supply

## System Architecture

### 1. Transmitter

The transmitter consists of an ESP32, GPS module, and LoRa module.

- GPS obtains the latitude and longitude of the cattle.
- ESP32 reads the GPS coordinates.
- Coordinates are transmitted to the receiver using LoRa communication.

### 2. Receiver

The receiver consists of an ESP32, LoRa module, GSM module, and buzzer.

- Receives GPS coordinates through LoRa.
- Reads the RSSI value of the received LoRa packet.
- Estimates the distance using an RSSI-based formula.
- Identifies the cattle's zone.
- Sends SMS alerts with Google Maps location links.
- Activates the buzzer when cattle move outside the boundary.

## Working Principle

1. The GPS module obtains the cattle's current latitude and longitude.
2. The transmitter ESP32 sends the coordinates through LoRa.
3. The receiver ESP32 receives the transmitted packet.
4. The receiver reads the LoRa RSSI value.
5. RSSI is used to estimate the distance between the transmitter and receiver.
6. The system identifies the cattle's zone based on the estimated distance.
7. GSM sends SMS notifications with GPS location links when required.
8. The buzzer turns ON when the cattle crosses the defined boundary.

## Zone Classification

| Estimated Distance | Zone | Action |
|---|---|---|
| 0–5 metres | SAFE | No SMS, buzzer OFF |
| Above 5–7 metres | SAFE BOUNDARY | SMS notification on entering zone |
| Above 7–10 metres | WARNING | Warning SMS on entering zone |
| Above 10 metres | OUT OF BOUNDARY | Buzzer ON and SMS every 30 seconds |

## Technologies Used

- Embedded C
- ESP32
- GPS
- LoRa Communication
- RSSI
- GSM
- Arduino IDE

## Software Requirements

- Arduino IDE
- ESP32 Board Package
- LoRa Library
- TinyGPS++ Library

## Applications

- Smart Agriculture
- Livestock Monitoring
- Cattle Movement Tracking
- Farm Boundary Monitoring

## Future Enhancements

- Improve RSSI distance calibration for better accuracy.
- Add a real-time cattle monitoring dashboard.
- Support monitoring multiple cattle.
- Improve alert reliability and location tracking.

## Note

RSSI-based distance estimation is approximate and depends on antenna orientation, obstacles, environmental conditions, and calibration.

GPS is used to obtain location coordinates, while RSSI is used for distance estimation.

---

**Project Domain:** Embedded Systems and Internet of Things (IoT)

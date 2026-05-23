# Ultrasonic Liquid Level Monitoring with HC-05 Bluetooth and Arduino UNO 🌊

![Arduino UNO](https://img.shields.io/badge/Arduino%20UNO-00979C.svg?style=for-the-badge&logo=arduino&logoColor=white)
![HC-05 Bluetooth](https://img.shields.io/badge/HC--05%20Bluetooth-007bff.svg?style=for-the-badge&logo=bluetooth)
![C++](https://img.shields.io/badge/C%2B%2B-00599C.svg?style=for-the-badge&logo=c%2b%2b&logoColor=white)

## 📝 Overview
This project implements a simple IoT liquid-level monitoring device. It measures the distance between a sensor mounted above a container and the liquid surface, converting that distance into liquid height and percentage. The data is then transmitted wirelessly to a smartphone terminal app via Bluetooth for real-time monitoring and control.

This project was developed for the **Internet of Things Laboratory** course at IUST (Department of Computer Engineering), instructed by Dr. Eisa ZarePoor (Fall 2025).

### 👥 Team Members
- Nazanin Mahmoodi (401522097)
- Mohammad Mahdi Sharaf-bayani (401521372)

## ✨ Key Features
- **Real-Time Monitoring:** Continuous monitoring of liquid levels using ultrasonic sensing.
- **Data Conversion:** Automatically converts raw distance measurements into water height and percentage (0-100% clamped safely).
- **Wireless Telemetry:** Live data display on a smartphone via a Bluetooth connection.
- **Remote Commands:** Send user commands through the terminal app for calibration and control.
- **Edge-Node Architecture:** Incorporates sensing, processing, communication, and application layers.

## 🛠️ Hardware Components
- **Arduino UNO** (Main microcontroller)
- **HC-SR04 Ultrasonic Sensor** (Distance measurement)
- **HC-05 Bluetooth Module** (Wireless communication)
- **Breadboard & Jumper Wires**
- **Smartphone** (With a Bluetooth Terminal App installed)

## 💻 Software & Technologies
- **Arduino IDE** (For writing and uploading C++ code)
- **Bluetooth Terminal App** (Android/iOS application for receiving telemetry)
- **SoftwareSerial / UART** (For serial communication between Arduino and HC-05)
- **Bluetooth Classic SPP** (Serial Port Profile)

## 🔌 Wiring & Circuit
- **HC-05 TXD** ➔ Arduino Digital Pin 2 (SoftwareSerial RX)
- **HC-05 RXD** ➔ Arduino Digital Pin 3 (SoftwareSerial TX) *via a voltage divider to safely step down the 5V TX to 3.3V*.
- **HC-05 EN/KEY** ➔ Disconnected (for normal communication mode).

## 🚀 Setup and Installation
1. **Hardware Assembly:** Connect the components according to the wiring guide above. Mount the HC-SR04 sensor at the top of your liquid container facing downward.
2. **Software Upload:** Open the source code in the Arduino IDE, compile, and upload it to the Arduino UNO.
3. **Bluetooth Pairing:** 
   - Turn on Bluetooth on your smartphone and pair it with the `HC-05` module (default PIN is usually `0000` or `1234`).
4. **App Configuration:** Open your Bluetooth Terminal App, connect to the paired HC-05, and view the real-time distance and percentage logs.

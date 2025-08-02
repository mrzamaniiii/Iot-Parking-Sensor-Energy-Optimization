# IoT Parking Sensor – Energy Optimization & Simulation

This project focuses on designing and analyzing an ultra-low power IoT parking sensor system using:
- **ESP32 microcontroller**
- **HC-SR04 ultrasonic sensor**
- **ESP-NOW protocol for transmission**

All data is collected, simulated, and analyzed in a Wokwi-based environment to assess energy consumption, battery lifetime, and network optimization.

## 🔧 Features

- **Distance-based vehicle detection**
- **Wireless transmission via ESP-NOW**
- **Deep sleep to reduce energy consumption**
- **Battery lifetime estimation per operational cycle**
- **Optimal sink placement for maximizing sensor network lifetime**
- **Wokwi JSON simulation diagram included**

## 📁 Project Structure

| File | Description |
|------|-------------|
| `sketch.ino` | Arduino sketch for ESP32 node (sensor + transmitter) |
| `Deep_Sleep.ipynb` | Power profiling during deep sleep |
| `Transmission_Power.ipynb` | Power profiling during data transmission |
| `Read_Sensor.ipynb` | Power profiling during ultrasonic distance measurement |
| `Optimizing.ipynb` | Lifetime optimization for wireless sensor network |
| `diagram.json` | Wokwi hardware setup (ESP32 + HC-SR04) |
| `Challenge 1 Report.pdf` | Full project report (methods, calculations, plots) |
| `Exercise.pdf` | Sink placement analysis and optimization |
| `wokwi-project.txt` | Link to online Wokwi simulation |

## 🧠 Key Results

- **Battery Lifetime**: Up to **13.9 hours** with sensor-only operations
- **Energy Consumption**: ~7.51 J per full sensing-transmitting cycle
- **Optimal Sink Placement**: Found via analytical and Python (Powell algorithm) methods
- **Simulation**: Fully replicable via [Wokwi link](https://wokwi.com/projects/425241064895099905)

## 📉 Battery Lifetime Estimation

> Lifetime is derived from measured power (mW) × duration (s) and calculated energy (J).

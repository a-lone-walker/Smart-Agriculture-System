# 🌱 Smart Agriculture System using ESP8266 & IoT

This project monitors environmental parameters crucial to plant growth using IoT-based smart farming techniques. Real-time data is visualized on the ThingSpeak platform.

## 🚀 Features
- Live monitoring of Soil Moisture, Temperature, Humidity & Light Intensity
- Wi-Fi enabled updates using ESP8266
- Data displayed on a real-time dashboard (ThingSpeak)
- Simple and cost-effective design for agriculture applications

## 📦 Components
- ESP8266 NodeMCU
- Soil Moisture Sensor (Analog)
- DHT11 or DHT22 Sensor
- LDR Sensor
- Jumper wires and Breadboard

## 🧠 How it Works
- Sensors collect environmental data.
- ESP8266 reads the sensor data and uploads it to ThingSpeak.
- The user can view it remotely via a custom dashboard or ThingSpeak channel.

## 📷 Project Demo
![Setup](Images/setup_diagram.png)
![Output](Images/output_screenshot.png)

## 🛠️ Setup Instructions
1. Install Arduino IDE and add ESP8266 board.
2. Upload the Arduino code from the `Arduino_Code/` folder.
3. Create a ThingSpeak account and get your API Key.
4. Connect the sensors to ESP8266 as described in the code.
5. Power it on and monitor data online.

## 🧪 Future Scope
- Auto irrigation using a relay and water pump.
- Mobile alerts when moisture goes low.
- Predictive analytics for crop health.

## 📜 License
This project is under MIT License. Feel free to use, modify, and share.


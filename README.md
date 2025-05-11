🌱 Smart Agriculture System using ESP8266 & IoT
An IoT-based smart farming solution to monitor environmental parameters critical for plant growth, such as soil moisture, temperature, humidity, and light intensity. The system uses the ESP8266 NodeMCU to collect sensor data and upload it to the ThingSpeak platform. A custom webpage fetches and displays real-time data with alerts for critical conditions.
 
🚀 Features

Live Monitoring: Real-time tracking of soil moisture, temperature, humidity, and light intensity.
Wi-Fi Enabled: Uses ESP8266 for wireless data updates to ThingSpeak.
Custom Dashboard: A responsive webpage displays sensor data and alerts for low soil moisture or high temperature.
Cost-Effective: Simple design using affordable components for agricultural applications.

📦 Components

ESP8266 NodeMCU
Soil Moisture Sensor (Analog)
DHT11 or DHT22 Sensor (Temperature & Humidity)
LDR Sensor (Light Intensity)
Jumper Wires & Breadboard
USB Cable for power and programming

🧠 How It Works

Sensor Data Collection: Sensors measure environmental parameters.
Data Processing: ESP8266 reads sensor data and processes it.
Cloud Upload: Data is sent to a ThingSpeak channel via Wi-Fi.
Custom Dashboard: A webpage fetches data using ThingSpeak's Read API Key and displays it with alerts for critical conditions (e.g., soil moisture < 30%).

🛠️ Setup Instructions
Prerequisites

Arduino IDE with ESP8266 board support
ThingSpeak Account (Sign up at thingspeak.com)
Libraries: ESP8266WiFi, ThingSpeak, DHT sensor library

Hardware Setup

Connect Sensors:
Soil Moisture Sensor: Connect to A0 (Analog pin).
DHT11/DHT22: Connect data pin to D4.
LDR: Connect to A0 (or another analog pin if needed, adjust code accordingly).
Power sensors with 3.3V or 5V as per their specifications.



Software Setup

Install Arduino IDE:
Download and install from arduino.cc.
Add ESP8266 board support via Boards Manager (URL: http://arduino.esp8266.com/stable/package_esp8266com_index.json).


Install Libraries:
Go to Sketch > Include Library > Manage Libraries and install:
ESP8266WiFi
ThingSpeak
DHT sensor library by Adafruit




Configure Arduino Code:
Open Arduino_Code/SmartAgriculture.ino.
Update the following:
ssid: Your Wi-Fi network name.
password: Your Wi-Fi password.
thingSpeakApiKey: Your ThingSpeak Write API Key.
channelID: Your ThingSpeak Channel ID.




Upload Code:
Connect ESP8266 to your computer via USB.
Select NodeMCU 1.0 (ESP-12E Module) in Arduino IDE.
Upload the code.


Set Up ThingSpeak:
Create a channel with four fields (Soil Moisture, Temperature, Humidity, Light Intensity).
Note the Channel ID, Write API Key, and Read API Key.


Configure Webpage:
Open website/script.js.
Update channelID and readApiKey with your ThingSpeak Channel ID and Read API Key.
Host the website/ folder on a server or GitHub Pages.



Viewing Data

Serial Monitor: Open at 115200 baud to verify sensor readings.
ThingSpeak: Access your channel for raw data.
Custom Dashboard: Open website/index.html in a browser (after hosting) to view real-time data and alerts.

📷 Project Demo
Coming soon: Add images to the images/ folder and update this section.
🧪 Future Scope

Automated Irrigation: Add a relay and water pump for automatic watering.
Mobile Notifications: Send alerts via email or SMS.
Predictive Analytics: Use machine learning for crop health predictions.

📜 License
This project is licensed under the MIT License. Feel free to use, modify, and share.
🙌 Contributing
Contributions are welcome! Fork the repository, make changes, and submit a pull request.
📬 Contact
For questions or feedback, open an issue or reach out via GitHub.

Built with 🌱 for sustainable agriculture.

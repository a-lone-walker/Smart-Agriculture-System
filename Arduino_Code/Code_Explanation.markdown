# 📝 Code Explanation for Smart Agriculture System

This document explains the Arduino code (`SmartAgriculture.ino`) for the **Smart Agriculture System using ESP8266 & IoT**. The system monitors **soil moisture**, **temperature**, **humidity**, and **light intensity** using an ESP8266 NodeMCU, uploading data to ThingSpeak for real-time visualization. Since the ESP8266 has only one analog pin (A0), the Soil Moisture Sensor and LDR share A0 using **time-multiplexing**, controlled by digital pins D5 and D6. This explanation details each section of the code, its purpose, and how it ensures accurate sensor readings.

## 📚 Libraries
```cpp
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>
```
- **`ESP8266WiFi.h`**: Enables Wi-Fi connectivity on the ESP8266, allowing the NodeMCU to connect to a Wi-Fi network and send data to the internet.
- **`ThingSpeak.h`**: Provides functions to communicate with the ThingSpeak IoT platform, where sensor data is uploaded and visualized.
- **`DHT.h`**: Interfaces with the DHT11 (or DHT22) sensor to read temperature and humidity data via a digital pin.

**Why**: These libraries are essential for Wi-Fi communication, cloud data storage, and sensor interfacing.

## 🔧 Configuration
```cpp
const char* ssid = "YOUR_WIFI_SSID"; // Replace with your Wi-Fi SSID
const char* password = "YOUR_WIFI_PASSWORD"; // Replace with your Wi-Fi password
const char* thingSpeakApiKey = "YOUR_THINGSPEAK_API_KEY"; // Replace with your ThingSpeak Write API Key
const unsigned long channelID = YOUR_CHANNEL_ID; // Replace with your ThingSpeak Channel ID
```
- **`ssid` and `password`**: Credentials for your Wi-Fi network. Replace with your network’s details to connect the ESP8266.
- **`thingSpeakApiKey`**: The Write API Key from your ThingSpeak channel, used to authenticate data uploads.
- **`channelID`**: The unique ID of your ThingSpeak channel, specifying where data is sent.

**Why**: These constants configure the network and ThingSpeak connection, ensuring the ESP8266 can communicate with the cloud.

## 📍 Pin Definitions
```cpp
#define SOIL_MOISTURE_POWER D5 // Power control for Soil Moisture Sensor
#define LDR_POWER D6          // Power control for LDR
#define DHT_PIN D4            // Digital pin for DHT11 sensor
#define ANALOG_PIN A0         // Shared analog pin for Soil Moisture and LDR
#define DHT_TYPE DHT11        // DHT sensor type (DHT11 or DHT22)
```
- **`SOIL_MOISTURE_POWER (D5)`**: Digital pin (GPIO 14) used to control power to the Soil Moisture Sensor’s VCC, enabling it only when reading its signal.
- **`LDR_POWER (D6)`**: Digital pin (GPIO 12) used to control power to the LDR’s voltage divider, enabling it only when reading its signal.
- **`DHT_PIN (D4)`**: Digital pin (GPIO 2) connected to the DHT11’s DATA pin for temperature and humidity readings.
- **`ANALOG_PIN (A0)`**: The ESP8266’s only analog input pin, shared by the Soil Moisture Sensor (AOUT) and LDR (voltage divider junction).
- **`DHT_TYPE`**: Specifies the DHT sensor model (DHT11 or DHT22). Change to `DHT22` if using that sensor.

**Why**: These definitions map hardware pins to code, ensuring correct sensor interfacing. D5 and D6 are critical for time-multiplexing to distinguish A0 readings.

## 🛠️ Global Objects and Variables
```cpp
DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient client;
unsigned long lastUpdate = 0;
const long updateInterval = 60000; // Update every 60 seconds
```
- **`dht`**: An instance of the `DHT` class, initialized with `DHT_PIN` (D4) and `DHT_TYPE` (DHT11), used to read temperature and humidity.
- **`client`**: A `WiFiClient` object for establishing a TCP connection to ThingSpeak’s servers.
- **`lastUpdate`**: Tracks the last time data was sent to ThingSpeak (in milliseconds).
- **`updateInterval`**: Sets the interval (60 seconds) between data uploads to avoid exceeding ThingSpeak’s free tier limits (15-second minimum).

**Why**: These variables manage sensor communication, network connections, and timing to ensure periodic, reliable data collection.

## ⚙️ Setup Function
```cpp
void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(SOIL_MOISTURE_POWER, OUTPUT);
  pinMode(LDR_POWER, OUTPUT);
  digitalWrite(SOIL_MOISTURE_POWER, LOW);
  digitalWrite(LDR_POWER, LOW);
  dht.begin();
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected");
  ThingSpeak.begin(client);
}
```
- **`Serial.begin(115200)`**: Initializes serial communication at 115200 baud for debugging and monitoring sensor data.
- **`delay(10)`**: Brief pause to ensure stable serial initialization.
- **`pinMode(SOIL_MOISTURE_POWER, OUTPUT)` and `pinMode(LDR_POWER, OUTPUT)`**: Configure D5 and D6 as output pins to control sensor power.
- **`digitalWrite(SOIL_MOISTURE_POWER, LOW)` and `digitalWrite(LDR_POWER, LOW)`**: Set both sensors off initially to prevent A0 interference.
- **`dht.begin()`**: Initializes the DHT11 sensor for temperature and humidity readings.
- **`WiFi.begin(ssid, password)`**: Starts Wi-Fi connection using the provided credentials.
- **`while (WiFi.status() != WL_CONNECTED)`**: Loops until connected, printing dots to indicate progress.
- **`Serial.println("\nWi-Fi connected")`**: Confirms successful connection.
- **`ThingSpeak.begin(client)`**: Initializes the ThingSpeak library with the Wi-Fi client for data uploads.

**Why**: The `setup()` function runs once at startup, configuring pins, sensors, and network connections to prepare the system for operation.

## 🔄 Loop Function
```cpp
void loop() {
  if (millis() - lastUpdate >= updateInterval) {
    // Read Soil Moisture
    digitalWrite(SOIL_MOISTURE_POWER, HIGH);
    digitalWrite(LDR_POWER, LOW);
    delay(100);
    float soilMoisture = analogRead(ANALOG_PIN) / 1023.0 * 100;
    digitalWrite(SOIL_MOISTURE_POWER, LOW);

    // Read LDR
    digitalWrite(LDR_POWER, HIGH);
    digitalWrite(SOIL_MOISTURE_POWER, LOW);
    delay(100);
    float lightIntensity = analogRead(ANALOG_PIN) / 1023.0 * 100;
    digitalWrite(LDR_POWER, LOW);

    // Read DHT11
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Check if DHT readings are valid
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Print sensor data
    Serial.printf("Soil Moisture: %.2f%%\n", soilMoisture);
    Serial.printf("Temperature: %.2f°C\n", temperature);
    Serial.printf("Humidity: %.2f%%\n", humidity);
    Serial.printf("Light Intensity: %.2f%%\n", lightIntensity);

    // Send data to ThingSpeak
    ThingSpeak.setField(1, soilMoisture);
    ThingSpeak.setField(2, temperature);
    ThingSpeak.setField(3, humidity);
    ThingSpeak.setField(4, lightIntensity);

    int response = ThingSpeak.writeFields(channelID, thingSpeakApiKey);
    if (response == 200) {
      Serial.println("Data sent to ThingSpeak successfully");
    } else {
      Serial.printf("Error sending data to ThingSpeak: %d\n", response);
    }

    lastUpdate = millis();
  }
}
```
The `loop()` function runs continuously, collecting and uploading sensor data every 60 seconds. Here’s a detailed breakdown:

1. **Timing Check**:
   ```cpp
   if (millis() - lastUpdate >= updateInterval)
   ```
   - Uses `millis()` to track elapsed time since the last update.
   - Executes the block only if 60 seconds (`updateInterval`) have passed, ensuring data is sent at a controlled rate.

2. **Reading Soil Moisture**:
   ```cpp
   digitalWrite(SOIL_MOISTURE_POWER, HIGH);
   digitalWrite(LDR_POWER, LOW);
   delay(100);
   float soilMoisture = analogRead(ANALOG_PIN) / 1023.0 * 100;
   digitalWrite(SOIL_MOISTURE_POWER, LOW);
   ```
   - Sets D5 HIGH to power the Soil Moisture Sensor.
   - Sets D6 LOW to ensure the LDR is off, preventing interference on A0.
   - Waits 100ms (`delay(100)`) for the sensor’s output to stabilize.
   - Reads A0 (`analogRead(ANALOG_PIN)`) to get a value (0–1023, 10-bit ADC).
   - Converts the reading to a percentage (`/ 1023.0 * 100`) for user-friendly output.
   - Powers off the Soil Moisture Sensor to prepare for the LDR reading.

   **Why**: This ensures the A0 reading is exclusively from the Soil Moisture Sensor, identified by D5 being HIGH.

3. **Reading LDR**:
   ```cpp
   digitalWrite(LDR_POWER, HIGH);
   digitalWrite(SOIL_MOISTURE_POWER, LOW);
   delay(100);
   float lightIntensity = analogRead(ANALOG_PIN) / 1023.0 * 100;
   digitalWrite(LDR_POWER, LOW);
   ```
   - Sets D6 HIGH to power the LDR’s voltage divider.
   - Sets D5 LOW to ensure the Soil Moisture Sensor is off.
   - Waits 100ms for stability.
   - Reads A0 to get the LDR’s voltage divider output (0–1023).
   - Converts to a percentage for consistency.
   - Powers off the LDR.

   **Why**: This isolates the LDR’s signal on A0, identified by D6 being HIGH, avoiding overlap with the Soil Moisture Sensor.

4. **Reading DHT11**:
   ```cpp
   float temperature = dht.readTemperature();
   float humidity = dht.readHumidity();
   ```
   - Uses the `DHT` library to read temperature (°C) and humidity (%) from the DHT11 on D4.
   - These are digital readings, independent of A0, so no power control is needed.

   **Why**: The DHT11’s digital protocol ensures clear identification of temperature and humidity data.

5. **Validating DHT Readings**:
   ```cpp
   if (isnan(temperature) || isnan(humidity)) {
     Serial.println("Failed to read from DHT sensor!");
     return;
   }
   ```
   - Checks if `temperature` or `humidity` is `NaN` (Not a Number), indicating a read error.
   - Prints an error message and skips the upload cycle (`return`) to avoid sending invalid data.

   **Why**: Ensures data integrity by skipping cycles with faulty DHT11 readings.

6. **Printing Data**:
   ```cpp
   Serial.printf("Soil Moisture: %.2f%%\n", soilMoisture);
   Serial.printf("Temperature: %.2f°C\n", temperature);
   Serial.printf("Humidity: %.2f%%\n", humidity);
   Serial.printf("Light Intensity: %.2f%%\n", lightIntensity);
   ```
   - Outputs all sensor values to the Serial Monitor with two decimal places (`%.2f`) for debugging.
   - Includes units (% for moisture and light, °C for temperature, % for humidity).

   **Why**: Helps users verify sensor readings in real-time via the Arduino IDE’s Serial Monitor.

7. **Sending Data to ThingSpeak**:
   ```cpp
   ThingSpeak.setField(1, soilMoisture);
   ThingSpeak.setField(2, temperature);
   ThingSpeak.setField(3, humidity);
   ThingSpeak.setField(4, lightIntensity);
   int response = ThingSpeak.writeFields(channelID, thingSpeakApiKey);
   ```
   - Assigns each sensor value to a ThingSpeak channel field (1–4).
   - Sends all fields to ThingSpeak using `writeFields()`, authenticated by `thingSpeakApiKey`.
   - Stores the HTTP response code in `response`.

   **Why**: Uploads data to the cloud for visualization on ThingSpeak’s dashboard.

8. **Handling Upload Response**:
   ```cpp
   if (response == 200) {
     Serial.println("Data sent to ThingSpeak successfully");
   } else {
     Serial.printf("Error sending data to ThingSpeak: %d\n", response);
   }
   ```
   - Checks if `response` is 200 (HTTP OK), indicating a successful upload.
   - Prints success or error messages with the response code for debugging.

   **Why**: Confirms whether data reached ThingSpeak, aiding troubleshooting.

9. **Updating Timing**:
   ```cpp
   lastUpdate = millis();
   ```
   - Records the current time to reset the 60-second interval.

   **Why**: Ensures the next upload occurs after `updateInterval` (60 seconds).

## 🔍 How Sensor Readings Are Distinguished
The ESP8266 has only one analog pin (A0), shared by the Soil Moisture Sensor and LDR. The code uses **time-multiplexing** to distinguish their readings:
- **Soil Moisture**: Powered on (D5 HIGH, D6 LOW), so A0 reads the Soil Moisture Sensor’s AOUT signal. The reading is stored in `soilMoisture`.
- **LDR**: Powered on (D6 HIGH, D5 LOW), so A0 reads the LDR’s voltage divider output. The reading is stored in `lightIntensity`.
- **DHT11**: Uses a digital pin (D4), so `temperature` and `humidity` are read directly via the `DHT` library, independent of A0.

The code ensures only one analog sensor is powered at a time, preventing signal interference on A0. The 100ms delay after powering each sensor allows the signal to stabilize, ensuring accurate readings.

## 💡 Notes for Users
- **Customization**: Adjust `updateInterval` if you need faster/slower updates (minimum 15 seconds for ThingSpeak’s free tier).
- **Sensor Calibration**: The `/ 1023.0 * 100` conversion assumes a linear 0–100% scale. Calibrate your sensors (e.g., test dry vs. wet soil) for accuracy.
- **Power Limits**: D5 and D6 power the sensors directly. If your Soil Moisture Sensor draws >40mA, use a transistor (e.g., 2N2222) to switch power.
- **Debugging**: Open the Serial Monitor (115200 baud) to view sensor data and connection status.
- **DHT22 Option**: If using a DHT22, change `DHT_TYPE` to `DHT22` for higher accuracy.

## 🛠️ Troubleshooting
- **Invalid DHT Readings**: Check the 10kΩ pull-up resistor and wiring to D4.
- **Wi-Fi Issues**: Verify `ssid` and `password`. Ensure the ESP8266 is within Wi-Fi range.
- **ThingSpeak Errors**: Confirm `channelID` and `thingSpeakApiKey`. Check ThingSpeak’s status codes in the Serial Monitor.
- **Inaccurate Analog Readings**: Ensure only one sensor is powered during its `analogRead()`. Test with a multimeter if needed.

This code powers the Smart Agriculture System, enabling real-time environmental monitoring for sustainable farming. For hardware details, see the [circuit diagram](photos/smart-agriculture-system-ckt.png).

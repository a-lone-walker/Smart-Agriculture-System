#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

// Wi-Fi credentials
const char* ssid = "YOUR_WIFI_SSID"; // Replace with your Wi-Fi SSID
const char* password = "YOUR_WIFI_PASSWORD"; // Replace with your Wi-Fi password

// ThingSpeak settings
const char* thingSpeakApiKey = "YOUR_THINGSPEAK_API_KEY"; // Replace with your ThingSpeak Write API Key
const unsigned long channelID = YOUR_CHANNEL_ID; // Replace with your ThingSpeak Channel ID

// Pin definitions
#define SOIL_MOISTURE_POWER D5 // Power control for Soil Moisture Sensor
#define LDR_POWER D6          // Power control for LDR
#define DHT_PIN D4            // Digital pin for DHT11 sensor
#define ANALOG_PIN A0         // Shared analog pin for Soil Moisture and LDR
#define DHT_TYPE DHT11        // DHT sensor type (DHT11 or DHT22)

// Initialize DHT sensor
DHT dht(DHT_PIN, DHT_TYPE);

// Wi-Fi client
WiFiClient client;

// Timing variables
unsigned long lastUpdate = 0;
const long updateInterval = 60000; // Update every 60 seconds

void setup() {
  Serial.begin(115200);
  delay(10);

  // Initialize pins
  pinMode(SOIL_MOISTURE_POWER, OUTPUT);
  pinMode(LDR_POWER, OUTPUT);
  digitalWrite(SOIL_MOISTURE_POWER, LOW); // Start with both sensors off
  digitalWrite(LDR_POWER, LOW);

  // Initialize DHT sensor
  dht.begin();

  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected");

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  if (millis() - lastUpdate >= updateInterval) {
    // Read Soil Moisture
    digitalWrite(SOIL_MOISTURE_POWER, HIGH); // Power on Soil Moisture Sensor
    digitalWrite(LDR_POWER, LOW);            // Power off LDR
    delay(100); // Wait for sensor to stabilize
    float soilMoisture = analogRead(ANALOG_PIN) / 1023.0 * 100; // Convert to percentage
    digitalWrite(SOIL_MOISTURE_POWER, LOW); // Power off Soil Moisture Sensor

    // Read LDR
    digitalWrite(LDR_POWER, HIGH);          // Power on LDR
    digitalWrite(SOIL_MOISTURE_POWER, LOW); // Ensure Soil Moisture is off
    delay(100); // Wait for sensor to stabilize
    float lightIntensity = analogRead(ANALOG_PIN) / 1023.0 * 100; // Convert to percentage
    digitalWrite(LDR_POWER, LOW);           // Power off LDR

    // Read DHT11
    float temperature = dht.readTemperature(); // Read temperature in Celsius
    float humidity = dht.readHumidity();       // Read humidity

    // Check if DHT readings are valid
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Print sensor data to Serial Monitor
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

    // Update last update time
    lastUpdate = millis();
  }
}

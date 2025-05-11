const channelID = "YOUR_CHANNEL_ID"; // Replace with your ThingSpeak Channel ID
const readApiKey = "YOUR_READ_API_KEY"; // Replace with your ThingSpeak Read API Key
const apiUrl = `https://api.thingspeak.com/channels/${channelID}/feeds.json?api_key=${readApiKey}&results=1`;

// Thresholds for alerts
const soilMoistureThreshold = 30; // Alert if soil moisture < 30%
const temperatureThreshold = 35; // Alert if temperature > 35°C

// Fetch data from ThingSpeak
async function fetchSensorData() {
  try {
    const response = await fetch(apiUrl);
    const data = await response.json();
    const latestFeed = data.feeds[0];

    // Extract sensor values
    const soilMoisture = parseFloat(latestFeed.field1).toFixed(2);
    const temperature = parseFloat(latestFeed.field2).toFixed(2);
    const humidity = parseFloat(latestFeed.field3).toFixed(2);
    const lightIntensity = parseFloat(latestFeed.field4).toFixed(2);

    // Update webpage
    document.getElementById("soil-moisture").textContent = `${soilMoisture} %`;
    document.getElementById("temperature").textContent = `${temperature} °C`;
    document.getElementById("humidity").textContent = `${humidity} %`;
    document.getElementById("light-intensity").textContent = `${lightIntensity} %`;

    // Check for alerts
    const alerts = [];
    if (soilMoisture < soilMoistureThreshold) {
      alerts.push(`⚠️ Low Soil Moisture: ${soilMoisture}% (Threshold: ${soilMoistureThreshold}%)`);
    }
    if (temperature > temperatureThreshold) {
      alerts.push(`⚠️ High Temperature: ${temperature}°C (Threshold: ${temperatureThreshold}°C)`);
    }

    // Display alerts
    const alertsDiv = document.getElementById("alerts");
    if (alerts.length > 0) {
      alertsDiv.innerHTML = alerts.map(alert => `<p class="alert">${alert}</p>`).join("");
    } else {
      alertsDiv.innerHTML = "<p>No alerts at the moment.</p>";
    }
  } catch (error) {
    console.error("Error fetching data:", error);
    document.getElementById("alerts").innerHTML = "<p class='alert'>Error fetching data from ThingSpeak.</p>";
  }
}

// Fetch data initially and every 60 seconds
fetchSensorData();
setInterval(fetchSensorData, 60000);

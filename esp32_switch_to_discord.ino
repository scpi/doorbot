#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* webhook_url = "your_webhook_URL";

const int switchPin = 12; // GPIO pin connected to the switch
bool switchState = false;
bool lastSwitchState = false;

void setup() {
  Serial.begin(115200);
  pinMode(switchPin, INPUT_PULLUP);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  switchState = digitalRead(switchPin) == LOW;

  if (switchState != lastSwitchState && switchState == true) {
    sendDiscordMessage("Switch flipped!");
  }

  lastSwitchState = switchState;
  delay(100);
}

void sendDiscordMessage(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(webhook_url);
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"content\":\"" + message + "\"}";
    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      Serial.printf("Message sent, response code: %d\n", httpResponseCode);
    } else {
      Serial.printf("Error sending message: %d\n", httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}
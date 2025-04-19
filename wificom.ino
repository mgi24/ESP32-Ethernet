#include <WiFi.h>

//change to your wifi SSID
const char *ssid = "YOUR WIFI";
const char *password = "YOURWIFI PASSWORD";

WiFiServer server(80);  //set serverport

void setup() {
  Serial.begin(115200);
  pinMode(15, OUTPUT);  //ignore this, just for my indicator pin
  digitalWrite(15, HIGH);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    while (client.connected()) {
      if (client.available()) {
        String message = client.readStringUntil('\n');  //read msg
        Serial.println("Received: " + message);
        client.println("You sent: " + message);     //send msg back to client
        int separatorIndex = message.indexOf('|');  //search for |
        if (separatorIndex != -1) {
          String part1 = message.substring(0, separatorIndex);  //split msg
          String part2 = message.substring(separatorIndex + 1);
          Serial.println("Part 1: " + part1);
          Serial.println("Part 2: " + part2);
        } else {
          Serial.println("Message does not contain '|'");
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

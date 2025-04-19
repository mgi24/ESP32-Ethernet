#include "SPI.h"
#include "EthernetESP32.h"

// Ethernet MAC Address (SHOULD NOT SAME AS OTHER DEVICES ON NETWORK)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
#define powerled 15  //indicator led only, not important
#define HSPI_CS 34   //config pin CS

IPAddress ip(192, 168, 0, 110);  // Set a static IP address (faster connect)
IPAddress dns(192, 168, 0, 1);

EthernetServer server(80);  // set HTTP server port
void setup() {
  pinMode(powerled, OUTPUT);
  digitalWrite(powerled, HIGH);
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  Serial.println("ESP32s2 started!");

  Ethernet.init(HSPI_CS);
  Ethernet.begin(mac, ip, dns);  //static IP
  //Ethernet.begin(mac);          //DHCP IP
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("No ethernet modeule detected");
  } else if (Ethernet.hardwareStatus() == EthernetW5100) {
    Serial.println("W5100 detected.");
  }
  Serial.print("IP Address: ");
  Serial.println(Ethernet.localIP());
  server.begin();
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("New client connected.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        String message = client.readStringUntil('\n');
        Serial.println("Received: " + message);
        client.println("You sent: " + message);
        int separatorIndex = message.indexOf('|');              //search for |
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
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }
}
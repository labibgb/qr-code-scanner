/*
	Esp32 Websockets Client

	This sketch:
        1. Connects to a WiFi network
        2. Connects to a Websockets server
        3. Sends the websockets server a message ("Hello Server")
        4. Prints all incoming messages while the connection is open

	Hardware:
        For this sketch you only need an ESP32 board.

	Created 15/02/2019
	By Gil Maimon
	https://github.com/gilmaimon/ArduinoWebsockets

*/
#include <esp32cam.h>
#include <ArduinoWebsockets.h>
#include <WiFi.h>

const char* ssid = ""; //Enter SSID
const char* password = ""; //Enter Password
const char* websockets_server_host = "0.0.0.0"; //Enter server adress
const uint16_t websockets_server_port = 8080; // Enter server port

static auto hiRes = esp32cam::Resolution::find(800, 600);

using namespace websockets;

WebsocketsClient client;

void serveJpg()
{
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    Serial.println("CAPTURE FAIL");
    return;
  }

  client.sendBinary((const char*) frame->data(), frame->size());
}

void handleJpgHi()
{
  if (!esp32cam::Camera.changeResolution(hiRes)) {
    Serial.println("SET-HIGH-RES FAIL");
  }
  serveJpg();
}

void setup() {
    Serial.begin(115200);

    Serial.println();
    {
      using namespace esp32cam;
      Config cfg;
      cfg.setPins(pins::AiThinker);
      cfg.setResolution(hiRes);
      cfg.setBufferCount(2);
      cfg.setJpeg(80);
  
      bool ok = Camera.begin(cfg);
      Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
    }

    // Connect to wifi
    WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }

    // Check if connected to wifi
    if(WiFi.status() != WL_CONNECTED) {
        Serial.println("No Wifi!");
        return;
    }

    Serial.println("Connected to Wifi, Connecting to server.");
    // try to connect to Websockets server
    bool connected = client.connect(websockets_server_host, websockets_server_port, "/");
    if(connected) {
        Serial.println("Connected!");
        //client.send("Hello Server");
    } else {
        Serial.println("Not Connected!");
    }
    
    // run callback when messages are received
    client.onMessage([&](WebsocketsMessage message){
        Serial.print("Got Message: ");
        Serial.println(message.data());
    });
}

void loop() {
    // let the websockets client check for incoming messages
    if(client.available()) {
        serveJpg();
        client.poll();
    }
    //serveJpg();
    delay(500);
}

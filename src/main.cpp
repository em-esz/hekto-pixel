#include "Arduino.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArtnetWifi.h>
#include "FastLED.h"
#include "FastLED_NeoMatrix.h"
#include "Animation.h"
#include "ESPAsyncWebServer.h"
#include "WebOta.h"
#include "HektoPixel.h"
#include "ArtNet.h"
#include "WebSocket.h"


AsyncWebServer server(80);
WebOta firmwareUpdate;
AsyncWebSocket websocket("/ws");

#define STATUS_LED 2
#define M_WIDTH 20
#define M_HEIGHT 15
#define NUM_LEDS (M_WIDTH*M_HEIGHT)

const char* host = "hektopixel";
const char* ssid = WIFI;
const char* password = WIFI_PASS;

HektoPixel board(M_WIDTH, M_HEIGHT);
uint8_t * ledsRaw = (uint8_t *)(board.getLeds());;
AnimationPlayer player((Canvas*)(board.getMatrix()));
WebAnimationSwitcher webPlayer(&player);

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(STATUS_LED, HIGH);

  if (!MDNS.begin(host)) {
    Serial.println(F("Error setting up MDNS responder!"));
    while (1) {
      delay(1000);
    }
  }
  Serial.println(F("mDNS responder started"));

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "POST, GET, DELETE");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type, Access-Control-Allow-Headers");

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  websocket.onEvent(onWsEvent);
  server.addHandler(&websocket);

  firmwareUpdate.init(&server);
  webPlayer.init(&server);

  SPIFFS.begin();
  server.begin();
}

void loop() {
  if (firmwareUpdate.shouldReboot()) {
    Serial.println(F("Firmware updated. Rebooting..."));
    digitalWrite(STATUS_LED, LOW);
    ESP.restart();
  }

  EVERY_N_MILLISECONDS(250) { //refresh of 4Hz
    websocket.cleanupClients();
    websocket.binaryAll((uint8_t*)ledsRaw, (size_t)900);
  }
  player.update(millis());
}
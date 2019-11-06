#include "Arduino.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArtnetWifi.h>
#include "FastLED.h"
#include "FastLED_NeoMatrix.h"
#include "ESPAsyncWebServer.h"
#include "WebOta.h"
#include "HektoPixel.h"
#include "WebSocket.h"
#include "animations/Noise.h"
#include "animations/Text.h"
#include "animations/ArtNet.h"
#include "animations/Plasma.h"
#include "animations/Sketch.h"

#define STATUS_LED 2
#define M_WIDTH 20
#define M_HEIGHT 15
#define NUM_LEDS (M_WIDTH*M_HEIGHT)
#define NUM_OF_ANIMATIONS 5

AsyncWebServer server(80);
WebOta firmwareUpdate;
AsyncWebSocket previewSocket("/ws");
Board board(M_WIDTH, M_HEIGHT);

const char* host = "hektopixel";
const char* ssid = WIFI;
const char* password = WIFI_PASS;

TextAnimation textAnimation;
Animation* animations[NUM_OF_ANIMATIONS] = {
  new RandomNoise(),
  &textAnimation,
  new ArtnetAnimation(),
  new Plasma(),
  new SketchClient(server)
};

AnimationPlayer player(board);
WebManager webManager(player, animations, NUM_OF_ANIMATIONS);

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
  textAnimation.setMessage(WiFi.localIP().toString());
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

  previewSocket.onEvent(onWsEvent);
  server.addHandler(&previewSocket);

  firmwareUpdate.init(server);
  webManager.init(server);

  player.setAnimation(&textAnimation);

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
    previewSocket.cleanupClients();
    previewSocket.binaryAll((uint8_t*)board.getLeds(), (size_t)BOARD_DATA_SIZE);
  }

  player.update(millis());
}
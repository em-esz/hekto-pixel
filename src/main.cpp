#include "Arduino.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <FS.h>
#include <SPIFFS.h>
#include "FastLED.h"
#include "FastLED_NeoMatrix.h"
#include "Animation.h"
#include "ESPAsyncWebServer.h"
#include "WebOta.h"
#include "HektoPixel.h"

AsyncWebServer server(80);
WebOta firmwareUpdate;

#define WIFI "TBSCG_IoT"
#define WIFI_PASS "VQxxsE@1d96Op"

#define DATA_PIN 14
#define STATUS_LED 2
#define M_WIDTH 8
#define M_HEIGHT 2
#define NUM_LEDS (M_WIDTH*M_HEIGHT)

const char* host = "hektopixel";
const char* ssid = WIFI;
const char* password = WIFI_PASS;

CRGB leds[NUM_LEDS];

FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, M_WIDTH, M_HEIGHT, 
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG );

AnimationPlayer player(matrix);
WebAnimationSwitcher hektoPixel(&player);

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
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "POST, GET");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type, Access-Control-Allow-Headers");
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  firmwareUpdate.init(&server);
  hektoPixel.init(&server);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(30);

  SPIFFS.begin();
  matrix->begin();
  server.begin();
}

void loop() {
  delay(1);
  if (firmwareUpdate.shouldReboot()) {
    Serial.println("Rebooting...");
    digitalWrite(STATUS_LED, LOW);
    ESP.restart();
  }

  player.update(millis());

}
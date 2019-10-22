#include "Arduino.h"
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "FastLED.h"
#include "WebOta.h"
#include "FastLED_NeoMatrix.h"
#include "animation.h"

#define DATA_PIN 14
#define M_WIDTH 8
#define M_HEIGHT 1
#define NUM_LEDS (M_WIDTH*M_HEIGHT)

AsyncWebServer server(80);
WebOta firmwareUpdater;

const char* ssid = "...";
const char* password = "...";


CRGB leds[NUM_LEDS];

FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, M_WIDTH, M_HEIGHT, 
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG );

Player player(matrix);
Blink blink(matrix);

void notFoundHandler(AsyncWebServerRequest *request){
  request->send(404);
}

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

  server.onNotFound(notFoundHandler);

  firmwareUpdater.init(&server);

  server.begin();

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(30);

  player.setAnimation(&blink);
  matrix->begin();
}

void loop(){
  if(firmwareUpdater.shouldReboot()){
    Serial.println("Rebooting...");
    delay(100);
    ESP.restart();
  } 

  player.update(millis());

}
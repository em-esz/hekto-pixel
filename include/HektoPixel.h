#pragma once
#include "ESPAsyncWebServer.h"
#include <functional>
#include <FastLED_NeoMatrix.h>
#include <FastLED.h>

#define LED_DATA_PIN 14
#define BOARD_DATA_SIZE 900

typedef FastLED_NeoMatrix Canvas;

class Board {
private:
    const uint8_t width_;
    const uint8_t height_;
    int size_;
    CRGB * leds;
    FastLED_NeoMatrix * matrix;
public:
    Board(uint8_t width, uint8_t height);
    uint8_t width();
    uint8_t height();
    Canvas& getMatrix();
    CRGB* getLeds();
    void show();
    void setBrightness(uint8_t brightness);
    void writeRawLedData(uint8_t * data);
};

class Animation {
public:
    int interval = 100;
    String name;
public:
    Animation() { }
    virtual boolean renderFrame(Canvas &canvas) = 0;
    virtual void start(Board &board);
    virtual void stop();
    virtual boolean configure(AsyncWebServerRequest *request);
};

class AnimationPlayer {
private:
    Animation *animation;
    Board &board;
    long lastRender = 0;
public:
    AnimationPlayer(Board& _board) : board(_board) {
    }
    void update(long currentTime);
    void setAnimation(Animation *animation);
};

class WebManager {
private:
    AnimationPlayer& player;
    Animation** animations;
    uint8_t numberOfAnimations = 0;
    void handlePlayRequest(AsyncWebServerRequest *request);
    Animation* findAnimation(String name);
public:
    WebManager(AnimationPlayer &player, Animation** animations, uint8_t numOfAnimations);
    void init(AsyncWebServer &server);
};
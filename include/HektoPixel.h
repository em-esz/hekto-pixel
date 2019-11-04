#pragma once
#include "ESPAsyncWebServer.h"
#include <functional>

#define NUM_OF_ANIMATIONS 3
#define LED_DATA_PIN 14

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
    const Canvas * getMatrix();
    CRGB* getLeds();
    void show();
    void setBrightness(uint8_t brightness);
};

class WebManager {
private:
    AnimationPlayer* player;
    Animation* animations[];
    void handlePlayRequest(AsyncWebServerRequest *request);
    Animation* findAnimation(String name);
public:
    WebManager(AnimationPlayer *player, Animation* animations[]);
    void init(AsyncWebServer *server);
};

class Animation {
public:
    int interval = 100;
    String name;
public:
    Animation() { }
    virtual boolean renderFrame(Canvas *canvas) = 0;
    virtual void init(Canvas *canvas);
};

class AnimationPlayer {
private:
    Animation *animation;
    Canvas *canvas;
    long lastRender = 0;
public:
    AnimationPlayer(Canvas *canvas) {
        this->canvas = canvas;
    }
    void update(long currentTime);
    void setAnimation(Animation *animation);
};
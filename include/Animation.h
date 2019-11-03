#pragma once
#include "FastLED_NeoMatrix.h"
#include "FastLED.h"

typedef FastLED_NeoMatrix Canvas;

class Animation {
public:
    int interval = 100;
    String name;
public:
    Animation() { }
    virtual boolean renderFrame(Canvas *canvas) = 0;
    virtual void init(Canvas *canvas) {
        canvas->clear();
    }
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


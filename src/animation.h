#include "FastLED_NeoMatrix.h"

class Animation {
protected:
    FastLED_NeoMatrix *canvas;
public:
    int interval = 100;
public:
    Animation(FastLED_NeoMatrix *canvas) {
        this->canvas = canvas;
    }
    virtual void renderFrame() = 0;
};

class Player {
private:
    Animation *animation;
    FastLED_NeoMatrix *canvas;
    long lastRender = 0;
public:
    Player(FastLED_NeoMatrix *canvas) {
        this->canvas = canvas;
    }
    void update(long currentTime);
    void setAnimation(Animation *animation);
};

class TestAnimation: public Animation {
public:
    void renderFrame();
};


void Player::update(long currentTime) {
    if (lastRender + animation->interval <= currentTime) {
        animation->renderFrame();
        lastRender = currentTime;
        canvas->show();
    }
}

void Player::setAnimation(Animation *animation) {
    this->animation = animation;
}

class Blink : public Animation {
    private:
        uint8_t current = 0;
        boolean isOn = false;
    public:
        Blink(FastLED_NeoMatrix *canvas) : Animation(canvas) {
            interval = 1000;
        }
        void renderFrame() {
            if (isOn) {
                canvas->clear();
            } else {
                canvas->fillScreen(canvas->Color(255, 0, 0));
                canvas->drawLine(0, 0, 3, 0, canvas->CRGBtoint32(CRGB::Green));
                canvas->drawPixel(5, 0, canvas->CRGBtoint32(CRGB::Blue));
            }
            isOn = !isOn;
            current++;
            if (current == canvas->width()) {
                current = 0;
            }
        }
};


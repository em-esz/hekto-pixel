#include "FastLED_NeoMatrix.h"
#include "Arduino.h"

typedef FastLED_NeoMatrix Canvas;

class Animation {
public:
    int interval = 100;
    String name;
public:
    Animation() { }
    virtual boolean renderFrame(Canvas *canvas) = 0;
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


void AnimationPlayer::update(long currentTime) {
    if (animation == NULL) {
        return;
    }
    if (lastRender + animation->interval <= currentTime) {
        lastRender = currentTime;
        if (animation->renderFrame(this->canvas)) {
            canvas->show();
        }
    }
}

void AnimationPlayer::setAnimation(Animation *animation) {
    this->animation = animation;
}

class Test : public Animation {
    private:
        uint8_t current = 0;
        boolean isOn = false;
    public:
        Test() : Animation() {
            interval = 1000;
            name = "test";
        }
        boolean renderFrame(Canvas *canvas) {
            if (isOn) {
                canvas->clear();
            } else {
                canvas->fillScreen(canvas->Color(255, 0, 0));
                canvas->drawRect(0, 0, 4, 2, canvas->Color(0, 255, 0));
                canvas->drawPixel(5, 0, canvas->CRGBtoint32(CRGB::Blue));
            }
            isOn = !isOn;
            current++;
            if (current == canvas->width()) {
                current = 0;
            }
            return true;
        }
};

class BlinkBuiltInLed : public Animation {
    private:
       int ledState = LOW;
       const int ledPin = 2;
    public:
        BlinkBuiltInLed() : Animation() {
            name = "ledblink";
            interval = 1000;
            pinMode(ledPin, OUTPUT);
        }
        boolean renderFrame(Canvas *canvas) {
            ledState = not(ledState);
            digitalWrite(ledPin,  ledState);
            return false;
        }

};

class RandomNoise: public Animation {
    public:
        RandomNoise() : Animation() {
            name = "noise";
            interval = 500;
        }
        boolean renderFrame(Canvas *canvas) {
            for (int16_t x = 0; x < canvas->width(); x++) {
                for (int16_t y = 0; y < canvas->height(); y++) {
                    canvas->drawPixel(x, y, canvas->Color(random(256), random(256), random(256)));
                }
            }
            return true;
        }
};


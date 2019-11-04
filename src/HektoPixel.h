#include "ESPAsyncWebServer.h"
#include <functional>
#include "Animation.h"
#include "animations/Noise.h"
#include "animations/Plasma.h"
#include "animations/Text.h"

#define NUM_OF_ANIMATIONS 3
#define LED_DATA_PIN 14

class HektoPixel {
    private:
        const uint8_t width_;
        const uint8_t height_;
        int size_;
        CRGB * leds;
        FastLED_NeoMatrix * matrix;
    public:
        HektoPixel(uint8_t width, uint8_t height) : width_(width), height_(height), size_(width * height) {
            this->leds = new CRGB[this->size_];
            this->matrix = new FastLED_NeoMatrix(
                    this->leds,
                    this->width_,
                    this->height_, 
                    NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG);
            FastLED.addLeds<WS2811, LED_DATA_PIN, RGB>(leds, this->size_);
            FastLED.setBrightness(100);
        }
        uint8_t width() {
            return this->width_;
        }
        uint8_t height() {
            return this->height_;
        }
        const FastLED_NeoMatrix * getMatrix() {
            return this->matrix;
        }
        CRGB* getLeds() {
            return this->leds;
        }
        void show() {
            FastLED.show();
        }
        void setBrightness(uint8_t brightness) {
            FastLED.setBrightness(brightness);
        }
};

class WebAnimationSwitcher {
    private:
        Animation* animations[NUM_OF_ANIMATIONS] = {
            new Plasma(),
            new RandomNoise(),
            new TextAnimation()
        };
        AnimationPlayer *player;

        void handlePlayRequest(AsyncWebServerRequest *request) {
                Animation *animation = this->findAnimation(request->pathArg(0));
                if (animation == NULL) {
                    request->send(404);
                } else {
                    if (animation->name == "text") {
                        if (request->hasParam("msg", true)) {
                            const String& msg = request->getParam("msg", true)->value();
                            TextAnimation* textAnimation = (TextAnimation*)animation;
                            textAnimation->setMessage(msg);
                        } else {
                            request->send(400);
                        }
                    }
                    player->setAnimation(animation);
                    request->send(200);
                }
        }

        Animation* findAnimation(String name) {
            for (int i = 0; i < NUM_OF_ANIMATIONS; i++) {
                if (animations[i]->name == name) {
                    return animations[i];
                }
            }
            return NULL;
        }
    public:
        WebAnimationSwitcher(AnimationPlayer *player) {
            this->player = player;
        }

        void init(AsyncWebServer *server) {
            server->on("^\\/animation\\/play\\/([0-9a-z]+)$", HTTP_POST, std::bind(&WebAnimationSwitcher::handlePlayRequest, this, std::placeholders::_1));
        }

};
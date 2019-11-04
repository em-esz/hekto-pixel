#pragma once
#include "ESPAsyncWebServer.h"
#include <functional>
#include "HektoPixel.h"
#include "FastLED.h"
#include "FastLED_NeoMatrix.h"

Board::Board(uint8_t width, uint8_t height) : width_(width), height_(height), size_(width * height) {
    this->leds = new CRGB[this->size_];
    this->matrix = new FastLED_NeoMatrix(
            this->leds,
            this->width_,
            this->height_, 
            NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG);
    FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, this->size_);
    FastLED.setBrightness(100);
}

uint8_t Board::width() {
    return this->width_;
}
uint8_t Board::height() {
    return this->height_;
}
const Canvas* Board::getMatrix() {
    return this->matrix;
}
CRGB* Board::getLeds() {
    return this->leds;
}
void Board::show() {
    FastLED.show();
}
void Board::setBrightness(uint8_t brightness) {
    FastLED.setBrightness(brightness);
}


void WebManager::handlePlayRequest(AsyncWebServerRequest *request) {
    Animation *animation = this->findAnimation(request->pathArg(0));
    if (animation == NULL) {
        request->send(404);
    } else {
        if (animation->name == "text") {
            if (request->hasParam("msg", true)) {
                // const String& msg = request->getParam("msg", true)->value();
                // TextAnimation* textAnimation = (TextAnimation*)animation;
                // textAnimation->setMessage(msg);
            } else {
                request->send(400);
            }
        }
        player->setAnimation(animation);
        request->send(200);
    }
}

Animation* WebManager::findAnimation(String name) {
    for (int i = 0; i < NUM_OF_ANIMATIONS; i++) {
        if (animations[i]->name == name) {
            return animations[i];
        }
    }
    return NULL;
}
WebManager::WebManager(AnimationPlayer *player, Animation* animations[]) {
    this->animations = animations;
    this->player = player;
}

void WebManager::init(AsyncWebServer *server) {
    server->on("^\\/animation\\/play\\/([0-9a-z]+)$", HTTP_POST, std::bind(&WebManager::handlePlayRequest, this, std::placeholders::_1));
}

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

void AnimationPlayer::setAnimation(Animation* animation) {
    this->animation = animation;
    this->animation->init(canvas);
}
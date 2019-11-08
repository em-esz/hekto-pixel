#include "ESPAsyncWebServer.h"
#include <functional>
#include "HektoPixel.h"
#include "FastLED.h"
#include "FastLED_NeoMatrix.h"
#include "ArduinoJson.h"
#include "AsyncJson.h"


Board::Board(uint8_t width, uint8_t height) : width_(width), height_(height), size_(width * height) {
    this->leds = new CRGB[this->size_];
    this->matrix = new FastLED_NeoMatrix(
            this->leds,
            this->width_,
            this->height_, 
            NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG);
    FastLED.addLeds<WS2811, LED_DATA_PIN, RGB>(leds, this->size_);
    FastLED.setBrightness(100);
}

uint8_t Board::width() {
    return this->width_;
}
uint8_t Board::height() {
    return this->height_;
}
Canvas& Board::getMatrix() {
    return *matrix;
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


void WebManager::handlePlayRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    if (len != total) {
        Serial.print("Total different than current length: ");
        Serial.print(total);
        Serial.print(", ");
        Serial.println(len);
        request->send(400);
    }
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, data);
    if (error) {
        Serial.println(error.c_str());
        request->send(400);
    }
    Animation *animation = this->findAnimation(request->pathArg(0));
    if (animation == NULL) {
        request->send(404);
    } else {
        if (!animation->configure(doc)) {
            request->send(400);
        }
        player.setAnimation(animation);
        request->send(200);
    }
}

Animation* WebManager::findAnimation(String name) {
    for (int i = 0; i < numberOfAnimations; i++) {
        if (animations[i]->name == name) {
            return animations[i];
        }
    }
    return NULL;
}
WebManager::WebManager(AnimationPlayer &_player, Animation **_animations, uint8_t _numberOfAnimations): player(_player) {
    animations = _animations;
    numberOfAnimations = _numberOfAnimations;
}

void WebManager::init(AsyncWebServer &server) {
    using namespace::std::placeholders;
    server.on("^\\/animation\\/play\\/([0-9a-z]+)$", HTTP_POST, 
    [](AsyncWebServerRequest *request) { request->send(200);},
    NULL,
    std::bind(&WebManager::handlePlayRequest, this, _1, _2, _3, _4, _5)
    );
}

void AnimationPlayer::update(long currentTime) {
    if (animation == NULL) {
        return;
    }
    if (lastRender + animation->interval <= currentTime) {
        lastRender = currentTime;
        if (animation->renderFrame(board.getMatrix())) {
            board.show();
        }
    }
}

void AnimationPlayer::setAnimation(Animation* animation) {
    if (this->animation != NULL) {
        animation->stop();
    }
    this->animation = animation;
    this->animation->start(this->board);
}

void Animation::start(Board &board) {
    board.getMatrix().clear();
}

void Animation::stop() {

}

boolean Animation::configure(AsyncWebServerRequest *request) {
    return true;
}

boolean Animation::configure(JsonDocument &config) {
    if (config.containsKey(F("interval"))) {
        interval = config[F("interval")];
    }
    return true;
}
#include "ESPAsyncWebServer.h"
#include <functional>

#define NUM_OF_ANIMATIONS 4

class WebAnimationSwitcher {
    private:
        Animation* animations[NUM_OF_ANIMATIONS] = {
            new BlinkBuiltInLed(),
            new Test(),
            new Plasma(),
            new RandomNoise()
        };
        AnimationPlayer *player;

        void handlePlayRequest(AsyncWebServerRequest *request) {
                Serial.print(F("Looking for animation: "));
                Serial.println(request->pathArg(0));
                Animation *animation = this->findAnimation(request->pathArg(0));
                if (animation == NULL) {
                    request->send(404);
                } else {
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
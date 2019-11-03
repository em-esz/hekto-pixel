#include "ESPAsyncWebServer.h"
#include <functional>
#include "Animation.h"
#include "animations/Noise.h"
#include "animations/Plasma.h"
#include "animations/Text.h"

#define NUM_OF_ANIMATIONS 3

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
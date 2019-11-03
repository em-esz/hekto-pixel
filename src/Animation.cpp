#include "Animation.h"

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
    this->animation->init(canvas);
}

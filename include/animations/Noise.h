#include "HektoPixel.h"

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
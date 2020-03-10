#include "HektoPixel.h"
#include "arcade/Mario.h"

class Arcade: public Animation {
    private:
        int currentFrame = 0;
        CRGB backgroundColor;
        Mario mario;

    public:
        Arcade() : Animation() {
            name = "arcade";
            interval = 120;
        }

        boolean configure(JsonDocument &json) {
            Animation::configure(json);
            if (json.containsKey(F("color"))) {
                JsonObject color = json[F("color")];
                uint8_t r = color["r"];
                uint8_t g = color["g"];
                uint8_t b = color["b"];
                this->backgroundColor = CRGB(r, g, b);
            }
            return true;
        }
        
        boolean renderFrame(Canvas &canvas) {
            for (int16_t x = 0; x < canvas.width(); x++) {
                for (int16_t y = 0; y < canvas.height(); y++) {
                    uint32_t color = mario.getPixelColor(currentFrame, y, x);
                    drawPixel(x, y, canvas, color);
                }
            }
            currentFrame++;
            restartAnimation();

            return true;
        }

        void drawPixel(int16_t &x, int16_t &y, Canvas &canvas, uint32_t &pixelColor) {
            if (pixelColor != mario.getBackgroundColor()) {
                canvas.drawPixel(x, y, canvas.Color24to16(pixelColor));
            } else {
                canvas.drawPixel(x, y, canvas.CRGBtoint32(pgm_read_dword(&this->backgroundColor)));
            }
        }

        void restartAnimation() {
            if (currentFrame > mario.getTotalFramesNumber() - 1) {
                currentFrame = 0;
            }
        }
};
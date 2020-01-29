#include "HektoPixel.h"
#include "arcade/Mario.h"

class Arcade: public Animation {
    private:
        int currentFrame = 0;
        CRGB backgroundColor PROGMEM = CRGB(0, 77, 0);
        Mario mario = Mario();

    public:
        Arcade() : Animation() {
            name = "arcade";
            interval = 200;
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
                    uint32_t color = pgm_read_dword(&mario.marioFrames[currentFrame][y][x]);
                    drawPixel(x, y, canvas, color);
                }
            }
            currentFrame = currentFrame == 0;

            return true;
        }

        void drawPixel(int16_t &x, int16_t &y, Canvas &canvas, uint32_t &pixelColor) {
            if (pixelColor != mario.marioBgColor) {
                canvas.drawPixel(x, y, canvas.Color24to16(pixelColor));
            } else {
                canvas.drawPixel(x, y, canvas.CRGBtoint32(pgm_read_dword(&this->backgroundColor)));
            }
        }
};
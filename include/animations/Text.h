#include "HektoPixel.h"
#include "ArduinoJson.h"

class TextAnimation: public Animation {
    private:
        int pos = 0;
        int textLength = 42;
        CRGB textColor = CRGB::White;
    public:
        String message = "hello:)";
    public:
        TextAnimation(): Animation() {
            name = "text";
            interval = 100;
        }
        void start(Board &board) {
            Animation::start(board);
            board.getMatrix().setTextWrap(false);
            board.getMatrix().setTextColor(board.getMatrix().Color(textColor.r, textColor.g, textColor.b));
            pos = board.width();
        }
        void setMessage(const String& msg) {
            this->message = msg;    
            this->textLength = msg.length() * 6;
        }
        boolean renderFrame(Canvas &canvas) {
            canvas.clear();
            canvas.setCursor(pos--, 4);
            canvas.print(message);
            if (pos < -(this->textLength)) {
                pos = canvas.width();
            }
            return true;
        }
        boolean configure(JsonDocument &json) {
            Animation::configure(json);
            if (json.containsKey(F("msg"))) {
                setMessage(json[F("msg")]);
            }
            if (json.containsKey(F("color"))) {
                JsonObject color = json[F("color")];
                uint8_t r = color["r"];
                uint8_t g = color["g"];
                uint8_t b = color["b"];
                this->textColor = CRGB(r, g, b);
            }
            return true;
        }

        void dumpConfig(JsonDocument &json) {
            Animation::dumpConfig(json);
            json[F("msg")] = message;
            json[F("color")]["r"] = textColor.r;
            json[F("color")]["g"] = textColor.g;
            json[F("color")]["b"] = textColor.b;
        }
};
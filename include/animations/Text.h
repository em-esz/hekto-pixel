#include "HektoPixel.h"

class TextAnimation: public Animation {
    private:
        int pos = 0;
        int textLength = 42;
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
            board.getMatrix().setTextColor(board.getMatrix().Color(0, 255, 0));
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
        boolean configure(AsyncWebServerRequest *request) {
            if (!request->hasParam("msg", true)) {
                return false;
            }
            const String& msg = request->getParam("msg", true)->value();
            setMessage(msg);
            return true;
        }
};
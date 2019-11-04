#include "HektoPixel.h"

class TextAnimation: public Animation {
    private:
        int pos = 0;
        int textLength = 0;
    public:
        String message = "hello :)";
    public:
        TextAnimation(): Animation() {
            name = "text";
            interval = 100;
        }
        void init(Canvas *canvas) {
            Animation::init(canvas);
            canvas->setTextWrap(false);
            canvas->setTextColor(canvas->Color(255, 0, 0));
            pos = canvas->width();
        }
        void setMessage(const String& msg) {
            this->message = msg;    
            this->textLength = msg.length() * 6;
        }
        boolean renderFrame(Canvas *canvas) {
            canvas->clear();
            canvas->setCursor(pos--, 4);
            canvas->print(message);
            if (pos < -(this->textLength)) {
                pos = canvas->width();
            }
            return true;
        }
};
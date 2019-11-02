#include "FastLED_NeoMatrix.h"
#include "FastLED.h"
#include "Arduino.h"

typedef FastLED_NeoMatrix Canvas;

DEFINE_GRADIENT_PALETTE( pit ) {
    0,     3,   3,   3,
    64,   13,   13, 255,  //blue
    128,   3,   3,   3,
    192, 255, 130,   3 ,  //orange
    255,   3,   3,   3
};

class Animation {
public:
    int interval = 100;
    String name;
public:
    Animation() { }
    virtual boolean renderFrame(Canvas *canvas) = 0;
    virtual void init(Canvas *canvas) {
        canvas->clear();
    }
};

class AnimationPlayer {
private:
    Animation *animation;
    Canvas *canvas;
    long lastRender = 0;
public:
    AnimationPlayer(Canvas *canvas) {
        this->canvas = canvas;
    }
    void update(long currentTime);
    void setAnimation(Animation *animation);
};


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

class Test : public Animation {
    private:
        uint8_t current = 0;
        boolean isOn = false;
    public:
        Test() : Animation() {
            interval = 1000;
            name = "test";
        }
        boolean renderFrame(Canvas *canvas) {
            if (isOn) {
                canvas->clear();
            } else {
                canvas->fillScreen(canvas->Color(255, 0, 0));
                canvas->drawRect(0, 0, 4, 2, canvas->Color(0, 255, 0));
                canvas->drawPixel(5, 0, canvas->CRGBtoint32(CRGB::Blue));
            }
            isOn = !isOn;
            current++;
            if (current == canvas->width()) {
                current = 0;
            }
            return true;
        }
};

class BlinkBuiltInLed : public Animation {
    private:
       int ledState = LOW;
       const int ledPin = 2;
    public:
        BlinkBuiltInLed() : Animation() {
            name = "ledblink";
            interval = 1000;
            pinMode(ledPin, OUTPUT);
        }
        boolean renderFrame(Canvas *canvas) {
            ledState = not(ledState);
            digitalWrite(ledPin,  ledState);
            return false;
        }

};

class Plasma: public Animation {
    private:    
        // matrix size
        uint8_t Width  = 20;
        uint8_t Height = 15;
        uint8_t CentreX =  (Width / 2) - 1;
        uint8_t CentreY = (Height / 2) - 1;

        #define NUM_LAYERS 1
        uint32_t x[NUM_LAYERS];
        uint32_t y[NUM_LAYERS];
        uint32_t z[NUM_LAYERS];
        uint32_t scale_x[NUM_LAYERS];
        uint32_t scale_y[NUM_LAYERS];
        uint8_t  noise[1][16][16];

    public:
        Plasma() : Animation() {
            name = "plasma";
            interval = 25;
        }
        boolean renderFrame(Canvas *canvas) {
            CRGBPalette16 Pal( pit );

            //modulate the position so that it increases/decreases x
            //(here based on the top left pixel - it could be any position else)
            //the factor "2" defines the max speed of the x movement
            //the "-255" defines the median moving direction
            x[0] = x[0] + (2 * noise[0][0][0]) - 255;
            //modulate the position so that it increases/decreases y
            //(here based on the top right pixel - it could be any position else)
            y[0] = y[0] + (2 * noise[0][Width-1][0]) - 255;
            //z just in one direction but with the additional "1" to make sure to never get stuck
            //in case the movement is stopped by a crazy parameter (noise data) combination
            //(here based on the down left pixel - it could be any position else)
            z[0] += 1 + ((noise[0][0][Height-1]) / 4);
            //set the scaling based on left and right pixel of the middle line
            //here you can set the range of the zoom in both dimensions
            scale_x[0] = 8000 + (noise[0][0][CentreY] * 16);
            scale_y[0] = 8000 + (noise[0][Width-1][CentreY] * 16);

            //calculate the noise data
            uint8_t layer = 0;
            for (uint8_t i = 0; i < Width; i++) {
                uint32_t ioffset = scale_x[layer] * (i - CentreX);
                for (uint8_t j = 0; j < Height; j++) {
                uint32_t joffset = scale_y[layer] * (j - CentreY);
                uint16_t data = inoise16(x[layer] + ioffset, y[layer] + joffset, z[layer]);
                // limit the 16 bit results to the interesting range
                if (data < 11000) data = 11000;
                if (data > 51000) data = 51000;
                // normalize
                data = data - 11000;
                // scale down that the result fits into a byte
                data = data / 161;
                // store the result in the array
                noise[layer][i][j] = data;
                }
            }

            //map the colors
            for (uint8_t y = 0; y < Height; y++) {
                for (uint8_t x = 0; x < Width; x++) {
                //I will add this overlay CRGB later for more colors
                //it´s basically a rainbow mapping with an inverted brightness mask
                CRGB overlay = CHSV(noise[0][y][x], 255, noise[0][x][y]);
                //here the actual colormapping happens - note the additional colorshift caused by the down right pixel noise[0][15][15]
                canvas->drawPixel(x,y,ColorFromPalette( Pal, noise[0][Width-1][Height-1] + noise[0][x][y]) + overlay);
                }
            }
            return true;
        }
};

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

class TextAnimation: public Animation {
    private:
        int pos = 0;
        int textLength = 0;
    public:
        String message = "";
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


#include "HektoPixel.h"

using namespace std;

typedef vector<vector<vector<uint32_t>>> ArcadeFrames;

class ArcadeModel {
    public:
        virtual uint32_t getBackgroundColor() = 0;
        virtual uint32_t getPixelColor(int16_t frame, int16_t column, int16_t row) = 0;
        virtual int getTotalFramesNumber() = 0;
};
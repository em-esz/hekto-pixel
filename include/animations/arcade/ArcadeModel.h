#include "HektoPixel.h"

typedef std::vector<std::vector<std::vector<uint32_t>>> Frames;

class ArcadeModel {
    public:
        virtual uint32_t getBackgroundColor() = 0;
        virtual Frames getFrames() = 0;
};
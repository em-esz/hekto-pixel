#include "HektoPixel.h"
#include "ArcadeModel.h"

class Mario: public ArcadeModel {
    public:
        uint32_t getBackgroundColor() {
            return backgroundColor;
        }

        uint32_t getPixelColor(int16_t frame, int16_t column, int16_t row) {
            return pgm_read_dword(&frames[frame][column][row]);
        }

        int getTotalFramesNumber() {
            return totalFrames;
        }

    private:
        const int totalFrames = 4;
        const uint32_t backgroundColor = 0x99ccff;
        const ArcadeFrames frames PROGMEM = {
            {
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0x996600, 0x996600, 0xffcc66, 0xffcc66, 0x000000, 0xffcc66, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0xffcc66, 0x996600, 0xffcc66, 0xffcc66, 0xffcc66, 0x000000, 0xffcc66, 0xffcc66, 0xffcc66, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0xffcc66, 0x996600, 0x996600, 0xffcc66, 0xffcc66, 0xffcc66, 0x000000, 0xffcc66, 0xffcc66, 0xffcc66, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0x996600, 0xffcc66, 0xffcc66, 0xffcc66, 0xffcc66, 0x000000, 0x000000, 0x000000, 0x000000, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xffcc66, 0xffcc66, 0xffcc66, 0xffcc66, 0xffcc66, 0xffcc66, 0xffcc66, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xff3300, 0xff3300, 0x0066cc, 0xff3300, 0xff3300, 0xff3300, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xff3300, 0xff3300, 0xff3300, 0x0066cc, 0xff3300, 0xff3300, 0x0066cc, 0xff3300, 0xff3300, 0xff3300, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xffffff, 0xffffff, 0xff3300, 0x0066cc, 0xffff00, 0x0066cc, 0x0066cc, 0xffff00, 0x0066cc, 0xff3300, 0xffffff, 0xffffff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xffffff, 0xffffff, 0xffffff, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0xffffff, 0xffffff, 0xffffff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xffffff, 0xffffff, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0xffffff, 0xffffff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x0066cc, 0x0066cc, 0x0066cc, 0x99ccff, 0x99ccff, 0x0066cc, 0x0066cc, 0x0066cc, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0x996600, 0x996600, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0x996600, 0x996600, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}
            },
            {
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0x996600, 0x996600, 0xffcc66, 0xffcc66, 0x000000, 0xffcc66, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0xffcc66, 0x996600, 0xffcc66, 0xffcc66, 0xffcc66, 0x000000, 0xffcc66, 0xffcc66, 0xffcc66, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0xffcc66, 0x996600, 0x996600, 0xffcc66, 0xffcc66, 0xffcc66, 0x000000, 0xffcc66, 0xffcc66, 0xffcc66, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0x996600, 0xffcc66, 0xffcc66, 0xffcc66, 0xffcc66, 0x000000, 0x000000, 0x000000, 0x000000, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xffcc66, 0xffcc66, 0xffcc66, 0xffcc66, 0xffcc66, 0xffcc66, 0xffcc66, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0x0066cc, 0x0066cc, 0xff3300, 0xff3300, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0xffffff, 0xffffff, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0x0066cc, 0x0066cc, 0x0066cc, 0xff3300, 0xff3300, 0xff3300, 0xffffff, 0xffffff, 0xffffff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0xffffff, 0xffffff, 0xffffff, 0x99ccff, 0xff3300, 0xff3300, 0x0066cc, 0xffff00, 0x0066cc, 0x0066cc, 0x0066cc, 0xff3300, 0xff3300, 0xffffff, 0xffffff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0xffffff, 0xffffff, 0x99ccff, 0x99ccff, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x99ccff, 0x99ccff, 0x996600, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x996600, 0x996600, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x996600, 0x996600, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0x996600, 0x0066cc, 0x0066cc, 0x0066cc, 0x99ccff, 0x99ccff, 0x99ccff, 0x0066cc, 0x0066cc, 0x0066cc, 0x996600, 0x996600, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0x996600, 0x996600, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}
            },
            {
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0x996600, 0x996600, 0xffcc66, 0xffcc66, 0x000000, 0xffcc66, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0xffcc66, 0x996600, 0xffcc66, 0xffcc66, 0xffcc66, 0x000000, 0xffcc66, 0xffcc66, 0xffcc66, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0xffcc66, 0x996600, 0x996600, 0xffcc66, 0xffcc66, 0xffcc66, 0x000000, 0xffcc66, 0xffcc66, 0xffcc66, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0x996600, 0xffcc66, 0xffcc66, 0xffcc66, 0xffcc66, 0x000000, 0x000000, 0x000000, 0x000000, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xffcc66, 0xffcc66, 0xffcc66, 0xffcc66, 0xffcc66, 0xffcc66, 0xffcc66, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xff3300, 0xff3300, 0x0066cc, 0xff3300, 0xff3300, 0xff3300, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0x0066cc, 0x0066cc, 0xff3300, 0xff3300, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xff3300, 0xff3300, 0xff3300, 0x0066cc, 0x0066cc, 0xffff00, 0x0066cc, 0x0066cc, 0xffff00, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x0066cc, 0xff3300, 0xff3300, 0xffffff, 0xffffff, 0xffffff, 0x0066cc, 0x0066cc, 0x0066cc, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x0066cc, 0xff3300, 0xffffff, 0xffffff, 0x0066cc, 0x0066cc, 0x0066cc, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x0066cc, 0x0066cc, 0x0066cc, 0x996600, 0x996600, 0x996600, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0x996600, 0x996600, 0x996600, 0x996600, 0x996600, 0x996600, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}
            },
            {
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0x996600, 0x996600, 0xffcc66, 0xffcc66, 0x000000, 0xffcc66, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0xffcc66, 0x996600, 0xffcc66, 0xffcc66, 0xffcc66, 0x000000, 0xffcc66, 0xffcc66, 0xffcc66, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0xffcc66, 0x996600, 0x996600, 0xffcc66, 0xffcc66, 0xffcc66, 0x000000, 0xffcc66, 0xffcc66, 0xffcc66, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0x996600, 0xffcc66, 0xffcc66, 0xffcc66, 0xffcc66, 0x000000, 0x000000, 0x000000, 0x000000, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xffcc66, 0xffcc66, 0xffcc66, 0xffcc66, 0xffcc66, 0xffcc66, 0xffcc66, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0x0066cc, 0xff3300, 0x99ccff, 0xffffff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xffffff, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xffffff, 0xffffff, 0xffffff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0xffffff, 0xffffff, 0x0066cc, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xff3300, 0xffffff, 0xffffff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0x996600, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x0066cc, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0x996600, 0x0066cc, 0x0066cc, 0x0066cc, 0x99ccff, 0x0066cc, 0x0066cc, 0x0066cc, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0x996600, 0x996600, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}, 
                {0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x996600, 0x996600, 0x996600, 0x996600, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff, 0x99ccff}
            }
        };
};
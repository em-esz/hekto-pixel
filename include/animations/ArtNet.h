#include <ArtnetWifi.h>
#include <HektoPixel.h>

typedef std::function<void (uint8_t * data)> FrameCallbackFunc;

class ArtnetAnimation: public Animation {
    private:
        uint16_t universes[2] = {0, 480};
        ArtnetWifi artnet;
        uint8_t * displayBuffer = (uint8_t*) malloc(BOARD_DATA_SIZE);
        uint8_t * dmxBuffer = (uint8_t*) malloc(BOARD_DATA_SIZE);
        bool receviedUniverses[2] = {false, false};
        boolean displayFrame = false;
        Board * board;
    protected:
        void onDmxPacket(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t * data) {
            uint16_t universeStart = universes[universe - 1];
            for (int i = 0; i < length; i++) {
                dmxBuffer[universeStart + i] = data[i];
            }
            receviedUniverses[universe - 1] = true;

            if (receviedUniverses[0] && receviedUniverses[1]) {
                receviedUniverses[0] = false;
                receviedUniverses[1] = false;
                memcpy(displayBuffer, dmxBuffer, BOARD_DATA_SIZE);
                displayFrame = true;
            }            
        }
    public:
        ArtnetAnimation(): Animation() {
            using namespace std::placeholders;
            artnet.setArtDmxCallback(std::bind(&ArtnetAnimation::onDmxPacket, this, _1, _2, _3, _4));
            name = "artnet";
            interval = 0;
        }
        boolean renderFrame(Canvas &canvas) {
            artnet.read();
            if (displayFrame) {
                board->writeRawLedData(displayBuffer);
                displayFrame = false;
                return true;
            }
            return false;
        }
        void start(Board &board) {
            Animation::start(board);
            artnet.begin();
            this->board = &board;
        }

        void stop() {
            artnet.stop();
        }
};

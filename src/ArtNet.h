#include <ArtnetWifi.h>
#include <Animation.h>

typedef std::function<void (uint8_t * data)> FrameCallbackFunc;

class ArtnetClient {
    private:
        ArtnetWifi artnet;
        uint8_t *data = (uint8_t *)malloc(900);
        bool receviedUniverses[2] = {false, false};
        uint16_t universes[2] = {0, 480};
        FrameCallbackFunc onFrameCallback;
    protected:
        void onDmxPacket(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t * data) {
            uint16_t universeStart = universes[universe - 1];
            for (int i = 0; i < length; i++) {
                data[universeStart + i] = data[i];
            }
            receviedUniverses[universe - 1] = true;

            if (receviedUniverses[0] && receviedUniverses[1]) {
                receviedUniverses[0] = false;
                receviedUniverses[1] = false;
                onFrameCallback(data);
            }            
        }
    public:
        ArtnetClient(FrameCallbackFunc callback): onFrameCallback(callback) {
            using namespace std::placeholders;
            artnet.setArtDmxCallback(std::bind(&ArtnetClient::onDmxPacket, this, _1, _2, _3, _4));
            begin();
        }
        void update() {
            artnet.read();
        }
        void begin() {
            artnet.begin();
        }
        uint8_t* getData() {
            return data;
        }
        void setFrameCallback(FrameCallbackFunc onFrameFunc) {
            onFrameCallback = onFrameFunc;
        }
};

class ArtnetAnimation: public Animation {
    private:
        ArtnetClient artnetClient;
    protected:
        void onFrame(uint8_t *data) {

        }
    public:
        ArtnetAnimation(): Animation(), artnetClient([](uint8_t * data){}) {
            name = "artnet";
            interval = 0;
        }
        boolean renderFrame(Canvas *canvas) {
            artnetClient.update();
            return false;
        }
        void init(Canvas *canvas) {
            Animation::init(canvas);
            artnetClient.setFrameCallback([this](uint8_t *data) {
                this->onFrame(data);
            });
        }
};

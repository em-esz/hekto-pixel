#include <ArtnetWifi.h>

class ArtnetClient {
    private:
        ArtnetWifi artnet;
        uint8_t *data = (uint8_t *)malloc(1024);
        boolean sequences[2] = {false, false};
    protected:
        void onDmxPacket(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t * data) {
            if (universe == 1) {
                for (int i = 0; i < length; i++)
                    this->data[i] = data[i];
            }
            if (universe == 2) {
                for (int i = 0; i < length; i++)
                    this->data[480 + i] = data[i];
            }
        } 
    public:
        ArtnetClient() {
            using namespace std::placeholders;
            artnet.setArtDmxCallback(std::bind(&ArtnetClient::onDmxPacket, this, _1, _2, _3, _4));
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
};


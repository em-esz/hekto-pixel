#include <ArtnetWifi.h>
#include <HektoPixel.h>
#include <ESPAsyncWebServer.h>

typedef std::function<void (uint8_t * data)> FrameCallbackFunc;

class SketchClient: public Animation {
    private:
        boolean displayFrame = false;
        uint8_t *frameBuffer = (uint8_t *) malloc(BOARD_DATA_SIZE);
        Board * board;
        AsyncWebServer &server;
        AsyncWebSocket webSocket;
    protected:
        void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
            if (type == WS_EVT_DATA) {
                AwsFrameInfo * info = (AwsFrameInfo*)arg;
                if (info->opcode == WS_BINARY && info->final && info->index == 0 && info->len == len) {
                    memcpy(frameBuffer, data, BOARD_DATA_SIZE);
                    displayFrame = true;
                } else {
                    Serial.println(F("Not a binary or final message."));
                }
            }
        }
    public:
        SketchClient(AsyncWebServer &server): Animation(), server(server), webSocket("/sketch") {
            name = "sketch";
            interval = 0;
            using namespace std::placeholders;
            webSocket.onEvent(std::bind(&SketchClient::onWsEvent, this, _1, _2, _3, _4, _5, _6));
        }
        boolean renderFrame(Canvas &canvas) {
            webSocket.cleanupClients();
            if (displayFrame) {
                board->writeRawLedData(frameBuffer);
                displayFrame = false;
                return true;
            }
            return false;
        }
        void start(Board &board) {
            Animation::start(board);
            this->board = &board;
            server.addHandler(&webSocket);
        }

        void stop() {
            webSocket.closeAll();
            server.removeHandler(&webSocket);
        }
};

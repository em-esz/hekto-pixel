#include "ESPAsyncWebServer.h"
#include "Update.h"
using namespace std;

class WebOta {
    private:
        boolean shouldReboot_ = false;
    protected:
        void notFoundHandler(AsyncWebServerRequest *request){
          if (request->method() == HTTP_OPTIONS) { //CORS
            request->send(200);
          } else {
            request->send(404);
          }
        }

        void onFirmwareUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
            if (index == 0) {
                Serial.printf("Starting firmware update: %s\n", filename.c_str());
                if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { 
                Update.printError(Serial);
                }
            }

            if (Update.write(data, len) != len) {
                Update.printError(Serial);
            }

            if (final) {
                request->send(200);
                if (Update.end(true)) {
                    Serial.printf("Update Success: %u\n", index + len);
                    shouldReboot_ = true;
                } else {
                    Update.printError(Serial);
                }
            }
        }
    public:

        void init(AsyncWebServer *server) {
            server->onNotFound([this](AsyncWebServerRequest *request){
                this->notFoundHandler(request);
            });

            server->on("/update", HTTP_POST, [](AsyncWebServerRequest *request){
                    request->send(200);
            }, [this](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
                this->onFirmwareUpload(request, filename, index, data, len, final);
            });
        }

        boolean shouldReboot() {
            return shouldReboot_;
        }

};
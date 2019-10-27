#include "ESPAsyncWebServer.h"
#include "Update.h"
using namespace std;

class WebOta {
    private:
        boolean shouldReboot_ = false;
    protected:
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
            server->on("/update", HTTP_GET, [](AsyncWebServerRequest *request){
                request->send(200, "text/html", F("<html><head><title>Firmware update</title><body><form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form></body></html>"));
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
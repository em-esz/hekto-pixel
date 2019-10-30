#include "ESPAsyncWebServer.h"
#include "Update.h"
using namespace std;

class WebOta {
    private:
        boolean shouldReboot_ = false;
    protected:
        void notFoundHandler(AsyncWebServerRequest *request){
            Serial.printf("NOT_FOUND: ");
                if(request->method() == HTTP_GET)
                Serial.printf("GET");
                else if(request->method() == HTTP_POST)
                Serial.printf("POST");
                else if(request->method() == HTTP_DELETE)
                Serial.printf("DELETE");
                else if(request->method() == HTTP_PUT)
                Serial.printf("PUT");
                else if(request->method() == HTTP_PATCH)
                Serial.printf("PATCH");
                else if(request->method() == HTTP_HEAD)
                Serial.printf("HEAD");
                else if(request->method() == HTTP_OPTIONS) {
                    Serial.printf("OPTIONS");
                    request->send(200);
                } else
                Serial.printf("UNKNOWN");
                Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

                if(request->contentLength()){
                Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
                Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
                }

                int headers = request->headers();
                int i;
                for(i=0;i<headers;i++){
                AsyncWebHeader* h = request->getHeader(i);
                Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
                }

                int params = request->params();
                for(i=0;i<params;i++){
                AsyncWebParameter* p = request->getParam(i);
                if(p->isFile()){
                    Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
                } else if(p->isPost()){
                    Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
                } else {
                    Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
                }
                }

                request->send(404);
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
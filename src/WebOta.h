#include "ESPAsyncWebServer.h"
#include <SPIFFS.h>
#include <FS.h>
#include "Update.h"

using namespace std;

#include "esp_spiffs.h" 


class WebOta {
    private:
        size_t total = 0, used = 0;
        boolean shouldReboot_ = false;
        esp_err_t ret;
        File fsUploadFile;

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

        String listDir(fs::FS &fs, String dirname, uint8_t levels){
            String response = "[";
            Serial.printf("Listing directory: %s\r\n", dirname);

            File root = fs.open(dirname);
            if(!root){
                Serial.println("- failed to open directory");
                return "";
            }
            if(!root.isDirectory()){
                Serial.println(" - not a directory");
                return "";
            }

            File file = root.openNextFile();
            while(file){
                if (response != "[") response += ',';
                if(file.isDirectory()){ //Directory is not supported in espressif SPIFFS                   
                    Serial.print("  DIR : ");
                    Serial.println(file.name());
                    if(levels){
                        listDir(fs, file.name(), levels -1);
                    }
                } else {
                    Serial.print("  FILE: ");
                    Serial.print(file.name());
                    Serial.print("\tSIZE: ");
                    Serial.println(file.size());
                    response += String("{\"name\":\"") + String(file.name()) + String("\",\"size\":\"") + String(file.size()) + String("\"}");
                }
                file = root.openNextFile();
            }
            response += "]";
            return response;
        }

        void uploadHandler(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
            String path = "/";
            if (request->hasParam("path")) {
                path = request->getParam("path")->value();
            }   
            if (index == 0) {
                Serial.printf("Starting write file: %s\n", path + filename.c_str());
                fsUploadFile = SPIFFS.open(path + filename, "w");
            }

            if (fsUploadFile) fsUploadFile.write(data, len);

            if (final) {
                request->send(200, "text/plain", "Ok");
                Serial.printf("Writing file done!\n");
                if(fsUploadFile) fsUploadFile.close();
            }
        }

        void deleteHandler(AsyncWebServerRequest *request) {
            String path = "/";
            if (request->hasParam("path")) {
                path = request->getParam("path")->value();
            } else {
                request->send(500, "text/plain", "No path specified");
            }
            if(path == "/") request->send(500, "text/plain", "Bad path");
            if(!SPIFFS.exists(path)) request->send(404, "text/plain", "FileNotFound");
            SPIFFS.remove(path);
            request->send(200, "text/plain", "Ok");
        }

        void dirHandler(AsyncWebServerRequest *request) {
            ret = esp_spiffs_info(NULL, &total, &used);
            if (ret != ESP_OK) {
                Serial.printf("Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
            } else {
                Serial.printf("Partition size: total: %d, used: %d", total, used);
            }
            String path = "/";
            if (request->hasParam("path")) {
                path = request->getParam("path")->value();
            }          
            String res = String("{\"total\":\"") + String(total) + String("\",\"used\":\"") + String(used) + String("\",\"files\":");
            res += listDir(SPIFFS, path, 0);
            res += "}";
            request->send(200, "application/json", res);
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

        void init(AsyncWebServer &server) {
            server.onNotFound([this](AsyncWebServerRequest *request){
                this->notFoundHandler(request);
            });

            server.on("/spiffs", HTTP_GET,[this](AsyncWebServerRequest *request){ //read spiffs
                this->dirHandler(request);
            });

            server.on("/spiffs", HTTP_DELETE,[this](AsyncWebServerRequest *request){ //delete file
                this->deleteHandler(request);
            });

            server.on("/spiffs", HTTP_POST, [](AsyncWebServerRequest *request){
                    request->send(200);
            }, [this](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) { //upload file
                this->uploadHandler(request, filename, index, data, len, final);
            });

            server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request){
                    request->send(200);
            }, [this](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
                this->onFirmwareUpload(request, filename, index, data, len, final);
            });
        }

        boolean shouldReboot() {
            return shouldReboot_;
        }

};
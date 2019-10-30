// Global universe buffer
struct {
uint16_t length;
uint8_t sequence;
uint8_t *data;
} global;

// keep track of the timing of the function calls
long tic_fps = 0;
unsigned long packetCounter = 0;
float fps = 0;

void onDmxPacket(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t * data) {
    // print some feedback
    packetCounter++;
    if ((millis() - tic_fps) > 1000) {
        fps = 1000 * packetCounter / (millis() - tic_fps);
        tic_fps = millis();
        packetCounter = 0;
        Serial.print("Packets per second = ");
        Serial.print(fps);
        Serial.println();
    }

    if (universe == 1) {
        global.sequence = sequence;
        if (length < 480)
        global.length = length;
        for (int i = 0; i < global.length; i++)
        global.data[i] = data[i];
    }
    if (universe == 2) {
        global.sequence = sequence;
        if (length < 480)
        global.length = length;
        for (int i = 0; i < global.length; i++)
        global.data[480 + i] = data[i];
    }
} // onDmxpacket

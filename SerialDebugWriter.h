#ifndef SerialDebugWriter_h
#define SerialDebugWriter_h

#include "util/CharBuffer.h"

#define LINE_SIZE 100
#define DECIMAL_DIGITS 5

struct TrailerDebugData {
    int id;
    char *state;
    char *legAState;
    char *legBState;
    char *legCState;
    char *legDState;
    float pitch;
    float roll;
    float iqrPitch;
    float iqrRoll;
};

class SerialDebugWriter {
    public:
        void setup() {
            Serial2.begin(SERIAL_DEBUG_BAUD_RATE);
        }

        void write(TrailerDebugData *data) {
            addCsvInt(data->id);
            addCsvInt(millis());
            addCsvString(data->state);
            addCsvString(data->legAState);
            addCsvString(data->legBState);
            addCsvString(data->legCState);
            addCsvString(data->legDState);
            addCsvFloat(data->pitch);
            addCsvFloat(data->roll);
            addCsvFloat(data->iqrPitch);
            addCsvFloatLast(data->iqrRoll);
            charBuffer.append("\r\n");

            if (!charBuffer.canAppend(LINE_SIZE))
                flush();
        }

    private:
        long speed;
        char valueBuff[50];
        CharBuffer charBuffer;

        void flush() {
            Serial2.print(charBuffer.getBuffer());
            charBuffer.reset();
        }

        void addCsvString(const char *value) {
            charBuffer.append(value);
            charBuffer.append(",");
        }

        void addCsvInt(unsigned long value) {
            ltoa(value, valueBuff, 10);
            addCsvString(valueBuff);
        }

        void addCsvFloat(float value) {
            dtostrf(value, 7, DECIMAL_DIGITS, valueBuff);
            addCsvString(valueBuff);
        }

        void addCsvFloatLast(float value) {
            dtostrf(value, 7, DECIMAL_DIGITS, valueBuff);
            charBuffer.append(valueBuff);
        }
};

#endif

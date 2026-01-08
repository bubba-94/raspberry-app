#include <Arduino.h>

// Needs fixing
// Writes to the serial port where the Arduino is connected.

constexpr int POT = A0;
constexpr int LED = 9;

constexpr int ANALOG_MIN = 0;
constexpr int ANALOG_MAX = 1023;
constexpr int WEIGHT_MIN = 0;
constexpr int WEIGHT_MAX = 15000;

int readValue = 0;
int convValue = 0;

int readPot();

void setup()
{
    pinMode(LED, OUTPUT);

    Serial.begin(9600);
    while (!Serial){}
}

void loop()
{
    readValue = readPot();
    convValue = map(readValue, ANALOG_MIN, ANALOG_MAX, WEIGHT_MIN, WEIGHT_MAX);

    Serial.println(convValue);
    delay(16);
}

int readPot() {
    long sum = 0;
    const int samples = 100;

    for (int i = 0; i < samples; i++) {
        sum += analogRead(POT);
        delayMicroseconds(300);
    }

    return sum / samples;
}
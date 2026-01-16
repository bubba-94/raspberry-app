# Takeover

## Description

Take over the Raspberry Monitor opening a terminal that renderes graphics [SDL2 Library](https://wiki.libsdl.org/SDL3/FrontPage).

**Options:**
- Boot terminal when Raspberry Pi starts
- Fullscreen / borderless
- Render a picture 
- Render a QR code if possible, start with JPEG and later retrive endpoint from an API
- Colors
- Text

## Building 

### Prerequisites

- Sysroots of target headers
- Pre installed libraries SDL2, SDL2 image, SDL2 ttf.

build.sh will build and compile executables for both PC and Pi 5 and gather them under separate architecture folders under bin/

## Testing

### Testing on the PC 

- `./run.sh` from root repo to run the application.
- Testing aarch64 binary on PC `ssh -X user@pi` to forward x11 and open a SDL terminal on the desktop.

To create a virtual pair of serial ports.
```bash
socat \
  pty,raw,echo=0,link=/tmp/ttyRS232_A \
  pty,raw,echo=0,link=/tmp/ttyRS232_B &'
```

### Test programs for writing analog value to serial
```cpp
#include <Arduino.h>

constexpr int POT = A0;

constexpr int ANALOG_MIN = 0;
constexpr int ANALOG_MAX = 1023;
constexpr int WEIGHT_MIN = 0;
constexpr int WEIGHT_MAX = 15000;

int readValue = 0;
int convValue = 0;

int readPot();

void setup()
{
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
```


## Running

### Running on Pi

- Binary with the library is needed
- Run `scp -r bin/aarch64 user@pi:Programs/pay-per-weigh` to copy binary and library to Pi.
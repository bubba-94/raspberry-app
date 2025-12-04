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

## Running

### Running on Pi

- Binary with the library is needed
- Run `scp -r bin/aarch64 user@pi:Programs/` to copy binary and library to Pi.
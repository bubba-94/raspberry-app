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

### Pre requisites

- Sysroot of target environment

build.sh will build and compile executables for both PC and Pi 5 and gather them under separate architecture folders under bin/


## Testing

### Testing on the PC 

- `./run.sh` from root repo to run the x86 binary.
- Testing aarch64 binary on PC `ssh -X user@pi` to forward x11 and open a SDL terminal on the desktop.

- 

## Running
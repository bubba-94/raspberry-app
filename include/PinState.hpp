#ifdef RPI
    #ifndef PINSTATE_HPP
    #define PINSTATE_HPP

    struct PinState{
        bool shutdownRequested = false;
        bool keyEnabled = false;
    };

    #endif
#endif
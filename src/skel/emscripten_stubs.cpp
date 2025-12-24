// Stubs for Emscripten/WASM build
#ifdef __EMSCRIPTEN__

#include <emscripten.h>
#include "common.h"
#include "crossplatform.h"

// Platform variables
long _dwOperatingSystemVersion = 0;
RwUInt32 gGameState = 0;
uint32 _dwMemAvailPhys = 128 * 1024 * 1024; // 128MB

extern "C" {

// Platform functions
double psTimer(void) {
    return emscripten_get_now();
}

void _psSelectScreenVM(RwInt32 vm) {
    // No-op for Emscripten
}

} // extern "C"

#ifdef DETECT_JOYSTICK_MENU
char gSelectedJoystickName[128] = "";
#endif

// Minimal implementation for now - can be expanded
RwBool IsForegroundApp() {
    return TRUE;
}

#endif // __EMSCRIPTEN__

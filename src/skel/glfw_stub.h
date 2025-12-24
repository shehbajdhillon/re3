// Stub GLFW types for Emscripten/SDL2 build
#ifndef GLFW_STUB_H
#define GLFW_STUB_H

#ifdef LIBRW_SDL2

// Stub GLFWwindow - we'll use SDL_Window instead but need the type to exist
typedef void GLFWwindow;
typedef void GLFWmonitor;

// Stub GLFW functions (do nothing)
inline void glfwSetCursorPos(GLFWwindow*, double, double) {}
inline int glfwJoystickPresent(int) { return 0; }
inline const unsigned char* glfwGetJoystickButtons(int, int*) { return nullptr; }
inline const float* glfwGetJoystickAxes(int, int*) { return nullptr; }
inline const char* glfwGetJoystickName(int) { return ""; }
inline int glfwGetJoystickHats(int, int*) { return 0; }
inline void glfwSetWindowShouldClose(GLFWwindow*, int) {}
inline int glfwWindowShouldClose(GLFWwindow*) { return 0; }
inline void glfwGetCursorPos(GLFWwindow*, double*, double*) {}
inline int glfwGetKey(GLFWwindow*, int) { return 0; }
inline int glfwGetMouseButton(GLFWwindow*, int) { return 0; }
inline void glfwSetInputMode(GLFWwindow*, int, int) {}
inline void glfwSwapBuffers(GLFWwindow*) {}
inline void glfwPollEvents() {}
inline void glfwSetWindowPos(GLFWwindow*, int, int) {}
inline void glfwGetWindowPos(GLFWwindow*, int*, int*) {}
inline void glfwSetWindowSize(GLFWwindow*, int, int) {}
inline void glfwGetWindowSize(GLFWwindow*, int*, int*) {}
inline GLFWmonitor* glfwGetPrimaryMonitor() { return nullptr; }
inline void glfwGetMonitorPos(GLFWmonitor*, int*, int*) {}

#define GLFW_CURSOR 0x00033001
#define GLFW_CURSOR_NORMAL 0x00034001
#define GLFW_CURSOR_HIDDEN 0x00034002
#define GLFW_CURSOR_DISABLED 0x00034003

// Mouse buttons
#define GLFW_MOUSE_BUTTON_1                 0
#define GLFW_MOUSE_BUTTON_2                 1
#define GLFW_MOUSE_BUTTON_3                 2
#define GLFW_MOUSE_BUTTON_4                 3
#define GLFW_MOUSE_BUTTON_5                 4
#define GLFW_MOUSE_BUTTON_6                 5
#define GLFW_MOUSE_BUTTON_7                 6
#define GLFW_MOUSE_BUTTON_8                 7
#define GLFW_MOUSE_BUTTON_LEFT              GLFW_MOUSE_BUTTON_1
#define GLFW_MOUSE_BUTTON_RIGHT             GLFW_MOUSE_BUTTON_2
#define GLFW_MOUSE_BUTTON_MIDDLE            GLFW_MOUSE_BUTTON_3

// Gamepad button constants (matching GLFW values)
#define GLFW_GAMEPAD_BUTTON_A               0
#define GLFW_GAMEPAD_BUTTON_B               1
#define GLFW_GAMEPAD_BUTTON_X               2
#define GLFW_GAMEPAD_BUTTON_Y               3
#define GLFW_GAMEPAD_BUTTON_LEFT_BUMPER     4
#define GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define GLFW_GAMEPAD_BUTTON_BACK            6
#define GLFW_GAMEPAD_BUTTON_START           7
#define GLFW_GAMEPAD_BUTTON_GUIDE           8
#define GLFW_GAMEPAD_BUTTON_LEFT_THUMB      9
#define GLFW_GAMEPAD_BUTTON_RIGHT_THUMB     10
#define GLFW_GAMEPAD_BUTTON_DPAD_UP         11
#define GLFW_GAMEPAD_BUTTON_DPAD_RIGHT      12
#define GLFW_GAMEPAD_BUTTON_DPAD_DOWN       13
#define GLFW_GAMEPAD_BUTTON_DPAD_LEFT       14
#define GLFW_GAMEPAD_BUTTON_LAST            GLFW_GAMEPAD_BUTTON_DPAD_LEFT

// Gamepad axis constants
#define GLFW_GAMEPAD_AXIS_LEFT_X            0
#define GLFW_GAMEPAD_AXIS_LEFT_Y            1
#define GLFW_GAMEPAD_AXIS_RIGHT_X           2
#define GLFW_GAMEPAD_AXIS_RIGHT_Y           3
#define GLFW_GAMEPAD_AXIS_LEFT_TRIGGER      4
#define GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER     5
#define GLFW_GAMEPAD_AXIS_LAST              GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER

// Key constants
#define GLFW_KEY_SPACE                      32
#define GLFW_KEY_A                          65
#define GLFW_KEY_B                          66
#define GLFW_KEY_C                          67
#define GLFW_KEY_D                          68
#define GLFW_KEY_E                          69
#define GLFW_KEY_F                          70
#define GLFW_KEY_G                          71
#define GLFW_KEY_H                          72
#define GLFW_KEY_I                          73
#define GLFW_KEY_J                          74
#define GLFW_KEY_K                          75
#define GLFW_KEY_L                          76
#define GLFW_KEY_M                          77
#define GLFW_KEY_N                          78
#define GLFW_KEY_O                          79
#define GLFW_KEY_P                          80
#define GLFW_KEY_Q                          81
#define GLFW_KEY_R                          82
#define GLFW_KEY_S                          83
#define GLFW_KEY_T                          84
#define GLFW_KEY_U                          85
#define GLFW_KEY_V                          86
#define GLFW_KEY_W                          87
#define GLFW_KEY_X                          88
#define GLFW_KEY_Y                          89
#define GLFW_KEY_Z                          90
#define GLFW_KEY_ESCAPE                     256
#define GLFW_KEY_ENTER                      257
#define GLFW_KEY_TAB                        258
#define GLFW_KEY_BACKSPACE                  259
#define GLFW_KEY_INSERT                     260
#define GLFW_KEY_DELETE                     261
#define GLFW_KEY_RIGHT                      262
#define GLFW_KEY_LEFT                       263
#define GLFW_KEY_DOWN                       264
#define GLFW_KEY_UP                         265
#define GLFW_KEY_PAGE_UP                    266
#define GLFW_KEY_PAGE_DOWN                  267
#define GLFW_KEY_HOME                       268
#define GLFW_KEY_END                        269
#define GLFW_KEY_F1                         290
#define GLFW_KEY_F2                         291
#define GLFW_KEY_F3                         292
#define GLFW_KEY_F12                        301
#define GLFW_KEY_LEFT_SHIFT                 340
#define GLFW_KEY_LEFT_CONTROL               341
#define GLFW_KEY_LEFT_ALT                   342
#define GLFW_KEY_RIGHT_SHIFT                344
#define GLFW_KEY_RIGHT_CONTROL              345
#define GLFW_KEY_RIGHT_ALT                  346

#define GLFW_PRESS                          1
#define GLFW_RELEASE                        0
#define GLFW_REPEAT                         2

#define GLFW_CONNECTED                      0x00040001
#define GLFW_DISCONNECTED                   0x00040002

#endif // LIBRW_SDL2

#endif // GLFW_STUB_H

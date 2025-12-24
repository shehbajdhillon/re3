// Emscripten/WASM platform skeleton for reVC
#ifdef __EMSCRIPTEN__

#include <emscripten.h>
#include <emscripten/html5.h>
#include <SDL2/SDL.h>

#include "common.h"
#include "rwcore.h"
#include "skeleton.h"
#include "platform.h"
#include "crossplatform.h"

#include "main.h"
#include "FileMgr.h"
#include "Text.h"
#include "Pad.h"
#include "Timer.h"
#include "DMAudio.h"
#include "ControllerConfig.h"
#include "Frontend.h"
#include "Game.h"
#include "PCSave.h"
#include "MemoryCard.h"
#include "Sprite2d.h"
#include "AnimViewer.h"
#include "Font.h"

// Platform globals
long _dwOperatingSystemVersion = OS_WINXP;
RwUInt32 gGameState = GS_START_UP;
size_t _dwMemAvailPhys = 256 * 1024 * 1024; // 256MB

#ifdef DETECT_JOYSTICK_MENU
char gSelectedJoystickName[128] = "";
#endif

// SDL2 globals
static SDL_Window *gWindow = nil;
static SDL_GLContext gGLContext = nil;

// Engine params
rw::EngineOpenParams openParams;

// Platform state
static RwBool ForegroundApp = TRUE;
static RwBool RwInitialised = FALSE;

// Use the psGlobalType from crossplatform.h
static psGlobalType PsGlobal;

/*
 *****************************************************************************
 */
double
psTimer(void)
{
    return emscripten_get_now();
}

/*
 *****************************************************************************
 */
RwBool
psCameraBeginUpdate(RwCamera *camera)
{
    if (!RwCameraBeginUpdate(Scene.camera))
    {
        ForegroundApp = FALSE;
        RsEventHandler(rsACTIVATE, (void *)FALSE);
        return FALSE;
    }
    return TRUE;
}

/*
 *****************************************************************************
 */
void
psCameraShowRaster(RwCamera *camera)
{
    if (gWindow) {
        RwCameraShowRaster(camera, gWindow, 0);
        SDL_GL_SwapWindow(gWindow);
    }
}

/*
 *****************************************************************************
 */
RwImage *
psGrabScreen(RwCamera *pCamera)
{
    // TODO: Implement screen capture for Emscripten
    return nil;
}

/*
 *****************************************************************************
 */
void
psMouseSetPos(RwV2d *pos)
{
    if (gWindow) {
        SDL_WarpMouseInWindow(gWindow, (int)pos->x, (int)pos->y);
    }
    PSGLOBAL(lastMousePos) = *pos;
}

/*
 *****************************************************************************
 */
static RwMemoryFunctions memFuncs;

RwMemoryFunctions *
psGetMemoryFunctions(void)
{
    memFuncs.rwmalloc = malloc;
    memFuncs.rwfree = free;
    memFuncs.rwrealloc = realloc;
    memFuncs.rwcalloc = calloc;
    return &memFuncs;
}

/*
 *****************************************************************************
 */
RwBool
psInstallFileSystem(void)
{
    return TRUE;
}

/*
 *****************************************************************************
 */
RwBool
psNativeTextureSupport(void)
{
    return FALSE;
}

/*
 *****************************************************************************
 */
void _psCreateFolder(const char *path)
{
    // Emscripten filesystem handles this
}

/*
 *****************************************************************************
 */
const char *_psGetUserFilesFolder()
{
    static char szUserFiles[256];
    strcpy(szUserFiles, "/userfiles");
    return szUserFiles;
}

/*
 *****************************************************************************
 */
RwBool
psInitialize(void)
{
    PsGlobal.lastMousePos.x = PsGlobal.lastMousePos.y = 0.0f;
    RsGlobal.ps = &PsGlobal;

    PsGlobal.fullScreen = FALSE;
    PsGlobal.cursorIsInWindow = TRUE;
    PsGlobal.joy1id = -1;
    PsGlobal.joy2id = -1;

    CFileMgr::Initialise();

#ifdef PS2_MENU
    CPad::Initialise();
    CPad::GetPad(0)->Mode = 0;
    CGame::frenchGame = false;
    CGame::germanGame = false;
    CGame::nastyGame = true;
    CMenuManager::m_PrefsAllowNastyGame = true;
    FrontEndMenuManager.m_PrefsLanguage = CMenuManager::LANGUAGE_AMERICAN;
    FrontEndMenuManager.InitialiseMenuContentsAfterLoadingGame();
    TheMemoryCard.Init();
#else
    C_PcSave::SetSaveDirectory(_psGetUserFilesFolder());
    InitialiseLanguage();
#endif

    gGameState = GS_START_UP;

#ifndef PS2_MENU
    FrontEndMenuManager.LoadSettings();
#endif

    TheText.Unload();

    return TRUE;
}

/*
 *****************************************************************************
 */
void
psTerminate(void)
{
    if (gGLContext) {
        SDL_GL_DeleteContext(gGLContext);
        gGLContext = nil;
    }
    if (gWindow) {
        SDL_DestroyWindow(gWindow);
        gWindow = nil;
    }
    SDL_Quit();
}

/*
 *****************************************************************************
 */
void
_psSelectScreenVM(RwInt32 videoMode)
{
    // No-op for Emscripten - we use canvas size
}

/*
 *****************************************************************************
 */
RwBool
psSelectDevice()
{
    RwEngineSetVideoMode(0);
    return TRUE;
}

/*
 *****************************************************************************
 */
void InitialiseLanguage()
{
#ifndef PS2_MENU
    CGame::frenchGame = false;
    CGame::germanGame = false;
    CGame::nastyGame = true;

    // Default to English
    FrontEndMenuManager.m_PrefsLanguage = CMenuManager::LANGUAGE_AMERICAN;
#endif
}

/*
 *****************************************************************************
 */
void HandleExit()
{
    RsGlobal.quit = TRUE;
}

/*
 *****************************************************************************
 */
void _InputTranslateShiftKeyUpDown(RsKeyCodes* rs)
{
    // TODO: Implement if needed
}

long _InputInitialiseMouse(bool exclusive)
{
    return 0; // Success
}

void _InputShutdownMouse()
{
}

bool _InputMouseNeedsExclusive()
{
    return false;
}

void _InputInitialiseJoys()
{
}

void CapturePad(RwInt32 padID)
{
    // TODO: Implement gamepad capture for Emscripten
}

/*
 *****************************************************************************
 */
RwBool _psSetVideoMode(RwInt32 subSystem, RwInt32 videoMode)
{
    return TRUE;
}

RwChar** _psGetVideoModeList()
{
    static RwChar *modes[] = { (RwChar*)"Default", nil };
    return modes;
}

RwInt32 _psGetNumVideModes()
{
    return 1;
}

/*
 *****************************************************************************
 */
RwBool IsForegroundApp()
{
    return ForegroundApp;
}

/*
 *****************************************************************************
 */
// Key mapping from SDL2 to reVC
static RsKeyCodes SDLKeyToRsKey(SDL_Keycode key)
{
    switch (key) {
        case SDLK_ESCAPE: return rsESC;
        case SDLK_SPACE: return (RsKeyCodes)' ';
        case SDLK_RETURN: return rsENTER;
        case SDLK_TAB: return rsTAB;
        case SDLK_BACKSPACE: return rsBACKSP;
        case SDLK_INSERT: return rsINS;
        case SDLK_DELETE: return rsDEL;
        case SDLK_HOME: return rsHOME;
        case SDLK_END: return rsEND;
        case SDLK_PAGEUP: return rsPGUP;
        case SDLK_PAGEDOWN: return rsPGDN;
        case SDLK_UP: return rsUP;
        case SDLK_DOWN: return rsDOWN;
        case SDLK_LEFT: return rsLEFT;
        case SDLK_RIGHT: return rsRIGHT;
        case SDLK_LSHIFT: return rsLSHIFT;
        case SDLK_RSHIFT: return rsRSHIFT;
        case SDLK_LCTRL: return rsLCTRL;
        case SDLK_RCTRL: return rsRCTRL;
        case SDLK_LALT: return rsLALT;
        case SDLK_RALT: return rsRALT;
        case SDLK_F1: return rsF1;
        case SDLK_F2: return rsF2;
        case SDLK_F3: return rsF3;
        case SDLK_F4: return rsF4;
        case SDLK_F5: return rsF5;
        case SDLK_F6: return rsF6;
        case SDLK_F7: return rsF7;
        case SDLK_F8: return rsF8;
        case SDLK_F9: return rsF9;
        case SDLK_F10: return rsF10;
        case SDLK_F11: return rsF11;
        case SDLK_F12: return rsF12;
        case SDLK_NUMLOCKCLEAR: return rsNUMLOCK;
        case SDLK_SCROLLLOCK: return rsSCROLL;
        case SDLK_PAUSE: return rsPAUSE;
        case SDLK_CAPSLOCK: return rsCAPSLK;
        default:
            if (key >= SDLK_a && key <= SDLK_z) {
                return (RsKeyCodes)(key - SDLK_a + 'A');
            }
            if (key >= SDLK_0 && key <= SDLK_9) {
                return (RsKeyCodes)(key - SDLK_0 + '0');
            }
            if (key >= SDLK_KP_0 && key <= SDLK_KP_9) {
                return (RsKeyCodes)(key - SDLK_KP_0 + rsPADINS);
            }
            return rsNULL;
    }
}

/*
 *****************************************************************************
 */
static void HandleKeyEvent(SDL_Event *event)
{
    RsKeyCodes code = SDLKeyToRsKey(event->key.keysym.sym);
    if (code != rsNULL) {
        RsKeyStatus ks;
        ks.keyCharCode = code;

        if (event->type == SDL_KEYDOWN) {
            RsKeyboardEventHandler(rsKEYDOWN, &ks);
        } else {
            RsKeyboardEventHandler(rsKEYUP, &ks);
        }
    }
}

/*
 *****************************************************************************
 */
static void HandleMouseButtonEvent(SDL_Event *event)
{
    RsEvent rsEvent = (event->type == SDL_MOUSEBUTTONDOWN) ? rsLEFTBUTTONDOWN : rsLEFTBUTTONUP;

    switch (event->button.button) {
        case SDL_BUTTON_LEFT:
            rsEvent = (event->type == SDL_MOUSEBUTTONDOWN) ? rsLEFTBUTTONDOWN : rsLEFTBUTTONUP;
            break;
        case SDL_BUTTON_RIGHT:
            rsEvent = (event->type == SDL_MOUSEBUTTONDOWN) ? rsRIGHTBUTTONDOWN : rsRIGHTBUTTONUP;
            break;
        case SDL_BUTTON_MIDDLE:
            // Middle button - no direct event, but could use for wheel click
            return;
    }

    RsEventHandler(rsEvent, nil);
}

/*
 *****************************************************************************
 */
static void HandleMouseMotionEvent(SDL_Event *event)
{
    // Update mouse position delta for game camera control
    PSGLOBAL(lastMousePos).x = (float)event->motion.x;
    PSGLOBAL(lastMousePos).y = (float)event->motion.y;
}

/*
 *****************************************************************************
 */
static void HandleMouseWheelEvent(SDL_Event *event)
{
    // Mouse wheel events - these are handled differently
    // The skeleton handles them through pad/mouse input, not RsEventHandler
    // For now, skip wheel events
}

/*
 *****************************************************************************
 */
static void ProcessEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                RsGlobal.quit = TRUE;
                break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
                HandleKeyEvent(&event);
                break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                HandleMouseButtonEvent(&event);
                break;

            case SDL_MOUSEMOTION:
                HandleMouseMotionEvent(&event);
                break;

            case SDL_MOUSEWHEEL:
                HandleMouseWheelEvent(&event);
                break;

            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                        ForegroundApp = TRUE;
                        RsEventHandler(rsACTIVATE, (void *)TRUE);
                        break;
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                        ForegroundApp = FALSE;
                        RsEventHandler(rsACTIVATE, (void *)FALSE);
                        break;
                    case SDL_WINDOWEVENT_RESIZED:
                        {
                            RwRect r;
                            r.x = 0;
                            r.y = 0;
                            r.w = event.window.data1;
                            r.h = event.window.data2;
                            RsEventHandler(rsCAMERASIZE, &r);
                        }
                        break;
                }
                break;
        }
    }
}

/*
 *****************************************************************************
 */
static void MainLoopIteration()
{
    ProcessEvents();

    if (RsGlobal.quit) {
        emscripten_cancel_main_loop();
        RsEventHandler(rsTERMINATE, nil);
        return;
    }

    if (ForegroundApp) {
        switch (gGameState) {
            case GS_START_UP:
                gGameState = GS_INIT_ONCE;
                break;

            case GS_INIT_ONCE:
                LoadingScreen(nil, nil, "loadsc0");
                gGameState = GS_INIT_FRONTEND;
                break;

            case GS_INIT_FRONTEND:
#ifdef PS2_MENU
                gGameState = GS_FRONTEND;
#else
                FrontEndMenuManager.m_bGameNotLoaded = true;
                FrontEndMenuManager.m_bStartUpFrontEndRequested = true;
                gGameState = GS_FRONTEND;
#endif
                break;

            case GS_FRONTEND:
                // The menu system handles this
                RsEventHandler(rsIDLE, (void *)TRUE);
                break;

            case GS_INIT_PLAYING_GAME:
                InitialiseGame();
                gGameState = GS_PLAYING_GAME;
                break;

            case GS_PLAYING_GAME:
                // Main game loop - handled by Idle
                {
                    float ms = (float)CTimer::GetCurrentTimeInCycles() / (float)CTimer::GetCyclesPerMillisecond();
                    RsEventHandler(rsIDLE, &ms);
                }
                break;

            default:
                RsEventHandler(rsIDLE, (void *)TRUE);
                break;
        }
    }
}

/*
 *****************************************************************************
 */
void psPostRWinit(void)
{
    RwVideoMode vm;
    RwEngineGetVideoModeInfo(&vm, RwEngineGetCurrentVideoMode());

    RsGlobal.maximumWidth = vm.width;
    RsGlobal.maximumHeight = vm.height;

    // Init controls
    ControlsManager.MakeControllerActionsBlank();
    ControlsManager.InitDefaultControlConfiguration();
    ControlsManager.InitDefaultControlConfigMouse(MousePointerStateHelper.GetMouseSetUp());
}

/*
 *****************************************************************************
 */
extern "C" int main(int argc, char *argv[])
{
    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Set OpenGL attributes for WebGL2
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // Initialize platform-independent data
    if (RsEventHandler(rsINITIALIZE, nil) == rsEVENTERROR) {
        return -1;
    }

    // Create window
    gWindow = SDL_CreateWindow(
        RsGlobal.appName,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        RsGlobal.maximumWidth,
        RsGlobal.maximumHeight,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!gWindow) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Create OpenGL context
    gGLContext = SDL_GL_CreateContext(gWindow);
    if (!gGLContext) {
        printf("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_GL_MakeCurrent(gWindow, gGLContext);

    // Setup engine params
    openParams.width = RsGlobal.maximumWidth;
    openParams.height = RsGlobal.maximumHeight;
    openParams.windowtitle = RsGlobal.appName;
    openParams.window = &gWindow;

    // Initialize RenderWare
    if (RsEventHandler(rsRWINITIALIZE, &openParams) == rsEVENTERROR) {
        RsEventHandler(rsTERMINATE, nil);
        return -1;
    }

    psPostRWinit();

    // Set initial camera size
    {
        RwRect r;
        r.x = 0;
        r.y = 0;
        r.w = RsGlobal.maximumWidth;
        r.h = RsGlobal.maximumHeight;
        RsEventHandler(rsCAMERASIZE, &r);
    }

    RwInitialised = TRUE;

    // Set mouse to center
    {
        RwV2d pos;
        pos.x = RsGlobal.maximumWidth * 0.5f;
        pos.y = RsGlobal.maximumHeight * 0.5f;
        RsMouseSetPos(&pos);
    }

    // Start main loop
    // 0 = use requestAnimationFrame (browser's frame rate)
    // 1 = simulate infinite loop
    emscripten_set_main_loop(MainLoopIteration, 0, 1);

    return 0;
}

#endif // __EMSCRIPTEN__

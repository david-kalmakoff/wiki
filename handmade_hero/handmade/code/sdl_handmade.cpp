#include "../libs/logging/logging.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_haptic.h>
#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <cstring>
#include <errno.h>
#include <stdint.h>
#include <sys/mman.h>

#define internal static
#define local_persist static
#define global_variable static

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32_t bool32;

struct sdl_offscreen_buffer {
  // NOTE: Pixels are always 32-bit wide, order: BB GG RR XX
  SDL_Texture *Texture;
  void *Memory;
  int Width;
  int Height;
  int Pitch;
};

struct sdl_window_dimension {
  int Width;
  int Height;
};

global_variable sdl_offscreen_buffer GlobalBackBuffer;

#define MAX_CONTROLLERS 4
SDL_GameController *ControllerHandles[MAX_CONTROLLERS];
SDL_Haptic *RumbleHandles[MAX_CONTROLLERS];

sdl_window_dimension SDLGetWindowDimensions(SDL_Window *Window) {
  sdl_window_dimension Result;
  SDL_GetWindowSize(Window, &Result.Width, &Result.Height);
  return (Result);
}

internal void RenderWeirdGradient(sdl_offscreen_buffer *Buffer, int BlueOffset,
                                  int GreenOffset) {
  uint8 *Row = (uint8 *)Buffer->Memory;
  for (int Y = 0; Y < Buffer->Height; Y++) {
    uint32 *Pixel = (uint32 *)Row;
    for (int X = 0; X < Buffer->Width; X++) {
      uint8 Blue = (X + BlueOffset);
      uint8 Green = (Y + GreenOffset);

      *Pixel++ = ((Green << 8) | Blue);
    }
    Row += Buffer->Pitch;
  }
}

internal void SDLResizeTexture(sdl_offscreen_buffer *Buffer,
                               SDL_Renderer *Renderer, int Width, int Height) {
  if (Buffer->Memory) {
    // free(Buffer->Memory);
    munmap(Buffer->Memory, Buffer->Height * Buffer->Pitch);
  }
  if (Buffer->Texture) {
    SDL_DestroyTexture(Buffer->Texture);
  }

  const int BytesPerPixel = 4;
  Buffer->Pitch = Width * BytesPerPixel;

  Buffer->Texture =
      SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_STREAMING, Width, Height);
  Buffer->Width = Width;
  Buffer->Height = Height;

  // Buffer->Memory = malloc(Buffer->Width * Buffer->Height *
  // Buffer->BytesPerPixel);
  Buffer->Memory =
      mmap(0, Buffer->Height * Buffer->Pitch, PROT_READ | PROT_WRITE,
           MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
}

internal void SDLUpdateWindow(SDL_Renderer *Renderer,
                              sdl_offscreen_buffer *Buffer) {
  if (SDL_UpdateTexture(Buffer->Texture, 0, Buffer->Memory, Buffer->Pitch)) {
    PRINTF(ERROR, "SDL_UpdateTexture did not work: %s", strerror(errno));
  }
  SDL_RenderCopy(Renderer, Buffer->Texture, 0, 0);
  SDL_RenderPresent(Renderer);
}

bool HandleEvent(SDL_Event *Event) {
  bool ShouldQuit = false;

  switch (Event->type) {

  case SDL_KEYDOWN:
  case SDL_KEYUP: {
    SDL_Keycode KeyCode = Event->key.keysym.sym;

    bool IsDown = (Event->key.state == SDL_PRESSED);
    bool WasDown = false;
    if (Event->key.state == SDL_RELEASED) {
      WasDown = true;
    } else if (Event->key.repeat != 0) {
      WasDown = true;
    }

    // NOTE: In the windows version, we used "if (IsDown != WasDown)"
    // to detect key repeats. SDL has the 'repeat' value, though,
    // which we'll use.
    if (Event->key.repeat == 0) {
      if (KeyCode == SDLK_w) {
      } else if (KeyCode == SDLK_a) {
      } else if (KeyCode == SDLK_s) {
      } else if (KeyCode == SDLK_d) {
      } else if (KeyCode == SDLK_q) {
      } else if (KeyCode == SDLK_e) {
      } else if (KeyCode == SDLK_UP) {
      } else if (KeyCode == SDLK_LEFT) {
      } else if (KeyCode == SDLK_DOWN) {
      } else if (KeyCode == SDLK_RIGHT) {
      } else if (KeyCode == SDLK_ESCAPE) {
        printf("ESCAPE: ");
        if (IsDown) {
          printf("IsDown ");
        }
        if (WasDown) {
          printf("WasDown");
        }
        printf("\n");
      } else if (KeyCode == SDLK_SPACE) {
      }
    }

    bool AltKeyWaskDown = (Event->key.keysym.mod & KMOD_ALT);
    if (KeyCode == SDLK_F4 && AltKeyWaskDown) {
      ShouldQuit = true;
    }
  } break;

  case SDL_QUIT: {
    PRINT(INFO, "SDL_QUIT");
    ShouldQuit = true;
  } break;

  case SDL_WINDOWEVENT: {
    switch (Event->window.event) {

    case SDL_WINDOWEVENT_SIZE_CHANGED: {
      SDL_Window *Window = SDL_GetWindowFromID(Event->window.windowID);
      SDL_Renderer *Renderer = SDL_GetRenderer(Window);
      PRINTF(INFO, "window size changed (%d %d)\n", Event->window.data1,
             Event->window.data2);
    } break;

    case SDL_WINDOWEVENT_FOCUS_GAINED: {
      PRINT(INFO, "window focus gained");
    } break;

    case SDL_WINDOWEVENT_EXPOSED: {
      SDL_Window *Window = SDL_GetWindowFromID(Event->window.windowID);
      SDL_Renderer *Renderer = SDL_GetRenderer(Window);
      SDLUpdateWindow(Renderer, &GlobalBackBuffer);
    } break;
    }

  } break;
  }

  return (ShouldQuit);
}

internal void SDLOpenGameControllers() {
  int MaxJoysticks = SDL_NumJoysticks();

  int ControllerIndex = 0;

  for (int JoystickIndex = 0; JoystickIndex < MaxJoysticks; JoystickIndex++) {
    if (!SDL_IsGameController(JoystickIndex)) {
      continue;
    }

    if (ControllerIndex >= MAX_CONTROLLERS) {
      break;
    }

    PRINTF(INFO, "opening controller %d", ControllerIndex);

    ControllerHandles[ControllerIndex] = SDL_GameControllerOpen(JoystickIndex);

    // add haptic
    RumbleHandles[ControllerIndex] = SDL_HapticOpen(JoystickIndex);

    if (RumbleHandles[ControllerIndex] &&
        SDL_HapticRumbleInit(RumbleHandles[ControllerIndex]) != 0) {
      PRINTF(INFO, "controller %d has no rumble", ControllerIndex);

      SDL_HapticClose(RumbleHandles[ControllerIndex]);
      RumbleHandles[ControllerIndex] = 0;
    } else {
      PRINTF(INFO, "controller %d rumble loaded", ControllerIndex);
    }

    ControllerIndex++;
  }
}

internal void SDLCloseGameControllers() {
  for (int ControllerIndex = 0; ControllerIndex < MAX_CONTROLLERS;
       ControllerIndex++) {
    if (ControllerHandles[ControllerIndex]) {
      PRINTF(INFO, "closing controller %d", ControllerIndex);

      if (RumbleHandles[ControllerIndex]) {
        SDL_HapticClose(RumbleHandles[ControllerIndex]);
      }
      SDL_GameControllerClose(ControllerHandles[ControllerIndex]);
    }
  }
}

internal void SDLAudioCallback(void *UserData, uint8 *AudioData, int Length) {
  // Clear out our audio buffer to silence
  memset(AudioData, 0, Length);
}

internal void SDLInitAudio(int32 SamplesPerSecond, int32 BufferSize) {
  SDL_AudioSpec AudioSettings = {0};

  AudioSettings.freq = SamplesPerSecond;
  AudioSettings.format = AUDIO_S16LSB;
  AudioSettings.channels = 2;
  AudioSettings.samples = BufferSize;
  AudioSettings.callback = &SDLAudioCallback;

  SDL_OpenAudio(&AudioSettings, 0);

  PRINTF(INFO, "initialised an audio device at frequency %d Hz, %d Channels",
         AudioSettings.freq, AudioSettings.channels);

  if (AudioSettings.format != AUDIO_S16LSB) {
    PRINT(ERROR, "we did not get AUDIO_S16LSB as our sample format");
    SDL_CloseAudio();
  }

  SDL_PauseAudio(0);
}

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC |
               SDL_INIT_AUDIO) != 0) {
    PRINT(ERROR, "SDL_INIT did not work");
    return (1);
  }

  SDLOpenGameControllers();

  PRINT(INFO, "starting the window");

  SDL_Window *Window =
      SDL_CreateWindow("Handmade Hero", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);

  if (Window) {
    PRINT(INFO, "starting the renderer");

    SDL_Renderer *Renderer = SDL_CreateRenderer(Window, -1, 0);

    if (Renderer) {
      bool Running = true;
      SDLResizeTexture(&GlobalBackBuffer, Renderer, 1280, 720);
      int XOffset = 0;
      int YOffset = 0;

      // NOTE: Sound test
      int SamplesPerSecond = 48000;
      int ToneHz = 256;
      int16 ToneVolume = 3000;
      uint32 RunningSampleIndex = 0;
      int SquareWavePeriod = SamplesPerSecond / ToneHz;
      int HalfSquareWavePeriod = SquareWavePeriod / 2;
      int BytesPerSample = sizeof(int16) * 2;
      int SecondaryBufferSize = SamplesPerSecond * BytesPerSample;

      SDLInitAudio(48000, SecondaryBufferSize);
      bool SoundIsPlaying = false;

      while (Running) {
        SDL_Event Event;
        while (SDL_PollEvent(&Event)) {
          if (HandleEvent(&Event)) {
            Running = false;
          }
        }

        for (int ControllerIndex = 0; ControllerIndex < MAX_CONTROLLERS;
             ControllerIndex++) {
          if (ControllerHandles[ControllerIndex] != 0 &&
              SDL_GameControllerGetAttached(
                  ControllerHandles[ControllerIndex])) {
            // NOTE: We have a controller with index ControllerIndex
            bool Up =
                SDL_GameControllerGetButton(ControllerHandles[ControllerIndex],
                                            SDL_CONTROLLER_BUTTON_DPAD_UP);
            bool Down =
                SDL_GameControllerGetButton(ControllerHandles[ControllerIndex],
                                            SDL_CONTROLLER_BUTTON_DPAD_DOWN);
            bool Left =
                SDL_GameControllerGetButton(ControllerHandles[ControllerIndex],
                                            SDL_CONTROLLER_BUTTON_DPAD_LEFT);
            bool Right =
                SDL_GameControllerGetButton(ControllerHandles[ControllerIndex],
                                            SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
            bool Start =
                SDL_GameControllerGetButton(ControllerHandles[ControllerIndex],
                                            SDL_CONTROLLER_BUTTON_START);
            bool Back = SDL_GameControllerGetButton(
                ControllerHandles[ControllerIndex], SDL_CONTROLLER_BUTTON_BACK);
            bool LeftShoulder =
                SDL_GameControllerGetButton(ControllerHandles[ControllerIndex],
                                            SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
            bool RightShoulder = SDL_GameControllerGetButton(
                ControllerHandles[ControllerIndex],
                SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
            bool AButton = SDL_GameControllerGetButton(
                ControllerHandles[ControllerIndex], SDL_CONTROLLER_BUTTON_A);
            bool BButton = SDL_GameControllerGetButton(
                ControllerHandles[ControllerIndex], SDL_CONTROLLER_BUTTON_B);
            bool XButton = SDL_GameControllerGetButton(
                ControllerHandles[ControllerIndex], SDL_CONTROLLER_BUTTON_X);
            bool YButton = SDL_GameControllerGetButton(
                ControllerHandles[ControllerIndex], SDL_CONTROLLER_BUTTON_Y);

            int16 StickX = SDL_GameControllerGetAxis(
                ControllerHandles[ControllerIndex], SDL_CONTROLLER_AXIS_LEFTX);
            int16 StickY = SDL_GameControllerGetAxis(
                ControllerHandles[ControllerIndex], SDL_CONTROLLER_AXIS_LEFTY);

            if (AButton) {
              YOffset += 2;
            }

            if (BButton) {
              if (RumbleHandles[ControllerIndex]) {
                SDL_HapticRumblePlay(RumbleHandles[ControllerIndex], 0.5f, 50);
              }
            }
          } else {
            // TODO: This controller is not plugged in
          }
        }

        RenderWeirdGradient(&GlobalBackBuffer, XOffset, YOffset);
        SDLUpdateWindow(Renderer, &GlobalBackBuffer);

        XOffset++;
      }
    } else {
      PRINT(ERROR, "renderer could not be created");
    }
  } else {
    PRINT(ERROR, "window could not be created");
  }

  SDLCloseGameControllers();

  PRINT(INFO, "quiting the application");
  SDL_Quit();
  return (0);
}

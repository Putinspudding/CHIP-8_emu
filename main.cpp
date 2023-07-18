#include "Chip8CPU.h"
#include <SDL2/SDL.h>

// Screen dimension constants
const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;

// Starts up SDL and creates window
bool init();

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

// The image we will load and show on the screen
SDL_Surface *gHelloWorld = NULL;

SDL_Renderer *renderTarget = NULL;

SDL_Texture *texture = NULL;

// uint32_t *pixels = new uint32_t[2048];

int pitch;

bool init()
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        // Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        renderTarget = SDL_CreateRenderer(gWindow, -1, 0);
        texture = SDL_CreateTexture(renderTarget,
                                    SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 64, 32);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            // Get window surface
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    if (SDL_Init(SDL_INIT_TIMER) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success &= false;
    }
    return success;
}

Uint32 clockLoop(Uint32 interval, void *param)
{
    Chip8CPU *cpu = static_cast<Chip8CPU *>(param);
    SDL_LockTexture(texture, NULL, (void **)cpu->pixels, &pitch);
    for (int i = 0; i < 2048; i++)
    {
        if (!cpu->screenData[i])
        {
            cpu->pixels[i] = 0000;
        }
        else
        {
            cpu->pixels[i] = 0b00101011;
        }
    }
    SDL_UnlockTexture(texture);

    SDL_RenderClear(renderTarget);

    SDL_RenderCopy(renderTarget, texture, NULL, NULL);

    SDL_RenderPresent(renderTarget);
    cpu->executeNextOpcode();
    cpu->decreaseTimers();
    cpu->test();
    return 16;
}

int main(int argc, char *args[])
{

    Chip8CPU cpu = Chip8CPU();
    cpu.reset();
    init();
    SDL_TimerID gTimer = SDL_AddTimer(16, clockLoop, &cpu);
    /* code */
    bool quit = false;

    // Event handler
    SDL_Event e;

    // While application is running
    while (!quit)
    {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
    }

    // Remove timer after main loop

    SDL_RemoveTimer(gTimer);
    SDL_DestroyWindow(gWindow);

    // Quit SDL subsystems
    SDL_Quit();
}
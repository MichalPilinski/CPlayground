#include <printf.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdarg.h>

#include "camera/camera.h"
#include "distance_providers/distance_aggregator.h"

// Program-persisted variables
SDL_Window *window;
SDL_Renderer *renderer;

TTF_Font *font;

int windowWidth = 800;
int windowHeight = 640;

SDL_Rect headerTextRect;
SDL_Texture *headerText;
int defaultMargin = 5;

SDL_Rect sampleRect = {.x = 10, .y = 10, .w = 100, .h = 100};
SDL_bool inSampleRect = SDL_FALSE;
SDL_Color white = {255, 255, 255};

double cameraFocalLength = 1;

void initSdl()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("[Error] SDL Init : %s \n", SDL_GetError());
    } else {
        printf("SDL INITIALISED\n");
        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);

        printf("Display mode is %dx%dpx @ %dhz\n", dm.w, dm.h, dm.refresh_rate);
    }
}

void initWindowAndRenderer()
{
    int windowCreationCode = SDL_CreateWindowAndRenderer(windowWidth, windowHeight, SDL_WINDOW_SHOWN, &window, &renderer);
    if (windowCreationCode != 0) {
        printf("[Error] Creating Window and Renderer: %s\n", SDL_GetError());
        exit(0);
    } else {
        printf("Created Window and Renderer %dx%d\n", windowWidth, windowHeight);
    }
}

void initTtf()
{
  TTF_Init();
}

void setupHeaderText()
{
    font = TTF_OpenFont("resources/OpenSans-Regular.ttf", 18);

    SDL_Surface *textSurface = TTF_RenderText_Blended(font, "Press [Escape] to Exit", white);
    headerText = SDL_CreateTextureFromSurface(renderer, textSurface);

    headerTextRect.x = defaultMargin;
    headerTextRect.y = 0;
    headerTextRect.w = textSurface->w;
    headerTextRect.h = textSurface->h;

    SDL_FreeSurface(textSurface);

    textSurface = NULL;
}

void setupWindowIcon()
{
    SDL_Surface *iconSurface;
    iconSurface = IMG_Load("resources/appicon.jpg");

    SDL_SetWindowIcon(window, iconSurface);

    SDL_FreeSurface(iconSurface);
}

void setupCamera()
{
    double sensorWidth = (double) windowWidth / windowHeight;
    initCamera(sensorWidth, 1, windowWidth, windowHeight, cameraFocalLength);

    setCameraPosition(&(struct Point3D) {0, 0, 0});
    setCameraTarget(&(struct Point3D){10, 10, 10});
}

void renderCameraData()
{
    SDL_Rect cameraTextRect;
    SDL_Texture *cameraHeaderText;

    struct Point3D cameraPosition = getCameraPosition();
    struct Point3D cameraTarget = getCameraTarget();

    char buffer[500];
    snprintf (buffer, 500, "Camera pos: [%.1f, %.1f, %.1f], target: [%.1f, %.1f, %.1f]",
              cameraPosition.x, cameraPosition.y, cameraPosition.z,
              cameraTarget.x, cameraTarget.y, cameraTarget.z);

    SDL_Surface *cameraTextSurface = TTF_RenderText_Blended(font, buffer, white);
    cameraHeaderText = SDL_CreateTextureFromSurface(renderer, cameraTextSurface);

    cameraTextRect.x = defaultMargin;
    cameraTextRect.y = windowHeight - 30;
    cameraTextRect.w = cameraTextSurface->w;
    cameraTextRect.h = cameraTextSurface->h;

    SDL_FreeSurface(cameraTextSurface);

    cameraTextSurface = NULL;

    SDL_RenderCopy(renderer, cameraHeaderText, NULL, &cameraTextRect);
}

void handleMouseDrag(SDL_Event e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        SDL_Point clickCoords = {.x = e.motion.x, .y = e.motion.y};

        if (SDL_PointInRect(&clickCoords, &sampleRect)) {
            inSampleRect = SDL_TRUE;
        }
    }

    if (e.type == SDL_MOUSEBUTTONUP && inSampleRect == SDL_TRUE) {
        inSampleRect = SDL_FALSE;
    }

    if (e.type == SDL_MOUSEMOTION && inSampleRect == SDL_TRUE) {
        sampleRect.x += e.motion.xrel;
        sampleRect.y += e.motion.yrel;
    }
}

/**
 * Handles keystrokes and other inputs
 * @return True if program should exit main loop, false otherwise
 */
SDL_bool handleInputs()
{
    SDL_Event event;

    // Allow quiting with escape key by polling for pending events
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return SDL_FALSE;
        }

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            return SDL_FALSE;
        }

        handleMouseDrag(event);
    }

    return SDL_TRUE;
}

void setupEntities()
{
    addSphere(&(struct Point3D) {10, 10, 10}, 1);

    double distance = getSceneDistance(&(struct Point3D) {0, 0, 0});
    printf("Distance: [%f]", distance);
}

void initScene()
{
    initSdl();
    initWindowAndRenderer();
    initTtf();

    setupWindowIcon();
    setupHeaderText();

    setupCamera();
    setupEntities();
}

void mainLoop()
{
    for (;;) {
        if(!handleInputs()) return;

        // Blank out the renderer with all black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        // Render the sample rectangle
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);
        SDL_RenderFillRect(renderer, &sampleRect);

        // Render sample text
        SDL_RenderCopy(renderer, headerText, NULL, &headerTextRect);
        renderCameraData();

        // Present to renderer
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
}

void destroyScene()
{
    SDL_DestroyTexture(headerText);
    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyWindow(window);
}

/**
 * Main entry point
 * @return exit code
 */
int main()
{
    initScene();
    mainLoop();
    destroyScene();

    SDL_Quit();
}
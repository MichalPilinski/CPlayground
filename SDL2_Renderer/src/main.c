#include <printf.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

// Program-persisted variables
SDL_Window *window;
SDL_Renderer *renderer;

TTF_Font *font;

int windowWidth = 640;
int windowHeight = 480;

SDL_Rect headerTextRect;
SDL_Texture *headerText;
int defaultMargin = 5;

SDL_Rect sampleRect = {.x = 10, .y = 10, .w = 100, .h = 100};
SDL_bool inSampleRect = SDL_FALSE;
SDL_Color white = {255, 255, 255};

void init_sdl()
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

void init_window_and_renderer()
{
    int windowCreationCode = SDL_CreateWindowAndRenderer(windowWidth, windowHeight, SDL_WINDOW_SHOWN, &window, &renderer);
    if (windowCreationCode != 0) {
        printf("[Error] Creating Window and Renderer: %s\n", SDL_GetError());
        exit(0);
    } else {
        printf("Created Window and Renderer %dx%d\n", windowWidth, windowHeight);
    }
}

void init_ttf()
{
  TTF_Init();
}

void setup_header_text()
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

void setup_window_icon()
{
    SDL_Surface *iconSurface;
    iconSurface = IMG_Load("resources/appicon.jpg");

    SDL_SetWindowIcon(window, iconSurface);

    SDL_FreeSurface(iconSurface);
}

void handle_mouse_drag(SDL_Event e)
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
SDL_bool handle_inputs()
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

        handle_mouse_drag(event);
    }

    return SDL_TRUE;
}

void main_loop()
{
    for (;;) {
        if(!handle_inputs()) return;

        // Blank out the renderer with all black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        // Render the sample rectangle
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);
        SDL_RenderFillRect(renderer, &sampleRect);

        // Render sample text
        SDL_RenderCopy(renderer, headerText, NULL, &headerTextRect);

        // Present to renderer
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
}

void init_scene()
{
    init_sdl();
    init_window_and_renderer();
    init_ttf();

    setup_window_icon();
    setup_header_text();
}

void destroy_scene()
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
    init_scene();
    main_loop();
    destroy_scene();

    SDL_Quit();
    exit(0);
}
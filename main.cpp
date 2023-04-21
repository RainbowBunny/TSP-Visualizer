// Custom library
#include <mainloop.h>

// SDL2 library
#include <SDL2/SDL.h>

// Standard library
#include <iostream>
#include <utility>

const char* gameTitle = "Game";

// Remember, do not forget argc, argv
int main(int argc, char **argv) {
    
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;

    initSDL(gWindow, gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT, gameTitle);
    // Creating game object
    Gallery gGallery(gRenderer);

    // Game game(28, 1, 1, 7, 7, 200, 20, gRenderer, gGallery);

    std::cout << "Initiated gallery" << std::endl;
    std::cout << SDL_GetError();
    
    // Event handler
    SDL_Event e;

    // Time frame to maintain a constant FPS
    Uint32 frameStart;
    int frameTime;

    MainLoop gameLoop(gRenderer, gGallery);

    // Main game loop.
    while (gameLoop.getGameState() != QUITTING_THE_GAME) {
        // Get the number of ticks at the start of the loop
        // std::cout << "Beginning the loop" << std::endl;
        frameStart = SDL_GetTicks();

        SDL_SetRenderDrawColor(gRenderer, WHITE_COLOR.r, WHITE_COLOR.g, WHITE_COLOR.b, WHITE_COLOR.a);
        SDL_RenderClear(gRenderer);

        while (SDL_PollEvent(&e)) {
            gameLoop.handleUserInput(e, gRenderer, gGallery);
        }

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        gameLoop.renderGame(gRenderer, gGallery, mouseX, mouseY);

        // std::cout << "Presenting the game" << std::endl;
        SDL_RenderPresent(gRenderer);

        // Delay to maintain the current FPS

        frameTime = SDL_GetTicks() - frameStart;
        std::cout << frameTime << std::endl;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
    quitSDL(gWindow, gRenderer);
    return 0;
}
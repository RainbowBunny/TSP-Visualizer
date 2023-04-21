#include <mainloop.h>

/* 
enum GameState {
    STARTING_SCREEN = 0,
    REWRITING, 
    REPOSITIONING,
    TRACKING,
    QUITTING_THE_GAME
};

*/

MainLoop::MainLoop(SDL_Renderer* &renderer, Gallery &gallery) {
    menu = loadMenuFromFile("data/box.txt", renderer, gallery);
    game = Game({0, 0, 950, 950}, 20);
    game.TSPInit();
}

void MainLoop::renderGame(SDL_Renderer* &renderer, Gallery &gallery, int mouseX, int mouseY) {
    // std::cout << "Mainloop rendering..." << std::endl;
    switch (gameState)
    {
    case QUITTING_THE_GAME: {
        break;
    }
    
    case STARTING_SCREEN: {
        break;
    }

    case TRACKING: {
        game.tracking(mouseX, mouseY);
        break;
    }
    
    default : {
        break;
    }
        
    }

    menu.renderMenu(renderer, gallery, mouseX, mouseY);
    game.renderGame(renderer, gallery);
}

void MainLoop::handleUserInput(SDL_Event e, SDL_Renderer* &renderer, Gallery &gallery) {
    switch (e.type) {
        case SDL_QUIT: {
            updateGameState(QUITTING_THE_GAME);
            break;
        }

        case SDL_MOUSEBUTTONDOWN: {
            if (game.updateTarget(e.motion.x, e.motion.y)) {
                updateGameState(TRACKING);
            }
            break;
        }

        case SDL_MOUSEBUTTONUP: {
            updateGameState(STARTING_SCREEN);
            break;
        }

        default: {
            break;
        }
    }
}
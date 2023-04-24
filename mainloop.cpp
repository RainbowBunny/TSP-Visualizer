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
    editor = loadMenuFromFile("data/editEngine.txt", renderer, gallery);
    game = Game({0, 0, 800, 800}, 4);
    game.TSPInit();
    game.SolveTSP();
    menu.updateBothButton("NPoints", "NPoints: " + std::to_string(game.getNumberOfVertexes()));
    menu.updateBothButton("MinimumDistance", std::to_string(game.getAnswerValue()));
    menu.updateBothButton("Hide/Show Paths", "Hide/Show Paths");
    editor.updateBothButton("Editing Points", "Enter a number between 1 and 20: ");
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

    case REWRITING: {
        editor.renderMenu(renderer, gallery, mouseX, mouseY);
        return;
        break;
    }
    
    default : {
        break;
    }
        
    }

    if (gameState != TRACKING) {
        if (game.getDrawState()) {
            menu.updateBothButton("MinimumDistance", std::to_string(game.getAnswerValue()));
        } else {
            menu.updateBothButton("MinimumDistance", "");
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
            } else {
                std::string pressedButton = menu.getPressedButton(e.motion.x, e.motion.y);
                if (pressedButton == "NPoints") {
                    inputNumber = 0;
                    updateGameState(REWRITING);
                } else if (pressedButton == "MinimumDistance") {

                } else if (pressedButton == "Hide/Show Paths") {
                    menu.updateBothButton("MinimumDistance", std::to_string(game.getAnswerValue()));
                    game.updateDrawingState();
                } else {

                }
            }
            break;
        }

        case SDL_MOUSEBUTTONUP: {
            if (gameState == TRACKING) {
                game.SolveTSP();
                updateGameState(STARTING_SCREEN);
            }
            break;
        }

        case SDL_KEYDOWN: {
            if (gameState == REWRITING) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    updateGameState(STARTING_SCREEN);
                } else if (e.key.keysym.sym >= SDLK_0 and e.key.keysym.sym <= SDLK_9) {
                    int tmp = inputNumber * 10 + e.key.keysym.sym - SDLK_0;
                    if (tmp > 20 or tmp < 0) {
                        break;
                    }
                    inputNumber = inputNumber * 10 + e.key.keysym.sym - SDLK_0;
                    editor.updateBothButton("Editing Points", "Enter a number between 1 and 20: " + std::to_string(inputNumber));
                } else if (e.key.keysym.sym == SDLK_BACKSPACE) {
                    inputNumber /= 10;
                    if (inputNumber == 0) {
                        editor.updateBothButton("Editing Points", "Enter a number between 1 and 20: ");
                    } else if (inputNumber > 0) {
                        editor.updateBothButton("Editing Points", "Enter a number between 1 and 20: " + std::to_string(inputNumber));
                    }
                } else if (e.key.keysym.sym == SDLK_RETURN) {
                    if (inputNumber > 0 and inputNumber <= 20) {
                        game.resize(inputNumber);
                        menu.updateBothButton("NPoints", "NPoints: " + std::to_string(game.getNumberOfVertexes()));
                        game.SolveTSP();
                    }
                    editor.updateBothButton("Editing Points", "Enter a number between 1 and 20: ");
                    updateGameState(STARTING_SCREEN);
                }
            }
            break;
        }

        default: {
            break;
        }
    }
}
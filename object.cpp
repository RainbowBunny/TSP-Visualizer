// Custom library
#include <object.h>

// SDL2 library

// Standard library
#include <cmath>

const int MAXN = 20;

int popcount[1 << MAXN], lastbit[1 << MAXN];
int parent[1 << MAXN][MAXN];
float dp[1 << MAXN][MAXN];
float d[MAXN][MAXN];

Game::Game(SDL_Rect gameScreen, int numberOfPoints) {
    screen = gameScreen;
    listOfPoints.resize(numberOfPoints);
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    randomGenerator = RandomGenerator(seed);
    for (int i = 0; i < numberOfPoints; i++) {
        listOfPoints[i].updateX(randomGenerator.randomInteger(gameScreen.x, gameScreen.x + gameScreen.w));
        listOfPoints[i].updateY(randomGenerator.randomInteger(gameScreen.y, gameScreen.y + gameScreen.h));
    }
}

void Game::resize(int numberOfPoints) {

}

void Point::renderPoint(SDL_Renderer* &renderer, Gallery &gallery) {
    SDL_Rect Potato = {x - sz / 2, y - sz / 2, sz, sz};
    SDL_RenderCopy(renderer, gallery.getFrame(id, frame), nullptr, &Potato);
}

bool Point::isBeingTracked(int mouseX, int mouseY) {
    return mouseX >= x - sz && mouseX <= x + sz && mouseY >= y - sz && mouseY <= y + sz;
}

void Game::renderGame(SDL_Renderer* &renderer, Gallery &gallery) {
    for (int i = 0; i < listOfPoints.size(); i++) {
        listOfPoints[i].renderPoint(renderer, gallery);
    }
}

bool Game::updateTarget(int mouseX, int mouseY) {
    for (int i = 0; i < listOfPoints.size(); i++) {
        if (listOfPoints[i].isBeingTracked(mouseX, mouseY)) {
            target = i;
            return true;
        }
    }
    return false;
}

void Game::TSPInit() {
    popcount[0] = -1;
    for (int i = 1; i < 1 << 20; i++) {
        popcount[i] = popcount[i >> 1] + (i & 1);
        for (int j = 0; j < 20; j++) {
            if (i & (1 << j)) {
                lastbit[i] = j;
                break;
            }
        }
    }
}

void Game::writeData() {
    std::ofstream file;
    file.open("data/input.txt", std::ios::out);
    if (file.is_open()) {
        file << listOfPoints.size() << std::endl;
        for (int i = 0; i < listOfPoints.size(); i++) {
            file << listOfPoints[i].getX() << " " << listOfPoints[i].getY() << std::endl;
        }
        file.close();
    }
}

void Game::tracking(int mouseX, int mouseY) {
    // Moving the target to mouse position
    listOfPoints[target].updateX(mouseX);
    listOfPoints[target].updateY(mouseY);
}

std::vector <int> Game::SolveTSP() {
    int n = listOfPoints.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            d[i][j] = sqrt(pow(listOfPoints[i].getX() - listOfPoints[j].getX(), 2) + pow(listOfPoints[i].getY() - listOfPoints[j].getY(), 2));
        }
    }

    float bestRoute = 1e9;
    std::vector <int> Answer(n);
    
    for (int k = 0; k < 1; k++) {
        for (int i = 0; i < 1 << n; i++) {
            for (int j = 0; j < n; j++) {
                // k is the starting vertex, i is the mask of visited vertex and j is the last vertex
                // if last vertex is not visited then this state is invalid
                if (!(i & (1 << j))) {
                    continue;
                }
                // We do not want to include k twice
                if (i & (1 << k)) {
                    continue;
                }
                dp[i][j] = 1e9;
                int tmp = i ^ (1 << j);
                if (tmp == 0) {
                    dp[i][j] = d[k][j];
                } else {
                    while (tmp > 0) {
                        int lastvertex = lastbit[tmp];
                        tmp ^= (1 << lastvertex);
                        if (dp[i][j] > dp[i ^ (1 << lastvertex)][lastvertex] + d[j][lastvertex]) {
                            dp[i][j] = dp[i ^ (1 << lastvertex)][lastvertex] + d[j][lastvertex];
                            // Saving the best previous vertex for the state
                            parent[i][j] = lastbit[tmp];
                        }
                    }
                    
                    // the following session is for updating the result
                    if (popcount[i] != n - 1) {
                        continue;
                    }
                    
                    // open if so that we do not have to use one more nested condition
                    if (bestRoute > dp[i][j] + d[j][k]) {
                        
                    } else {
                        continue;
                    }
                    
                    bestRoute = dp[i][j] + d[j][k];
                    int currentMask = i, currentVertex = j;
                    while (currentMask > 0) {
                        Answer[popcount[currentMask]] = currentVertex;
                        currentVertex = parent[currentMask][currentVertex];
                        currentMask ^= (1 << currentVertex);
                    }

                    Answer[0] = k;

                }
                
            }
        }
    }
    return Answer;
} 
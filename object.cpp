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
        listOfPoints[i].updateIndex(i + 1);
    }
    Answer.resize(numberOfPoints);
}

void Game::resize(int numberOfPoints) {
    std::cout << numberOfPoints << std::endl;
    if (listOfPoints.size() < numberOfPoints) {
        while ((int)listOfPoints.size() < numberOfPoints) {
            listOfPoints.push_back(Point());
            listOfPoints.back().updateX(randomGenerator.randomInteger(screen.x, screen.x + screen.w));
            listOfPoints.back().updateY(randomGenerator.randomInteger(screen.y, screen.y + screen.h));
            listOfPoints.back().updateIndex((int)listOfPoints.size());
        }
    } else {
        while ((int)listOfPoints.size() > numberOfPoints) {
            listOfPoints.pop_back();
        }
    }

    Answer.resize(listOfPoints.size());
    SolveTSP();
}

void Point::renderPoint(SDL_Renderer* &renderer, Gallery &gallery) {
    SDL_Rect Potato = {x - sz / 2, y - sz / 2, sz, sz};
    SDL_RenderCopy(renderer, gallery.getFrame(id, frame), nullptr, &Potato);
    SDL_RenderCopy(renderer, gallery.loadTextureFromText(std::to_string(index), BLACK_COLOR), nullptr, &Potato);
}

bool Point::isBeingTracked(int mouseX, int mouseY) {
    return mouseX >= x - sz && mouseX <= x + sz && mouseY >= y - sz && mouseY <= y + sz;
}

float Point::distance(Point B) {
    return sqrt(pow(x - B.getX(), 2) + pow(y - B.getY(), 2));
}

void Game::renderGame(SDL_Renderer* &renderer, Gallery &gallery) {
    for (int i = 0; i < listOfPoints.size(); i++) {
        listOfPoints[i].renderPoint(renderer, gallery);
    }

    if (drawingPath) {
        for (int i = 0; i < listOfPoints.size(); i++) {
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderDrawLine(renderer, listOfPoints[Answer[i]].getX(), listOfPoints[Answer[i]].getY(), 
                listOfPoints[Answer[(i + 1) % listOfPoints.size()]].getX(), listOfPoints[Answer[(i + 1) % listOfPoints.size()]].getY());
        }
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
    mouseX = std::max(mouseX, screen.x); mouseX = std::min(mouseX, screen.x + screen.w);
    mouseY = std::max(mouseY, screen.y); mouseY = std::min(mouseY, screen.y + screen.h);
    listOfPoints[target].updateX(mouseX);
    listOfPoints[target].updateY(mouseY);
}

void Game::SolveTSP() {
    int n = listOfPoints.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            d[i][j] = listOfPoints[i].distance(listOfPoints[j]);
        }
    }

    float bestRoute = 1e9;
    
    for (int startingVertex = 0; startingVertex < 1; startingVertex++) {
        for (int maskOfVisited = 1; maskOfVisited < 1 << n; maskOfVisited++) {
            for (int lastCity = 0; lastCity < n; lastCity++) {
                // k is the starting vertex, i is the mask of visited vertex and j is the last vertex
                // if last vertex is not visited then this state is invalid
                if (!(maskOfVisited & (1 << lastCity))) {
                    continue;
                }
                // We do not want to include k twice
                if (maskOfVisited & (1 << startingVertex)) {
                    continue;
                }
                
                dp[maskOfVisited][lastCity] = 1e9;
                int tmp = maskOfVisited ^ (1 << lastCity);
                if (tmp == 0) {
                    // Only one bit
                    dp[maskOfVisited][lastCity] = d[startingVertex][lastCity];
                } else {
                    while (tmp > 0) {
                        int lastvertex = lastbit[tmp];
                        tmp ^= (1 << lastvertex);
                        if (dp[maskOfVisited][lastCity] > dp[maskOfVisited ^ (1 << lastCity)][lastvertex] + d[lastvertex][lastCity]) {
                            dp[maskOfVisited][lastCity] = dp[maskOfVisited ^ (1 << lastCity)][lastvertex] + d[lastvertex][lastCity];
                            // Saving the best previous vertex for the state
                            parent[maskOfVisited][lastCity] = lastvertex;
                        }
                    }
                    
                    // std::cout << i << " " << j << " " << k << std::endl;
                    
                    // the following session is for updating the result
                    if (popcount[maskOfVisited] != n - 1) {
                        continue;
                    }
                    
                    // std::cout << i << " " << j << " " << k << std::endl;

                    // open if so that we do not have to use one more nested condition
                    if (bestRoute > dp[maskOfVisited][lastCity] + d[lastCity][startingVertex]) {
                        
                    } else {
                        continue;
                    }
                    
                    
                    bestRoute = dp[maskOfVisited][lastCity] + d[lastCity][startingVertex];
                    int currentMask = maskOfVisited, currentVertex = lastCity;
                    while (currentMask > 0) {
                        Answer[popcount[currentMask]] = currentVertex;
                        int tmp = currentVertex;
                        currentVertex = parent[currentMask][currentVertex];
                        currentMask ^= (1 << tmp);
                        // std::cout << 1 << " " << currentMask << " " << currentVertex << std::endl;
                    }

                    Answer[0] = startingVertex;

                }
                
            }
        }
    }

    for (int i = 0; i < n; i++) {
        std::cout << Answer[i] << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << listOfPoints[i].getX() << " " << listOfPoints[i].getY() << std::endl;
    }
} 

float Game::getAnswerValue() {
    float ans = 0;
    for (int i = 0; i < (int)listOfPoints.size(); i++) {
        ans += listOfPoints[i].distance(listOfPoints[(i + 1) % (int)listOfPoints.size()]);
    }
    return ans;
}
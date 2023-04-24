#pragma once
#ifndef OBJECT_H
#define OBJECT_H
// Custom library
#include <utils.h>
#include <gallery.h>
#include <constants.h>
#include <menu.h>

// SDL2 library
#include <SDL2/SDL.h>

// Standard library
#include <vector>
#include <chrono>
#include <random>

// Random device
class RandomGenerator {
private:
    std::mt19937_64 rng;
public:
    RandomGenerator() {}
    RandomGenerator(unsigned seed) { rng.seed(seed); }
    int randomInteger(int l, int r) { return rng() % (r - l + 1) + l; }
};

class Point {
private:
    int frame = 0;
    PictureID id = POINT;
    int x, y, index, sz = 30;
public:
    Point() {};
    Point(int _x, int _y) { x = _x; y = _y; }
    void updateX(int _x) { x = _x; }
    void updateY(int _y) { y = _y; }
    void updateIndex(int _index) { index = _index; }
    int getX() { return x; }
    int getY() { return y; }
    int getIndex() { return index; }
    void renderPoint(SDL_Renderer* &renderer, Gallery &gallery);
    bool isBeingTracked(int mouseX, int mouseY);
    float distance(Point B);
};

class Game {
private:
    int target;
    SDL_Rect screen;
    std::vector <Point> listOfPoints;
    RandomGenerator randomGenerator;
    std::vector <int> Answer;
    bool drawingPath;
public:
    Game() {}
    Game(SDL_Rect screenBoard, int numberOfPoints);
    int getNumberOfVertexes() { return listOfPoints.size(); }
    void resize(int numberOfPoints);
    void renderGame(SDL_Renderer* &renderer, Gallery &gallery);
    bool updateTarget(int mouseX, int mouseY);
    void tracking(int mouseX, int mouseY);
    void writeData();
    void TSPInit();
    void SolveTSP();
    bool getDrawState() { return drawingPath; }
    void updateDrawingState() { drawingPath ^= 1; }
    float getAnswerValue();
};

#endif
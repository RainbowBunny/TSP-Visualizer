#pragma once
#ifndef GALLERY_H
#define GALLERY_H
// Custom library

// SDL2 library
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// Standard library
#include <string>
#include <vector>

// enumeration for different types of image.
enum PictureID {
    BUTTON = 0,
    POINT,
    NONE,
    PIC_COUNT
};

class Gallery {
/*
    This is the class to load all the image in the game
*/
private:
    std::vector <std::vector <SDL_Texture*> > pictures;
    SDL_Renderer* renderer;
    TTF_Font* font;
    void loadFont(std::string path);
    std::vector <SDL_Texture*> loadTextureFromImage(std::string path, 
        int numberOfFrame, std::string extension, SDL_BlendMode blendMode);
public:
    Gallery(SDL_Renderer* _renderer);
    ~Gallery();

    void loadGamePictures();
    std::vector <SDL_Texture*> getImage(PictureID id) const { return pictures[id]; }
    SDL_Texture* getFrame(PictureID obj, int &currentFrame);
    SDL_Texture* loadTextureFromText(std::string textString, SDL_Color textColor);
    SDL_Texture* mergingTexture(SDL_Texture* texture1, SDL_Texture* texture2);
};

#endif
// Custom library
#include <menu.h>

// SDL2 library

// Standard library
#include <unistd.h>

Button::Button(std::string _buttonName, SDL_Rect _buttonRect, Textbox _normal, Textbox _special) {
    buttonName = _buttonName;
    buttonRect = _buttonRect;
    normal = _normal;
    special = _special;
}

bool Button::isChoosing(int mouseX, int mouseY) {
    return mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
        mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h;
}

void Button::renderButton(SDL_Renderer* &renderer, Gallery& gallery, int mouseX, int mouseY) {
    if (isChoosing(mouseX, mouseY)) {
        special.renderTextBox(renderer, gallery);
    } else {
        normal.renderTextBox(renderer, gallery);
    }
}

Menu::Menu(std::vector <std::string> _buttonName, std::vector <SDL_Rect> _buttonPosition, 
    std::vector <Textbox> normal, std::vector <Textbox> special) {
    buttonList.resize((int)_buttonName.size());
    for (int i = 0; i < (int)_buttonName.size(); i++) {
        buttonList[i] = Button(_buttonName[i], _buttonPosition[i], normal[i], special[i]);
    }
}

void Menu::renderMenu(SDL_Renderer* &renderer, Gallery& gallery, int mouseX, int mouseY) {
    for (int i = 0; i < (int)buttonList.size(); i++) {
        buttonList[i].renderButton(renderer, gallery, mouseX, mouseY);
    }
}

Uint8 castingIntToUint8(int target) { return target % (1 << 8); }

Menu loadMenuFromFile(std::string file, SDL_Renderer* &renderer, Gallery &gallery) {
    /*
        Data format:
        name x y w h content
        string name: name of the button
        x y w h - SDL_Rect, position of the menu button
        string content: the text string on the menu
    */
    std::ifstream fin(file);
    if (!fin.is_open()) {
        logError(std::cout, "Can not open file: " + file, true);
    }
    int n;
    fin >> n;

    std::vector <std::string> buttonName(n);
    std::vector <SDL_Rect> buttonPosition(n);
    std::vector <Textbox> normal(n), special(n);

    for (int i = 0; i < n; i++) {
        getline(fin, buttonName[i]);
        getline(fin, buttonName[i]);
        std::cout << buttonName[i] << std::endl;
        fin >> buttonPosition[i].x >> buttonPosition[i].y >> buttonPosition[i].w >> buttonPosition[i].h;
        std::cout << buttonPosition[i].x << " " << buttonPosition[i].y << " " << buttonPosition[i].w << " " << buttonPosition[i].h << std::endl;
        normal[i] = createTextboxFromFile(fin), special[i] = createTextboxFromFile(fin);
    }
    return Menu(buttonName, buttonPosition, normal, special);
}

std::string Menu::getPressedButton(int mouseX, int mouseY) {
    for (int i = 0; i < (int)buttonList.size(); i++) {
        if (buttonList[i].isChoosing(mouseX, mouseY)) {
            return buttonList[i].getButtonName();
        }
    }
    return "none";
}

Background::Background() {
    /*
    enum State {
        MAIN_MENU = 0,
        GAME_BACKGROUND,
        PAUSE_MENU,
        GAME_WINNING,
        GAME_LOSING,
        TOTALSTATE
    };
    */

    currentState = NO_BACKGROUND;

    backgroundID.push_back(NONE);
}

void Background::renderBackground(SDL_Renderer* &renderer, Gallery &gallery) {
    if (currentState == NO_BACKGROUND) {
        return;
    }
    SDL_RenderCopy(renderer, gallery.getFrame(backgroundID[currentState], frame), nullptr, nullptr);
    frame++;
}

void Textbox::renderTextBox(SDL_Renderer* &renderer, Gallery &gallery) {
    if (background != NONE) {
        SDL_RenderCopy(renderer, gallery.getFrame(background, frame), nullptr, &backgroundRect);
        frame++;
    }
    
    if ((int)textString.size() > 0) {
        SDL_RenderCopy(renderer, gallery.loadTextureFromText(textString, textColor), nullptr, &textRect);
    }
}

Textbox createTextboxFromFile(std::ifstream &fin) {
    std::string backgroundID;
    fin >> backgroundID;
    std::cout << backgroundID << std::endl;
    PictureID id;
    if (backgroundID == "BUTTON") { 
        id = BUTTON;
    }
    
    SDL_Rect _backgroundRect, _textRect;
    SDL_Color _color;
    
    fin >> _backgroundRect.x >> _backgroundRect.y >> _backgroundRect.w >> _backgroundRect.h;
    std::cout << _backgroundRect.x << " " << _backgroundRect.y << " " << _backgroundRect.w << " " << _backgroundRect.h << std::endl;
    fin >> _textRect.x >> _textRect.y >> _textRect.w >> _textRect.h;
    std::cout << _textRect.x << " " << _backgroundRect.y << " " << _backgroundRect.w << " " << _backgroundRect.h << std::endl;

    int r, g, b;
    fin >> r >> g >> b;
    std::cout << r << " " << g << " " << b << std::endl;
    _color.r = castingIntToUint8(r);
    _color.g = castingIntToUint8(g);
    _color.b = castingIntToUint8(b);
    
    return Textbox(id, _backgroundRect, _textRect, _color);
}
#include "view/hud.hpp"

Menu::Menu(SDL_Renderer *rend) : render(rend), hidden(false) {}

void Menu::add_rect_draw(std::string text, SDL_Rect rect, rgb color, std::string font_route, int font_size) {

    RectDraw *new_ui = new RectDraw(render, text, rect, color, font_route, font_size);

    UI.push_back(new_ui);

    //UI.emplace_back(render, text, rect, color, font_route, font_size);
}

void Menu::hide() {
    hidden = true;
}

void Menu::show() {
    hidden = false;
}

void Menu::render_menu() const {
    if (!hidden) {
        for (const RectDraw* draw : UI) {
            draw->render();
        }
    }
}

void Menu::use() {
    if (!hidden) {
        for (RectDraw* draw : UI) {
            draw->use();
        }
    }
}

void Menu::update() {
    if (!hidden) {
        for (RectDraw* draw : UI) {
            draw->update();
        }
    }
}

bool Menu::is_hidden() {
    return hidden;
}

HUD::HUD() {
    TTF_Init();
}

HUD::~HUD() {
    TTF_Quit();
}

void    HUD::init_hud(SDL_Window *win, SDL_Renderer *rend) {
    window = win;
    render = rend;
}

bool    HUD::exist(std::string name) const {
    if (menus.find(name) == menus.end()) {
        return false;
    }
    return true;
}

void    HUD::create_menu(std::string name) {
    if (exist(name)) {
        throw std::invalid_argument("The menu " + name + " already exist");
    }

    Menu menu(render);

    menus.emplace(name, menu);
    //menus.insert({name, menu});
    //menus[name] = menu;
}

void    HUD::add_rect_draw(std::string name, std::string text, Double_rect rect, rgb color, std::string font_route, int font_size) {
    int w, h;

    SDL_GL_GetDrawableSize(window, &w, &h);

    SDL_Rect sdl_rect = {static_cast<int>(rect.x * w), static_cast<int>(rect.y * h), static_cast<int>(rect.w * w), static_cast<int>(rect.h * h)};

    std::cout << sdl_rect.w << ", " << sdl_rect.h << std::endl;

    if (!exist(name))
        throw std::invalid_argument("The menu " + name + " doesn't exist");
    menus.find(name)->second.add_rect_draw(text, sdl_rect, color, font_route, font_size);
}

void    HUD::hide_menu(std::string name) {
    if (!exist(name))
        throw std::invalid_argument("The menu " + name + " doesn't exist");
    menus.find(name)->second.hide();
}
void    HUD::show_menu(std::string name) {
    if (!exist(name))
        throw std::invalid_argument("The menu " + name + " doesn't exist");
    menus.find(name)->second.show();
}

void    HUD::render_menus() const {
    for (auto const& x : menus) {
        x.second.render_menu();
    }
}

void    HUD::use() {
    for (auto& x : menus) {
        x.second.use();
    }
}

void    HUD::update() {
    for (auto& x : menus) {
        x.second.update();
    }
}

bool    HUD::menu_is_hidden(std::string name) {
    if (!exist(name))
        throw std::invalid_argument("The menu " + name + " doesn't exist");
    return menus.find(name)->second.is_hidden();
}
#include "view/hud.hpp"

Menu::Menu(SDL_Renderer *rend) : render(rend), hidden(false) {}

void Menu::add_rect_draw(std::string text, SDL_Rect rect, rgb color, std::string font_route, int font_size) {
    UI.emplace_back(render, text, rect, color, font_route, font_size);
}

void Menu::hide() {
    hidden = true;
}

void Menu::show() {
    hidden = false;
}

void Menu::render_menu() const {
    //TODO
}


HUD::HUD() {
    TTF_Init();
}

HUD::~HUD() {
    TTF_Quit();
}

void    HUD::init_hud(SDL_Renderer *rend) {
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

void    HUD::add_rect_draw(std::string name, std::string text, SDL_Rect rect, rgb color, std::string font_route, int font_size) {
    if (!exist(name))
        throw std::invalid_argument("The menu " + name + " doesn't exist");
    menus.find(name)->second.add_rect_draw(text, rect, color, font_route, font_size);
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
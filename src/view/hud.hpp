#pragma once

#include <SDL2/SDL_timer.h>
#include <SDL2/SDL.h>

#include "view/color.hpp"
#include <iostream>
#include <map>

#include <SDL2/SDL_ttf.h>

class RectDraw {
    private:
        SDL_Rect    rect;
        SDL_Texture *texture;
        SDL_Texture *highlight;
        std::string text;
        SDL_Renderer *render;

    public:
        RectDraw(SDL_Renderer *rend, std::string t, SDL_Rect r, rgb color) : render(rend), text(t), rect(r) {
            texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, r.w, r.h);
            
        }
        virtual void use() {};

};

template <class Function>
class Button : public RectDraw {
    private:
        Function function;

    public:
        Button(SDL_Renderer *render, std::string text, SDL_Rect rect, rgb color, Function f) : RectDraw(render, text, rect, color), function(f) {
        }
        void use() override {
            function();
        };
};

class Menu {
    private:
        SDL_Renderer            *render;
        std::vector<RectDraw>   buttons;
        bool                    hidden;

    public:
        //Menu() {}
        Menu(SDL_Renderer *rend);

        template <class Function>
        void    add_button(std::string text, SDL_Rect rect, rgb color, Function f) {
            buttons.emplace_back(render, text, rect, color, f);
        }
        void add_rect_draw(std::string text, SDL_Rect rect, rgb color);
        void hide();
        void show();

        void render_menu() const;
};

class HUD {
    private:
        SDL_Renderer                    *render;
        std::map<std::string, Menu>     menus;

    public:
        HUD(SDL_Renderer *rend);

        bool    exist(std::string name) const;
        void    create_menu(std::string name);
        template <class Function>
        void    add_button(std::string name, std::string text, SDL_Rect rect, rgb color, Function f) {
            if (!exist(name))
                throw std::invalid_argument("The menu " + name + " doesn't exist");
            menus.find(name)->second.add_button(text, rect, color, f);
        }
        void    add_rect_draw(std::string name, std::string text, SDL_Rect rect, rgb color);
        void    hide_menu(std::string name);
        void    show_menu(std::string name);

        void    render_menus() const;
};

//TTF_Init();

//TTF_Font *police = NULL;

//TTF_OpenFont

//ex: police = TTF_OpenFont("angelina.ttf", 65);

//TTF_CloseFont(police); /* Doit être avant TTF_Quit() */
//TTF_Quit();
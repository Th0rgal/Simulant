#pragma once

#include <SDL2/SDL_timer.h>
#include <SDL2/SDL.h>

#include "view/color.hpp"
#include <iostream>
#include <map>

#include <SDL2/SDL_ttf.h>

class RectDraw {
    private:
        std::string text;
        SDL_Texture *texture;
        SDL_Rect    rect;
        SDL_Texture *text_texture;
        SDL_Rect    text_rect;
        SDL_Renderer *renderer;

    public:
        RectDraw(SDL_Renderer *rend, std::string t, SDL_Rect r, rgb color, std::string font_route, int font_size) : renderer(rend), text(t), rect(r) {
            texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, r.w, r.h);
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

            SDL_SetRenderTarget(renderer, texture);
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

            SDL_RenderFillRect(renderer, NULL);

            SDL_SetRenderTarget(renderer, NULL);

            TTF_Font *font = TTF_OpenFont(font_route.c_str(), font_size);

            SDL_Color color_sdl = {0, 0, 0, 0xFF};

            SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), color_sdl);

            text_texture = SDL_CreateTextureFromSurface(renderer, surface);

            int w, h;

            SDL_QueryTexture(text_texture, NULL, NULL, &w, &h);
            text_rect = {(r.w - w) / 2, (r.h - h) / 2, w, h};

            TTF_CloseFont(font);
        }
        ~RectDraw() {
            SDL_DestroyTexture(texture);
            SDL_DestroyTexture(text_texture);
        }
        virtual void use() {};
        virtual void render() {
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
        }
};

//TTF_Init();

//TTF_Font *police = NULL;

//TTF_OpenFont

//ex: police = TTF_OpenFont("angelina.ttf", 65);

//TTF_CloseFont(police); /* Doit être avant TTF_Quit() */
//TTF_Quit();

template <class Function>
class Button : public RectDraw {
    private:
        Function function;
        SDL_Texture *highlight;

    public:
        Button(SDL_Renderer *render, std::string text, SDL_Rect rect, rgb color,  std::string font_route, int font_size, Function f) : RectDraw(render, text, rect, color, font_route, font_size), function(f) {}
        ~Button() {
            //SDL_DestroyTexture(highlight);
        }
        void use() override {
            function();
        }
        void render() override {
            RectDraw::render();
            //SDL_RenderCopy(renderer, highlight, NULL, &rect);
            //TODO
        }
};

class Menu {
    private:
        SDL_Renderer            *render;
        std::vector<RectDraw>   UI;
        bool                    hidden;

    public:
        //Menu() {}
        Menu(SDL_Renderer *rend);

        template <class Function>
        void    add_button(std::string text, SDL_Rect rect, rgb color, std::string font_route, int font_size, Function f) {
            UI.emplace_back(render, text, rect, color, font_route, font_size, f);
        }
        void add_rect_draw(std::string text, SDL_Rect rect, rgb color, std::string font_route, int font_size);
        void hide();
        void show();

        void render_menu() const;
};

class HUD {
    private:
        SDL_Renderer                    *render;
        std::map<std::string, Menu>     menus;

    public:
        HUD();
        ~HUD();

        void    init_hud(SDL_Renderer *rend);

        bool    exist(std::string name) const;
        void    create_menu(std::string name);
        template <class Function>
        void    add_button(std::string name, std::string text, SDL_Rect rect, rgb color, Function f, std::string font_route, int font_size) {
            if (!exist(name))
                throw std::invalid_argument("The menu " + name + " doesn't exist");
            menus.find(name)->second.add_button(text, rect, color, font_route, font_size, f);
        }
        void    add_rect_draw(std::string name, std::string text, SDL_Rect rect, rgb color, std::string font_route, int font_size);
        void    hide_menu(std::string name);
        void    show_menu(std::string name);

        void    render_menus() const;
};
#pragma once

#include <SDL2/SDL_timer.h>
#include <SDL2/SDL.h>

#include "view/color.hpp"
#include <iostream>
#include <map>

#include <SDL2/SDL_ttf.h>

extern double scale_high_dpi;

class RectDraw {
    protected:
        std::string text;
        SDL_Texture *texture;
        SDL_Rect    rect;
        SDL_Texture *text_texture;
        SDL_Rect    text_rect;
        SDL_Renderer *renderer;

    public:
        RectDraw(SDL_Renderer *rend, std::string t, SDL_Rect r, rgb color, std::string font_route, int font_size) : renderer(rend), text(t), rect(r) {
            
            std::cout << color.r << ", " << color.g << ", " << color.b << ", " << color.a << std::endl;

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
            text_rect = {r.x + (r.w - w) / 2, r.y + (r.h - h) / 2, w, h};

            TTF_CloseFont(font);
        }
        ~RectDraw() {
                std::cout << "JE ME DETRUIT" << std::endl;
            SDL_DestroyTexture(texture);
            SDL_DestroyTexture(text_texture);
        }
        virtual void use() {};
        virtual void render() const {
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
        }
        virtual void update() {};
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
        SDL_Texture *overlay;
        bool        show_overlay;

    public:
        Button(SDL_Renderer *render, std::string text, SDL_Rect rect, rgb color,  std::string font_route, int font_size, Function f) : RectDraw(render, text, rect, color, font_route, font_size), function(f), show_overlay(false) {
            overlay = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);
            SDL_SetTextureBlendMode(overlay, SDL_BLENDMODE_BLEND);

            SDL_SetRenderTarget(renderer, overlay);
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x80);

            SDL_RenderFillRect(renderer, NULL);

            SDL_SetRenderTarget(renderer, NULL);

        }
        ~Button() {
            SDL_DestroyTexture(overlay);
        }
        void use() override {
            SDL_Point p;

            SDL_GetMouseState(&p.x, &p.y);
            p.x *= scale_high_dpi;
            p.y *= scale_high_dpi;

            if (SDL_PointInRect(&p, &rect)) {
                function();
            }
        }
        void render() const override {
            RectDraw::render();
            if (show_overlay)
                SDL_RenderCopy(renderer, overlay, NULL, &rect);
        }
        void update() override {
            SDL_Point p;
            SDL_GetMouseState(&p.x, &p.y);

            p.x *= scale_high_dpi;
            p.y *= scale_high_dpi;

            if (SDL_PointInRect(&p, &rect)) {
                show_overlay = true;
            } else {
                show_overlay = false;
            }
        }
};

class Menu {
    private:
        SDL_Renderer            *render;
        std::vector<RectDraw*>  UI;
        bool                    hidden;

    public:
        //Menu() {}
        Menu(SDL_Renderer *rend);

        template <class Function>
        void    add_button(std::string text, SDL_Rect rect, rgb color, std::string font_route, int font_size, Function f) {
            RectDraw *new_ui = new Button(render, text, rect, color, font_route, font_size, f);

            UI.push_back(new_ui);

            //UI.emplace_back(render, text, rect, color, font_route, font_size, f);
        }
        void add_rect_draw(std::string text, SDL_Rect rect, rgb color, std::string font_route, int font_size);
        void hide();
        void show();

        void render_menu() const;
        void use();
        void update();

        bool is_hidden();
};

struct Double_rect {
    double x;
    double y;
    double w;
    double h;
};


class HUD {
    private:
        SDL_Renderer                    *render;
        SDL_Window                      *window;
        std::map<std::string, Menu>     menus;

    public:
        HUD();
        ~HUD();

        void    init_hud(SDL_Window *window, SDL_Renderer *rend);

        bool    exist(std::string name) const;
        void    create_menu(std::string name);
        template <class Function>
        void    add_button(std::string name, std::string text, Double_rect rect, rgb color, std::string font_route, int font_size, Function f) {
            int w, h;
            SDL_GL_GetDrawableSize(window, &w, &h);
            SDL_Rect sdl_rect = {static_cast<int>(rect.x * w), static_cast<int>(rect.y * h), static_cast<int>(rect.w * w), static_cast<int>(rect.h * h)};
            if (!exist(name))
                throw std::invalid_argument("The menu " + name + " doesn't exist");
            menus.find(name)->second.add_button(text, sdl_rect, color, font_route, font_size, f);
        }
        void    add_rect_draw(std::string name, std::string text, Double_rect rect, rgb color, std::string font_route, int font_size);
        void    hide_menu(std::string name);
        void    show_menu(std::string name);

        void    render_menus() const;
        void    use();
        void    update();

        bool    menu_is_hidden(std::string name);
};
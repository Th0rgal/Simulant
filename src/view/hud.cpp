#include "view/hud.hpp"

HUD::HUD(SDL_Window *w) : window(w) {
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
}

HUD::~HUD() {
    SDL_DestroyRenderer(render);
}
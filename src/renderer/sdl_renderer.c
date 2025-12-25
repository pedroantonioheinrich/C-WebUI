#include "cwebui/cwebui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static TTF_Font* global_font = NULL;

static void draw_filled_circle(SDL_Renderer* r_ptr, int x, int y, int radius) {
    if (radius <= 0) return;
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) 
                SDL_RenderDrawPoint(r_ptr, x + dx, y + dy);
        }
    }
}

static void render_text(SDL_Renderer* r_ptr, const char* text, int x, int y, int w, int h) {
    if (!global_font || !text || strlen(text) == 0) return;

    // Usamos Blended para a melhor qualidade possível
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderUTF8_Blended(global_font, text, white);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(r_ptr, surface);
    
    int text_w, text_h;
    TTF_SizeText(global_font, text, &text_w, &text_h);

    // Centralização matemática rigorosa
    SDL_Rect dest = { 
        x + (w - text_w) / 2, 
        y + (h - text_h) / 2, 
        text_w, 
        text_h 
    };

    SDL_RenderCopy(r_ptr, texture, NULL, &dest);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

static void draw_rounded_rect(SDL_Renderer* r_ptr, CWidget* w) {
    int r = w->style.border_radius;
    uint32_t c = (w->style.bg_color == 0) ? 0x1F6AA5 : w->style.bg_color;

    uint8_t red = (uint8_t)((c >> 16) & 0xFF);
    uint8_t green = (uint8_t)((c >> 8) & 0xFF);
    uint8_t blue = (uint8_t)(c & 0xFF);

    if (w->is_hovered) {
        red = (red > 225) ? 255 : red + 30;
        green = (green > 225) ? 255 : green + 30;
        blue = (blue > 225) ? 255 : blue + 30;
    }

    SDL_SetRenderDrawColor(r_ptr, red, green, blue, 255);

    draw_filled_circle(r_ptr, w->x + r, w->y + r, r);
    draw_filled_circle(r_ptr, w->x + w->width - r - 1, w->y + r, r);
    draw_filled_circle(r_ptr, w->x + r, w->y + w->height - r - 1, r);
    draw_filled_circle(r_ptr, w->x + w->width - r - 1, w->y + w->height - r - 1, r);

    SDL_Rect h_rect = {w->x + r, w->y, w->width - 2 * r, w->height};
    SDL_Rect v_rect = {w->x, w->y + r, w->width, w->height - 2 * r};
    SDL_RenderFillRect(r_ptr, &h_rect);
    SDL_RenderFillRect(r_ptr, &v_rect);

    // Renderiza o texto se houver uma label
    if (strlen(w->label) > 0) {
        render_text(r_ptr, w->label, w->x, w->y, w->width, w->height);
    }
}

void cwebui_init_graphics(int width, int height, const char* title) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Carrega uma fonte padrão do Ubuntu
    global_font = TTF_OpenFont("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf", 16);
    if (!global_font) printf("[C-WebUI] Aviso: Não foi possível carregar a fonte TTF.\n");
}

void cwebui_render_all(CWidget* widgets, int count) {
    SDL_SetRenderDrawColor(renderer, 26, 26, 26, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < count; i++) {
        if (widgets[i].visible) {
            draw_rounded_rect(renderer, &widgets[i]);
        }
    }

    SDL_RenderPresent(renderer);
}

void cwebui_cleanup_graphics() {
    if (global_font) TTF_CloseFont(global_font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
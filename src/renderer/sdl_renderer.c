#include "cwebui/cwebui.h"
#include <SDL2/SDL.h>
#include <stdio.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

/* Função auxiliar para desenhar um círculo preenchido (usada para os cantos arredondados) */
void draw_filled_circle(SDL_Renderer* renderer, int x, int y, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // distância horizontal do centro
            int dy = radius - h; // distância vertical do centro
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

/* Desenha um retângulo com cantos arredondados (Estilo CustomTkinter) */
void draw_rounded_rect(SDL_Renderer* renderer, CWidget* w) {
    int r = w->style.border_radius;
    SDL_Rect rect = {w->x, w->y, w->width, w->height};

    // Define a cor de fundo (extraída do CSS)
    uint32_t c = w->style.bg_color;
    SDL_SetRenderDrawColor(renderer, (c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF, 255);

    if (r <= 0) {
        SDL_RenderFillRect(renderer, &rect);
        return;
    }

    // Desenha os 4 círculos nos cantos
    draw_filled_circle(renderer, w->x + r, w->y + r, r);
    draw_filled_circle(renderer, w->x + w->width - r, w->y + r, r);
    draw_filled_circle(renderer, w->x + r, w->y + w->height - r, r);
    draw_filled_circle(renderer, w->x + w->width - r, w->y + w->height - r, r);

    // Desenha os retângulos internos (forma de cruz) para preencher o centro
    SDL_Rect mid_h = {w->x + r, w->y, w->width - 2 * r, w->height};
    SDL_Rect mid_v = {w->x, w->y + r, w->width, w->height - 2 * r};
    SDL_RenderFillRect(renderer, &mid_h);
    SDL_RenderFillRect(renderer, &mid_v);
}

void cwebui_init_graphics(int width, int height, const char* title) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("[C-WebUI] Erro SDL_Init: %s\n", SDL_GetError());
        return;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                              width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    printf("[C-WebUI] Renderer SDL2 iniciado (%dx%d).\n", width, height);
}

/* Renderiza todos os widgets que foram processados pelo Core */
void cwebui_render_all(CWidget* widgets, int count) {
    // Fundo padrão Dark (CustomTkinter)
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
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
#include "cwebui/cwebui.h"
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

typedef struct {
    CWidget widgets[256];
    int widget_count;
    bool is_running;
} CWebUI_Context;

static CWebUI_Context ctx;

void cwebui_init() {
    ctx.widget_count = 0;
    ctx.is_running = false;
    memset(ctx.widgets, 0, sizeof(ctx.widgets));
    cwebui_init_graphics(800, 600, "C-WebUI Engine v0.1");
}

void cwebui_register_widget(CWidget w) {
    if (ctx.widget_count < 256) {
        ctx.widgets[ctx.widget_count++] = w;
    }
}

CWidget* cwebui_get_widget_by_id(const char* id) {
    for (int i = 0; i < ctx.widget_count; i++) {
        if (strcmp(ctx.widgets[i].id, id) == 0) {
            return &ctx.widgets[i];
        }
    }
    return NULL;
}

void cwebui_sync_styles() {
    CStyleMap* style_sheet;
    int style_count = cwebui_get_style_sheet(&style_sheet);

    for (int i = 0; i < ctx.widget_count; i++) {
        CWidget* w = &ctx.widgets[i];
        for (int j = 0; j < style_count; j++) {
            CStyleMap* s = &style_sheet[j];
            if (s->selector[0] == '#' && strcmp(&s->selector[1], w->id) == 0) {
                w->style = s->style;
            } else if (strcmp(s->selector, w->tag) == 0) {
                w->style = s->style;
            }
        }
    }
}

void cwebui_run() {
    ctx.is_running = true;
    SDL_Event event;

    while (ctx.is_running) {
        int mx, my;
        // Removido o uint32_t mouse_state para evitar o warning
        SDL_GetMouseState(&mx, &my);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) ctx.is_running = false;
            
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                for (int i = 0; i < ctx.widget_count; i++) {
                    CWidget* w = &ctx.widgets[i];
                    if (mx >= w->x && mx <= w->x + w->width && my >= w->y && my <= w->y + w->height) {
                        if (w->onclick) w->onclick();
                    }
                }
            }
        }

        for (int i = 0; i < ctx.widget_count; i++) {
            CWidget* w = &ctx.widgets[i];
            w->is_hovered = (mx >= w->x && mx <= w->x + w->width && my >= w->y && my <= w->y + w->height);
        }

        cwebui_render_all(ctx.widgets, ctx.widget_count);
        SDL_Delay(16);
    }
    cwebui_cleanup_graphics();
}
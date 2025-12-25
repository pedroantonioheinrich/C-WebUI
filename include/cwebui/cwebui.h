#ifndef CWEBUI_H
#define CWEBUI_H

#include <stdint.h>
#include <stdbool.h>

/* Definições de Estilo Estilo CustomTkinter */
typedef struct {
    uint32_t bg_color;
    uint32_t text_color;
    int border_radius;
    int padding;
    int shadow_width;
} CStyle;

/* Definição do Widget (Elemento da GUI) */
typedef struct {
    char id[64];
    char tag[32];
    int x, y, width, height;
    CStyle style;
    bool visible;
} CWidget;

/* API Principal da Biblioteca */
void cwebui_init();
void cwebui_load_hml(const char* path);
void cwebui_load_css(const char* path);
void cwebui_run();

void cwebui_init_graphics(int width, int height, const char* title);
void cwebui_render_all(CWidget* widgets, int count);
void cwebui_cleanup_graphics();

#endif
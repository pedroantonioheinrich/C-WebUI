#ifndef CWEBUI_H
#define CWEBUI_H

#include <stdint.h>
#include <stdbool.h>

typedef void (*CWebUICallback)();

typedef struct {
    uint32_t bg_color;
    int border_radius;
} CStyle;

typedef struct {
    char id[64];
    char tag[32];
    char label[128];        // Novo: Texto que aparece no widget
    int x, y, width, height;
    CStyle style;
    bool visible;
    bool is_hovered;
    CWebUICallback onclick;
} CWidget;

typedef struct {
    char selector[64];
    CStyle style;
} CStyleMap;

/* API Core */
void cwebui_init();
void cwebui_sync_styles();
void cwebui_run();
CWidget* cwebui_get_widget_by_id(const char* id);
void cwebui_register_widget(CWidget w);

/* API Parser */
void cwebui_load_hml(const char* path);
void cwebui_load_css(const char* path);
int cwebui_get_style_sheet(CStyleMap** out_sheet);

/* API Renderer */
void cwebui_init_graphics(int width, int height, const char* title);
void cwebui_render_all(CWidget* widgets, int count);
void cwebui_cleanup_graphics();

#endif
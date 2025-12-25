#include "cwebui/cwebui.h"
#include <stdio.h>
#include <string.h>

/* Estado Global da Biblioteca */
typedef struct {
    CWidget widgets[256];
    int widget_count;
    bool is_running;
} CWebUI_Context;

static CWebUI_Context ctx;

/* Inicializa o estado interno */
void cwebui_init() {
    ctx.widget_count = 0;
    ctx.is_running = false;
    memset(ctx.widgets, 0, sizeof(ctx.widgets));
    printf("[C-WebUI] Core inicializado com sucesso.\n");
}

/* Busca um widget no pool pelo seu ID único */
CWidget* cwebui_get_widget_by_id(const char* id) {
    for (int i = 0; i < ctx.widget_count; i++) {
        if (strcmp(ctx.widgets[i].id, id) == 0) {
            return &ctx.widgets[i];
        }
    }
    return NULL;
}

/* Busca widgets por Tag (ex: todos os <button>) */
void cwebui_apply_style_to_tag(const char* tag, CStyle style) {
    for (int i = 0; i < ctx.widget_count; i++) {
        if (strcmp(ctx.widgets[i].tag, tag) == 0) {
            ctx.widgets[i].style = style;
        }
    }
}

/* * FUNÇÃO DE SINCRONIZAÇÃO (The Glue)
 * Esta função deve ser chamada após o carregamento do HML e CSS.
 * Ela percorre a lista de widgets e aplica os estilos extraídos.
 */
void cwebui_sync_styles() {
    printf("[C-WebUI] Sincronizando estilos HML <-> CSS...\n");
    // Futuramente: Iterar sobre um mapa de seletores CSS e aplicar aos widgets
}

/* Loop Principal da GUI */
void cwebui_run() {
    ctx.is_running = true;
    printf("[C-WebUI] Entrando no MainLoop...\n");

    while (ctx.is_running) {
        /* 1. Processar Input (Mouse/Teclado) */
        
        /* 2. Atualizar Lógica de Layout */
        
        /* 3. Renderizar Widgets */
        // Para cada widget em ctx.widgets: render_widget(widget);

        /* Simulação de saída para evitar loop infinito no teste inicial */
        ctx.is_running = false; 
    }
}

/* Função para registrar novos widgets no pool (usada pelo HML Parser) */
void cwebui_register_widget(CWidget w) {
    if (ctx.widget_count < 256) {
        ctx.widgets[ctx.widget_count++] = w;
    }
}
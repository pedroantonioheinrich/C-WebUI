#include "cwebui/cwebui.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Banco de dados de estilos (Style Sheet)
static CStyleMap style_sheet[128];
static int style_count = 0;

/**
 * Remove espaços em branco e quebras de linha das extremidades da string.
 */
static char* trim(char* str) {
    char* end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

/**
 * Converte string Hexadecimal (#RRGGBB) para uint32_t.
 */
static uint32_t hex_to_uint32(const char* hex) {
    if (hex[0] == '#') hex++;
    return (uint32_t)strtoul(hex, NULL, 16);
}

/**
 * Retorna o ponteiro para o mapa de estilos global.
 */
int cwebui_get_style_sheet(CStyleMap** out_sheet) {
    *out_sheet = style_sheet;
    return style_count;
}

void cwebui_load_css(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        printf("[C-WebUI] Erro ao abrir CSS: %s\n", path);
        return;
    }

    char line[256];
    bool inside_block = false;

    printf("[C-WebUI] Carregando estilos: %s\n", path);

    while (fgets(line, sizeof(line), file)) {
        char* trimmed = trim(line);

        // Pula linhas vazias ou comentários
        if (strlen(trimmed) == 0 || trimmed[0] == '/') continue;

        // Início do bloco de estilo
        if (strchr(trimmed, '{')) {
            char* selector = strtok(trimmed, " {");
            if (selector && style_count < 128) {
                strncpy(style_sheet[style_count].selector, trim(selector), 63);
                inside_block = true;
            }
            continue;
        }

        // Fim do bloco de estilo
        if (strchr(trimmed, '}')) {
            if (inside_block) {
                style_count++;
            }
            inside_block = false;
            continue;
        }

        // Propriedades (ex: background-color: #ffffff;)
        if (inside_block && strchr(trimmed, ':')) {
            char* prop_raw = strtok(trimmed, ":");
            char* val_raw = strtok(NULL, ";");
            
            if (prop_raw && val_raw) {
                char* prop = trim(prop_raw);
                char* val = trim(val_raw);
                
                CStyle* s = &style_sheet[style_count].style;

                if (strcmp(prop, "background-color") == 0) {
                    s->bg_color = hex_to_uint32(val);
                } else if (strcmp(prop, "border-radius") == 0) {
                    s->border_radius = atoi(val);
                }
            }
        }
    }

    fclose(file);
    printf("[C-WebUI] CSS carregado. %d seletores encontrados.\n", style_count);
}
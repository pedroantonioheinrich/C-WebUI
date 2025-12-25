#include "cwebui/cwebui.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Armazenamento global temporário de widgets para o motor */
#define MAX_WIDGETS 256
static CWidget widget_pool[MAX_WIDGETS];
static int widget_count = 0;

/* Função auxiliar para extrair atributos como id="valor" */
static void extract_attribute(const char* tag, const char* attr, char* dest, int max_len) {
    char* pos = strstr(tag, attr);
    if (pos) {
        pos = strchr(pos, '\"');
        if (pos) {
            pos++; // Pula a primeira aspa
            int i = 0;
            while (pos[i] != '\"' && i < max_len - 1) {
                dest[i] = pos[i];
                i++;
            }
            dest[i] = '\0';
        }
    }
}

void cwebui_load_hml(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        printf("[C-WebUI] Erro ao abrir arquivo HML: %s\n", path);
        return;
    }

    char line[256];
    printf("[C-WebUI] Iniciando parsing do HML: %s...\n", path);

    while (fgets(line, sizeof(line), file)) {
        char* ptr = line;
        
        // Pula espaços iniciais
        while (isspace(*ptr)) ptr++;

        // Verifica se é uma tag de abertura <tag ...>
        if (ptr[0] == '<' && ptr[1] != '/') {
            char tag_content[128];
            int i = 0;
            ptr++; // Pula '<'
            while (*ptr != '>' && *ptr != '\0') {
                tag_content[i++] = *ptr++;
            }
            tag_content[i] = '\0';

            // Criar novo widget no pool
            if (widget_count < MAX_WIDGETS) {
                CWidget* w = &widget_pool[widget_count];
                
                // Extrai o nome da tag (ex: button, window)
                sscanf(tag_content, "%s", w->tag);
                
                // Extrai o ID
                extract_attribute(tag_content, "id=", w->id, 64);

                // Configurações padrão (serão sobrescritas pelo CSS)
                w->visible = true;
                w->x = 0; w->y = 0;
                w->width = 100; w->height = 30;

                printf("  Widget Criado: Tag [%s] | ID [%s]\n", w->tag, w->id);
                widget_count++;
            }
        }
        // Verifica se é uma tag de fechamento </tag>
        else if (ptr[0] == '<' && ptr[1] == '/') {
            // Lógica de fechamento (pop da pilha de hierarquia no futuro)
            continue;
        }
    }

    fclose(file);
    printf("[C-WebUI] Parsing de HML concluído. %d widgets carregados.\n", widget_count);
}
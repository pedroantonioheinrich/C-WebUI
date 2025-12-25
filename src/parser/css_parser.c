#include "cwebui/cwebui.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Função auxiliar para remover espaços em branco nas extremidades */
static char* trim(char* str) {
    char* end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

/* Converte string Hexadecimal (#1F6AA5) para uint32_t */
static uint32_t hex_to_uint32(const char* hex) {
    if (hex[0] == '#') hex++;
    return (uint32_t)strtoul(hex, NULL, 16);
}

/* Extrai apenas o número de strings como "20px" ou "10" */
static int parse_int_value(const char* value) {
    return atoi(value);
}

void cwebui_load_css(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        printf("[C-WebUI] Erro ao abrir arquivo CSS: %s\n", path);
        return;
    }

    char line[256];
    char current_selector[64] = "";
    bool inside_block = false;

    printf("[C-WebUI] Iniciando parsing do CSS: %s...\n", path);

    while (fgets(line, sizeof(line), file)) {
        char* trimmed = trim(line);

        // Identifica o início de um bloco (ex: #btn_action {)
        if (strchr(trimmed, '{')) {
            char* selector = strtok(trimmed, "{");
            strncpy(current_selector, trim(selector), 63);
            inside_block = true;
            continue;
        }

        // Identifica o fim de um bloco
        if (strchr(trimmed, '}')) {
            inside_block = false;
            memset(current_selector, 0, 64);
            continue;
        }

        // Processa propriedades dentro do bloco
        if (inside_block && strchr(trimmed, ':')) {
            char* prop = strtok(trimmed, ":");
            char* val = strtok(NULL, ";");

            if (prop && val) {
                char* p = trim(prop);
                char* v = trim(val);

                /* LOG DE DEPURAÇÃO: Aqui mapearemos para a Struct CWidget no futuro */
                printf("  Selector [%s] -> Property: %s | Value: %s\n", current_selector, p, v);

                // Exemplo de mapeamento inicial para a lógica de estilo
                if (strcmp(p, "background-color") == 0) {
                    uint32_t color = hex_to_uint32(v);
                    // Futuramente: buscar widget por ID e setar style.bg_color
                } else if (strcmp(p, "border-radius") == 0) {
                    int radius = parse_int_value(v);
                    // Futuramente: style.border_radius = radius
                }
            }
        }
    }

    fclose(file);
    printf("[C-WebUI] Parsing de CSS concluído com sucesso.\n");
}
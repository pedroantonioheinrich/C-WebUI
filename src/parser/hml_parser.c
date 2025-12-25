#include "cwebui/cwebui.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/**
 * Extrai valores de atributos como id="nome" ou class="nome".
 */
static void extract_attribute(const char* tag, const char* attr, char* dest, int max_len) {
    char* pos = strstr(tag, attr);
    if (pos) {
        pos = strchr(pos, '\"');
        if (pos) {
            pos++;
            int i = 0;
            while (pos[i] != '\"' && i < max_len - 1 && pos[i] != '\0') {
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
        printf("[C-WebUI] Erro ao abrir HML: %s\n", path);
        return;
    }

    char line[512];
    int offset = 20;

    while (fgets(line, sizeof(line), file)) {
        char* ptr = line;
        while (isspace(*ptr)) ptr++;

        // Detecta abertura de tag: <button ...
        if (ptr[0] == '<' && ptr[1] != '/') {
            char tag_content[256];
            int i = 0; 
            ptr++; // Pula o '<'

            while (*ptr != '>' && *ptr != '\0' && i < 255) {
                tag_content[i++] = *ptr++;
            }
            tag_content[i] = '\0';

            // Agora a variável 'w' engloba toda a lógica da tag
            CWidget w;
            memset(&w, 0, sizeof(CWidget));
            
            // Define a TAG (ex: button, label)
            sscanf(tag_content, "%s", w.tag);
            
            // Extrai o ID
            extract_attribute(tag_content, "id=", w.id, 64);

            // Captura o TEXTO entre as tags: >TEXTO</
            if (*ptr == '>') {
                ptr++;
                int j = 0;
                while (*ptr != '<' && *ptr != '\0' && j < 127) {
                    // Limpeza: Só adiciona se não for espaço no início
                    if (j > 0 || !isspace((unsigned char)*ptr)) {
                        w.label[j++] = *ptr;
                    }
                    ptr++;
                }
                w.label[j] = '\0';

                // Trim right: remove espaços do final
                while (j > 0 && isspace((unsigned char)w.label[j-1])) {
                    w.label[--j] = '\0';
                }
            }

            // Configurações padrão de layout (provisório)
            w.visible = true;
            w.width = 200; 
            w.height = 40;
            w.x = 50; 
            w.y = offset;
            offset += 55;
            
            // Registra o widget no sistema global
            cwebui_register_widget(w);
            printf("[C-WebUI] Widget carregado: %s (ID: %s) Texto: [%s]\n", w.tag, w.id, w.label);
        }
    }
    fclose(file);
}
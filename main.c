#include "cwebui/cwebui.h"
#include <stdlib.h> // Necessário para a função exit()
#include <stdio.h>

/**
 * Função de callback para o botão.
 * Em C, definimos funções fora do main.
 */
void fechar_app() {
    printf("[App] Botão de ação clicado! Encerrando...\n");
    exit(0);
}

int main() {
    // 1. Inicializa o Motor (Cria janela e contextos)
    cwebui_init();

    // 2. Carrega a estrutura e o design
    cwebui_load_hml("examples/index.hml");
    cwebui_load_css("examples/style.css");

    // 3. Sincroniza os estilos CSS com os Widgets HML
    cwebui_sync_styles();

    // 4. Atribui lógica aos widgets (Event Binding)
    // Buscamos o widget pelo ID definido no index.hml
    CWidget* btn = cwebui_get_widget_by_id("btn_action");
    if (btn) {
        btn->onclick = fechar_app;
        printf("[App] Evento vinculado ao widget: %s\n", btn->id);
    }

    // 5. Inicia o Loop principal de renderização e eventos
    cwebui_run();

    return 0;
}
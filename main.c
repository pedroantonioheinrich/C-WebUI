#include "cwebui/cwebui.h"

int main() {
    // Inicializa o Motor
    cwebui_init();

    // Carrega os arquivos que criamos na pasta examples/
    cwebui_load_hml("examples/index.hml");
    cwebui_load_css("examples/style.css");

    // Sincroniza e Roda
    cwebui_sync_styles();
    cwebui_run();

    return 0;
}
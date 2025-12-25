# C-WebUI Engine (v0.1)

**C-WebUI** é um motor de interface gráfica ultra-minimalista escrito inteiramente em **C puro**, que permite o desenvolvimento de GUIs modernas (estilo CustomTkinter) utilizando uma estrutura baseada em **HTML** e estilização via **CSS**.

O objetivo é separar completamente a **Lógica de Negócios (C)** da **Interface e Estilo (HML/CSS)**, eliminando a necessidade de reconstruir a interface programaticamente a cada mudança visual.

---

## 🏗️ Arquitetura do Sistema

O motor opera através de quatro pilares fundamentais:

1. **Parser HML (Hyperlink Markup Language):** Um analisador léxico que converte tags de texto em uma árvore de objetos (`DOM`) na memória RAM usando `structs` de C.
2. **Parser CSS (Cascading Style Sheets):** Um motor de busca que mapeia seletores (ID e Classes) para aplicar propriedades visuais como `border-radius`, `background-color` e `box-shadow`.
3. **Binding Engine:** A ponte que conecta os eventos definidos no HML (ex: `onclick`) a funções de callback reais dentro do seu código C.
4. **Renderer (Backend):** Atualmente projetado para renderizar via **LVGL/SDL2**, garantindo aceleração de hardware e visual moderno.

---

## 🎨 Sintaxe da Linguagem

### 1. Estrutura (index.hml)
Diferente do HTML web, o **HML** foca em componentes de sistema operacional.

```html
<window id="main_window" title="Meu App Moderno">
    <container id="sidebar">
        <button id="nav_home" onclick="go_home">Início</button>
        <button id="nav_settings" onclick="go_settings">Configurações</button>
    </container>
    
    <main_view id="content">
        <text id="welcome_msg">Bem-vindo ao C-WebUI</text>
    </main_view>
</window>
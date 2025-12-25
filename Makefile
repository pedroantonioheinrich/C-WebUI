# Nome do executável final
TARGET = cwebui_app

# Compilador e Flags
CC = gcc
CFLAGS = -Wall -Wextra -I./include -I./include/cwebui `sdl2-config --cflags`
LDFLAGS = -lSDL2 -lSDL2_ttf

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

# Lista de todos os arquivos fonte (.c) encontrados nas subpastas de src/
SOURCES = main.c \
          $(SRC_DIR)/core/main_core.c \
          $(SRC_DIR)/parser/css_parser.c \
          $(SRC_DIR)/parser/hml_parser.c \
          $(SRC_DIR)/renderer/sdl_renderer.c

# Converte a lista de fontes em uma lista de arquivos objeto (.o) na pasta obj/
OBJECTS = $(SOURCES:%.c=$(OBJ_DIR)/%.o)

# Regra principal (padrão)
all: $(TARGET)

# Cria o executável final
$(TARGET): $(OBJECTS)
	@echo "🔗 Vinculando: $@"
	@$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "✅ Compilação concluída com sucesso!"

# Regra para compilar cada arquivo .c em um .o
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "🔨 Compilando: $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Regra para rodar o projeto
run: all
	@echo "🚀 Iniciando $(TARGET)..."
	@./$(TARGET)

# Limpa os arquivos de compilação
clean:
	@echo "🧹 Limpando arquivos objeto e binário..."
	@rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean run
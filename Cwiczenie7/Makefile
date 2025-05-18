# Kompilator i flagi
CC = gcc
CFLAGS = -Wall -g -pedantic -std=c99 -I./include
LIB_DIR = ./lib

# Ścieżki
SRC_DIR = ./source
INCLUDE_DIR = ./include

# Pliki źródłowe
SRC_FILES = $(SRC_DIR)/semaphore.c $(SRC_DIR)/shared_memory.c
OBJ_FILES = $(SRC_FILES:.c=.o)

# Nazwy bibliotek
STATIC_LIB = $(LIB_DIR)/libmymodule.a
SHARED_LIB = $(LIB_DIR)/libmymodule.so

# Programy binarne
MAIN_NAME ?= ./main
PRODUCENT_NAME ?= ./producent
KONSUMENT_NAME ?= ./konsument

# Pliki danych
INPUT_FILE ?= input.txt
OUTPUT_FILE ?= output.txt

# Linkowanie
LDFLAGS_STATIC = -L$(LIB_DIR) -lmymodule -lrt -pthread
LDFLAGS_SHARED = -L$(LIB_DIR) -lmymodule -Wl,-rpath,$(LIB_DIR) -lrt -pthread

#=============================================================================
# Domyślny cel
#=============================================================================
all: static shared

#=============================================================================
# Budowanie bibliotek
#=============================================================================
$(STATIC_LIB): $(OBJ_FILES)
	mkdir -p $(LIB_DIR)
	ar rcs $@ $^

$(SHARED_LIB): $(OBJ_FILES)
	mkdir -p $(LIB_DIR)
	$(CC) -shared -o $@ $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

#=============================================================================
# Budowanie wersji statycznej
#=============================================================================
static: $(STATIC_LIB) main.o producent.o konsument.o
	$(CC) $(CFLAGS) -o $(MAIN_NAME) main.o $(LDFLAGS_STATIC)
	$(CC) $(CFLAGS) -o $(PRODUCENT_NAME) producent.o $(LDFLAGS_STATIC)
	$(CC) $(CFLAGS) -o $(KONSUMENT_NAME) konsument.o $(LDFLAGS_STATIC)

#=============================================================================
# Budowanie wersji dynamicznej
#=============================================================================
shared: $(SHARED_LIB) main.o producent.o konsument.o
	$(CC) $(CFLAGS) -o $(MAIN_NAME) main.o $(LDFLAGS_SHARED)
	$(CC) $(CFLAGS) -o $(PRODUCENT_NAME) producent.o $(LDFLAGS_SHARED)
	$(CC) $(CFLAGS) -o $(KONSUMENT_NAME) konsument.o $(LDFLAGS_SHARED)

#=============================================================================
# Kompilacja plików .c
#=============================================================================
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

#=============================================================================
# Uruchomienie
#=============================================================================
run_static: static
	$(MAIN_NAME) $(PRODUCENT_NAME) $(KONSUMENT_NAME) $(INPUT_FILE) $(OUTPUT_FILE)

run_shared: shared
	LD_LIBRARY_PATH=$(LIB_DIR):$$LD_LIBRARY_PATH $(MAIN_NAME) $(PRODUCENT_NAME) $(KONSUMENT_NAME) $(INPUT_FILE) $(OUTPUT_FILE)

#=============================================================================
# Czyszczenie
#=============================================================================
clean:
	rm -f *.o *.a *.so *.x *~ core core.* a.out
	rm -f $(SRC_DIR)/*.o
	rm -f $(MAIN_NAME) $(PRODUCENT_NAME) $(KONSUMENT_NAME) $(OUTPUT_FILE)
	rm -f $(STATIC_LIB) $(SHARED_LIB)
	rm -rf $(LIB_DIR)

#=============================================================================
# Archiwizacja
#=============================================================================
tar: clean
	(cd .. && tar czf Cwiczenie7.tar.gz Cwiczenie7)

#=============================================================================
# Pomoc
#=============================================================================
help:
	@echo "----------------------------------------------------"
	@echo " Dostępne cele:"
	@echo "  make             - Kompilacja wersji statycznej i dynamicznej"
	@echo "  make static      - Kompilacja tylko wersji statycznej"
	@echo "  make shared      - Kompilacja tylko wersji dynamicznej"
	@echo "  make run_static  - Uruchomienie programu z biblioteką statyczną"
	@echo "  make run_shared  - Uruchomienie programu z biblioteką dynamiczną"
	@echo "  make clean       - Usunięcie plików tymczasowych"
	@echo "  make tar         - Spakowanie katalogu do Cwiczenie7.tar.gz"
	@echo ""
	@echo "  Zmienne (można nadpisać np. make run_static INPUT_FILE=data.txt):"
	@echo "     MAIN_NAME=$(MAIN_NAME)"
	@echo "     PRODUCENT_NAME=$(PRODUCENT_NAME)"
	@echo "     KONSUMENT_NAME=$(KONSUMENT_NAME)"
	@echo "     INPUT_FILE=$(INPUT_FILE)"
	@echo "     OUTPUT_FILE=$(OUTPUT_FILE)"
	@echo "----------------------------------------------------"

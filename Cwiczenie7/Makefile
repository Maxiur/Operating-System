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
TARGETS = main producent konsument

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
static: $(STATIC_LIB) $(TARGETS)

$(TARGETS): %: %.o $(STATIC_LIB)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS_STATIC)

#=============================================================================
# Budowanie wersji dynamicznej (shared1)
#=============================================================================
shared: $(SHARED_LIB) $(addsuffix _shared, $(TARGETS))

%_shared: %.o $(SHARED_LIB)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS_SHARED)

#=============================================================================
# Kompilacja plików .c
#=============================================================================
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

#=============================================================================
# Uruchomienie
#=============================================================================
run_static: static
	./main ./producent ./konsument $(INPUT_FILE) $(OUTPUT_FILE)
	@echo "Wynik porównania plików: $(OUTPUT_FILE) i $(INPUT_FILE)"
	diff -s $(OUTPUT_FILE) $(INPUT_FILE)

run_shared1: shared
	./main_shared ./producent_shared ./konsument_shared $(INPUT_FILE) $(OUTPUT_FILE)
	@echo "Wynik porównania plików: $(OUTPUT_FILE) i $(INPUT_FILE)"
	diff -s $(OUTPUT_FILE) $(INPUT_FILE)

run_shared2: shared
	LD_LIBRARY_PATH=$(LIB_DIR):$$LD_LIBRARY_PATH ./main_shared ./producent_shared ./konsument_shared $(INPUT_FILE) $(OUTPUT_FILE)
	@echo "Wynik porównania plików: $(OUTPUT_FILE) i $(INPUT_FILE)"
	diff -s $(OUTPUT_FILE) $(INPUT_FILE)

#=============================================================================
# Czyszczenie
#=============================================================================
clean:
	rm -f *.o *.a *.so *.x *~ core core.* a.out
	rm -f $(SRC_DIR)/*.o
	rm -f $(TARGETS) $(addsuffix _shared, $(TARGETS)) $(OUTPUT_FILE)
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
	@echo "  make shared      - Kompilacja tylko wersji dynamicznej (shared1)"
	@echo "  make run_static  - Uruchomienie programu z biblioteką statyczną"
	@echo "  make run_shared1 - Uruchomienie programu z biblioteką dynamiczną (rpath)"
	@echo "  make run_shared2 - Uruchomienie programu z biblioteką dynamiczną (LD_LIBRARY_PATH)"
	@echo "  make clean       - Usunięcie plików tymczasowych"
	@echo "  make tar         - Spakowanie katalogu do Cwiczenie7.tar.gz"
	@echo ""
	@echo "  Zmienne (można nadpisać np. make run_static INPUT_FILE=data.txt):"
	@echo "     INPUT_FILE=$(INPUT_FILE)"
	@echo "     OUTPUT_FILE=$(OUTPUT_FILE)"
	@echo "----------------------------------------------------"

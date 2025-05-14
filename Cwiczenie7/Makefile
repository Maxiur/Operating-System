#=============================================================================
# Makefile do cwiczenia 7 - Producent-Konsument z pamiecia dzielona i semaforami
#=============================================================================

# Kompilator i flagi
CC = gcc
CFLAGS = -Wall -g -pedantic -std=c99 -I./include
LIB_DIR = ./lib

# Pliki zrodlowe
SRC_DIR = ./source
INCLUDE_DIR = ./include
SRC_FILES = $(SRC_DIR)/semaphore.c $(SRC_DIR)/shared_memory.c
OBJ_FILES = $(SRC_FILES:.c=.o)

# Nazwy bibliotek
STATIC_LIB = $(LIB_DIR)/libmymodule.a
SHARED_LIB = $(LIB_DIR)/libmymodule.so

# Programy
BINARIES = main producent konsument

# Linkowanie
LDFLAGS_STATIC = -L$(LIB_DIR) -lmymodule -lrt -pthread
LDFLAGS_SHARED = -L$(LIB_DIR) -lmymodule -Wl,-rpath,$(LIB_DIR) -lrt -pthread

#=============================================================================
# Domyslny cel
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
	$(CC) $(CFLAGS) -o main main.o $(LDFLAGS_STATIC)
	$(CC) $(CFLAGS) -o producent producent.o $(LDFLAGS_STATIC)
	$(CC) $(CFLAGS) -o konsument konsument.o $(LDFLAGS_STATIC)

#=============================================================================
# Budowanie wersji dynamicznej
#=============================================================================
shared: $(SHARED_LIB) main.o producent.o konsument.o
	$(CC) $(CFLAGS) -o main main.o $(LDFLAGS_SHARED)
	$(CC) $(CFLAGS) -o producent producent.o $(LDFLAGS_SHARED)
	$(CC) $(CFLAGS) -o konsument konsument.o $(LDFLAGS_SHARED)

#=============================================================================
# Kompilacja pojedynczych plikow
#=============================================================================
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

#=============================================================================
# Uruchamianie
#=============================================================================
run_static: static
	./main

run_shared: shared
	LD_LIBRARY_PATH=$(LIB_DIR):$$LD_LIBRARY_PATH ./main

#=============================================================================
# Czyszczenie
#=============================================================================
clean:
	rm -f *.o *.a *.so *.x *~ core core.* a.out
	rm -f $(SRC_DIR)/*.o
	rm -f $(BINARIES) $(DYNAMIC_BINARIES)
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
	@echo " Dostepne cele:"
	@echo "  make             - Kompilacja wersji statycznej i dynamicznej"
	@echo "  make static      - Kompilacja tylko wersji statycznej (main, producent, konsument)"
	@echo "  make shared      - Kompilacja tylko wersji dynamicznej (main, producent, konsument)"
	@echo "  make run_static  - Uruchomienie programu z biblioteka statyczna"
	@echo "  make run_shared  - Uruchomienie programu z biblioteka dynamiczna"
	@echo "  make clean       - Usuniecie plikow posrednich i bibliotek"
	@echo "  make tar         - Spakowanie katalogu do Cwiczenie7.tar.gz"
	@echo "  make help        - Wyswietlenie tej pomocy"
	@echo "----------------------------------------------------"

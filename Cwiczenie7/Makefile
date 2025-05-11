.SUFFIXES: .c .o .x .h

# Kompilator
CC = gcc
CFLAGS = -Wall -g -pedantic -std=c99 -I$(INCLUDE_DIR)

# Katalogi
PARENT_DIR  = $(notdir $(CURDIR))
SOURCE_DIR  = ./source
INCLUDE_DIR = ./include

# Źródła biblioteczne
LIB_SRC = $(SOURCE_DIR)/semaphore.c $(SOURCE_DIR)/shared_memory.c
LIB_OBJ = $(LIB_SRC:.c=.o)

# Pliki główne
MAIN_SRC = main.c
PROD_SRC = producent.c
KONS_SRC = konsument.c

MAIN_OBJ = $(MAIN_SRC:.c=.o)
PROD_OBJ = $(PROD_SRC:.c=.o)
KONS_OBJ = $(KONS_SRC:.c=.o)

MAIN_EXE = main
PROD_EXE = producent
KONS_EXE = konsument

# Domyślny cel
all: $(MAIN_EXE) $(PROD_EXE) $(KONS_EXE)

# Kompilacja plików .c do .o
%.o: %.c $(INCLUDE_DIR)/shared_buffer.h
	$(CC) $(CFLAGS) -c $< -o $@

# Budowanie programów
$(MAIN_EXE): $(MAIN_OBJ) $(LIB_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(PROD_EXE): $(PROD_OBJ) $(LIB_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(KONS_EXE): $(KONS_OBJ) $(LIB_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Uruchomienie programu głównego
.PHONY: run
run: all
	./$(MAIN_EXE)

# Czyszczenie
.PHONY: clean
clean:
	rm -f *.o *~ core core.* a.out \
		$(MAIN_EXE) $(PROD_EXE) $(KONS_EXE)
	rm -f $(SOURCE_DIR)/*.o

# Archiwizacja katalogu
.PHONY: tar
tar: clean
	(cd .. && tar cvzf $(PARENT_DIR).tar.gz $(PARENT_DIR) && ls -altr)

# Pomoc
.PHONY: help
help:
	@echo "----------------------------------------------------"
	@echo " Dostępne cele:"
	@echo "  make              - Kompilacja wszystkich programów (main, producent, konsument)"
	@echo "  make run          - Uruchomienie programu main"
	@echo "  make clean        - Czyszczenie plików wynikowych"
	@echo "  make tar          - Spakowanie katalogu"
	@echo "  make help         - Wyświetlenie tej pomocy"
	@echo "----------------------------------------------------"

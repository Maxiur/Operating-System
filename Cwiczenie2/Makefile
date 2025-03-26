.SUFFIXES: .c .cpp .o .x .h
SYSNAME = $(shell uname -s)

# Kompilator
CC = gcc
CFLAGS = -Wall -g -pedantic -std=c99
PROGRAM ?= ./komunikat

# Nazwa katalogu
PARENT_DIR = $(notdir $(CURDIR))

# Pliki wynikowe
MAIN_NAME = 2a
SRC_OBJS = $(MAIN_NAME).c
OBJS = $(MAIN_NAME).o
TARGET = program
ARG = komunikat

# Kompilacja ARG
$(ARG): $(ARG).c
	$(CC) $(CFLAGS) -o $(ARG) $(ARG).c

# Reguła główna
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Kompilacja plików obiektowych
$(OBJS): $(SRC_OBJS)
	$(CC) $(CFLAGS) -c $(SRC_OBJS) -o $(OBJS)

all: $(ARG) $(TARGET)

# Czyszczenie
.PHONY: clean
clean:
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf $(TARGET) $(PROGRAM)

# Uruchomienie pliku
.PHONY: run
run: $(ARG) $(TARGET) 
	./$(TARGET) $(PROGRAM)

# Pakowanie i archiwizacja
.PHONY: clean
tar: clean
	(cd .. && tar cvzf $(PARENT_DIR).tar.gz $(PARENT_DIR) && ls -altr)

# Dostepne komendy
.PHONY: help
help:
	@echo "----------------------------------------------------"
	@echo ""
	@echo "Dostępne cele:"
	@echo "  make             - Kompilacja i linkowanie plików"
	@echo "  make run         - Uruchomienie programu z danymi wejściowymi"
	@echo "  make clean       - Usunięcie plików obiektowych i programu"
	@echo "  make tar         - Spakowanie katalogu do archiwum .tar.gz"
	@echo "  make help        - Wyświetlenie tej pomocy"
	@echo "  make run PROGRAM=./nazwa_programu - uruchamia z własnym programem"
	@echo ""
	@echo "----------------------------------------------------"

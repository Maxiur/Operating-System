.SUFFIXES: .c .cpp .o .x .h
SYSNAME = $(shell uname -s)

# Kompilator
CC = gcc
CFLAGS = -Wall -g -pedantic -std=c99

# Nazwa katalogu
PARENT_DIR = $(notdir $(CURDIR))

# Pliki wynikowe
MAIN_NAME = 1c
SRC_OBJS = $(MAIN_NAME).c
OBJS = $(MAIN_NAME).o
TARGET = program
#DATA = dane.txt
#RESULT = wyniki.txt

# Reguła główna
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Kompilacja plików obiektowych
$(OBJS): $(SRC_OBJS)
	$(CC) $(CFLAGS) -c $(SRC_OBJS) -o $(OBJS)

# Czyszczenie
.PHONY: clean
clean:
	rm -f *.o $(TARGET)

# Uruchomienie pliku
.PHONY: run
run: $(TARGET)
	./$(TARGET)

# Pakowanie i archiwizacja
.PHONY: clean
tar: clean
	(cd .. && tar cvzf $(PARENT_DIR).tar.gz $(PARENT_DIR) && ls -altr)

# Dostepne komendy
.PHONY: help
help:
	@echo "----------------------------------------------------"
	@echo "Dostępne cele:"
	@echo "  make             - Kompilacja i linkowanie plików"
	@echo "  make run         - Uruchomienie programu z danymi wejściowymi"
	@echo "  make clean       - Usunięcie plików obiektowych i programu"
	@echo "  make tar         - Spakowanie katalogu do archiwum .tar.gz"
	@echo "  make help        - Wyświetlenie tej pomocy"


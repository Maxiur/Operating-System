.SUFFIXES: .c .o .x .h

# Kompilator
CC = gcc
CFLAGS = -Wall -g -pedantic -std=c99 -I$(INCLUDE_DIR)

# Nazwy katalogów
PARENT_DIR = $(notdir $(CURDIR))
SOURCE_DIR = ./source
INCLUDE_DIR = ./include

# Parametry uruchomienia
PROCESS ?= 5
SEKCJE ?= 10

# Pliki źródłowe i wynikowe
SEM_SRC = $(SOURCE_DIR)/my_semaphore.c
SEM_OBJ = $(SEM_SRC:.c=.o)

DUPLICATOR_SRC = duplicator.c
DUPLICATOR_OBJ = $(DUPLICATOR_SRC:.c=.o)

WORKER_SRC = critical_worker.c
WORKER_OBJ = $(WORKER_SRC:.c=.o)

DUPLICATOR_EXE = duplicator
WORKER_EXE = critical_worker

# Domyślny cel
all: $(DUPLICATOR_EXE) $(WORKER_EXE)

# Kompilacja .c -> .o
%.o: %.c $(INCLUDE_DIR)/my_semaphore.h
	$(CC) $(CFLAGS) -c $< -o $@

# Budowanie plików wykonywalnych
$(DUPLICATOR_EXE): $(DUPLICATOR_OBJ) $(SEM_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(WORKER_EXE): $(WORKER_OBJ) $(SEM_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Uruchomienie
.PHONY: run
run: all
	./$(DUPLICATOR_EXE) ./$(WORKER_EXE) 5 10

# Czyszczenie
.PHONY: clean
clean:
	rm -f *.o *~ *.x core core* a.out \
		$(DUPLICATOR_EXE) $(WORKER_EXE) numer.txt

# Archiwizacja
.PHONY: tar
tar: clean
	(cd .. && tar cvzf $(PARENT_DIR).tar.gz $(PARENT_DIR) && ls -altr)

# Pomoc
.PHONY: help
help:
	@echo "----------------------------------------------------"
	@echo " Dostępne cele:"
	@echo "  make              - Kompilacja duplicatora i workera (ćw. 6)"
	@echo "  make run          - Uruchomienie duplicatora z workerem"
	@echo "  make clean        - Czyszczenie plików wynikowych"
	@echo "  make tar          - Spakowanie katalogu"
	@echo "  make help         - Wyświetlenie tej pomocy"
	@echo "----------------------------------------------------"
.SUFFIXES: .c .cpp .o .x .h
SYSNAME = $(shell uname -s)

# Kompilator
CC = gcc
CFLAGS = -Wall -g -pedantic -std=c99

# Nazwa katalogu
PARENT_DIR = $(notdir $(CURDIR))

# Pliki wynikowe
A_SRC = 3a.c
A_OBJ = 3a.o
A_EXE = program_a

B_SRC = 3b.c
B_OBJ = 3b.o
B_EXE = program_b

# Reguła główna
all: $(A_EXE) $(B_EXE)

# Kompilacja plików wykonywalnych
$(A_EXE): $(A_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(B_EXE): $(B_OBJ) $(A_EXE)
	$(CC) $(CFLAGS) -o $@ $(B_OBJ)

# Kompilacja plików obiektowych
$(A_OBJ): $(A_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(B_OBJ): $(B_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

# Czyszczenie
.PHONY: clean
clean:
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf $(A_EXE) $(B_EXE)

# Uruchomienie pliku
.PHONY: run_a
run_a: $(A_EXE))
	./$(A_EXE)

.PHONY: run_b
run_b: $(B_EXE)
	./$(B_EXE) ./$(A_EXE)

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
	@echo ""
	@echo "----------------------------------------------------"

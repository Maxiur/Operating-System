.SUFFIXES: .c .o .x .h

# Kompilator
CC = gcc
CFLAGS = -Wall -g -pedantic -std=c99

# Nazwa katalogu
PARENT_DIR = $(notdir $(CURDIR))

# Nazwy plików
MAIN_SRC = 5a.c
MAIN_OBJ = 5a.o
MAIN_EXE = producent_konsument

WRITE_SRC = write_fifo.c
WRITE_OBJ = write_fifo.o
WRITE_EXE = write_fifo

READ_SRC = read_fifo.c
READ_OBJ = read_fifo.o
READ_EXE = read_fifo

# Pliki tekstowe oraz FIFO
INPUT ?= input.txt
OUTPUT ?= output.txt
FIFO = mojfifo
FIFO_PATH ?= ./temp/$(FIFO)

# Domyślny cel
all: $(MAIN_EXE) $(WRITE_EXE) $(READ_EXE)

# Kompilacja plików .c do .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Budowanie plików wykonywalnych
$(MAIN_EXE): $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(WRITE_EXE): $(WRITE_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(READ_EXE): $(READ_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Tworzenie FIFO z powłoki
mkfifo:
	@echo "Tworzę potok FIFO: $(FIFO)"
	-mkfifo $(FIFO)
# Uruchomienie
.PHONY: run
run: all
	@echo "Tworzenie katalogu temp/ (jeśli nie istnieje)..."
	mkdir -p temp
	./$(MAIN_EXE) $(INPUT) $(OUTPUT) $(FIFO_PATH)
	@echo "\nSprawdzenie zgodności plików:"
	diff -s $(INPUT) $(OUTPUT)

.PHONY: run_b
run_b: mkfifo
	@echo "Uruchamiam Producenta i Konsumenta w xterm..."
	xterm -hold -title PRODUCENT -e ./$(WRITE_EXE) $(INPUT) $(FIFO) &
	xterm -hold -title KONSUMENT -e ./$(READ_EXE) $(OUTPUT) $(FIFO)

# Sprawdzenie zgodności plików
diff:
	@echo "Sprawdzam zgodność plików:"
	diff -s $(INPUT) $(OUTPUT)

# Czyszczenie
.PHONY: clean
clean:
	rm -f *.o *~ *.x core core* a.out $(MAIN_EXE) $(WRITE_EXE) $(READ_EXE) $(OUTPUT) $(FIFO)
	rm -rf temp

# Archiwizacja katalogu
.PHONY: tar
tar: clean
	(cd .. && tar cvzf $(PARENT_DIR).tar.gz $(PARENT_DIR) && ls -altr)

# Pomoc
.PHONY: help
help:
	@echo "----------------------------------------------------"
	@echo " Dostępne cele:"
	@echo "  make             - Kompilacja wszystkich programów"
	@echo "  make run         - Uruchomienie programu z input/output/FIFO"
	@echo "  make clean       - Usunięcie plików tymczasowych i FIFO"
	@echo "  make tar         - Spakowanie katalogu do .tar.gz"
	@echo "  make help        - Wyświetlenie tej pomocy"
	@echo "----------------------------------------------------"


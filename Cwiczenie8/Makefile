#=============================================================================
# Ścieżki
#=============================================================================
SRC_DIR     := source
INCLUDE_DIR := include

# Nazwa katalogu
PARENT_DIR = $(notdir $(CURDIR))

#=============================================================================
# Kompilator i flagi
#=============================================================================
CC        := gcc
CFLAGS    := -Wall -Wextra -g -std=c99 -pedantic -I$(INCLUDE_DIR)
LDFLAGS   := -lrt

#=============================================================================
# Pliki źródłowe i obiekty
#=============================================================================
MQUEUE_SRC := $(SRC_DIR)/my_mqueue.c
MQUEUE_OBJ := $(MQUEUE_SRC:.c=.o)

CLIENT_SRC := klient.c
SERVER_SRC := serwer.c
CLIENT_OBJ := $(CLIENT_SRC:.c=.o)
SERVER_OBJ := $(SERVER_SRC:.c=.o)

#=============================================================================
# Binarne
#=============================================================================
CLIENT_BIN := klient
SERVER_BIN := serwer

#=============================================================================
# Domyślny cel
#=============================================================================
.PHONY: all
all: $(CLIENT_BIN) $(SERVER_BIN)

#=============================================================================
# Linkowanie programów
#=============================================================================
$(CLIENT_BIN): $(CLIENT_OBJ) $(MQUEUE_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SERVER_BIN): $(SERVER_OBJ) $(MQUEUE_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

#=============================================================================
# Reguły kompilacji obiektów
#=============================================================================
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

#=============================================================================
# Uruchomienie serwera i kilku klientów w xterm
#=============================================================================
.PHONY: run
run: all
	@echo "Uruchamiam serwer..."
	xterm -hold -title SERWER   -bg red   -fg black -e ./$(SERVER_BIN) &
	@sleep 1
	@echo "Uruchamiam klienta 1..."
	xterm -hold -title KLIENT1  -bg green -fg black -e ./$(CLIENT_BIN) &
	@sleep 1
	@echo "Uruchamiam klienta 2..."
	xterm -hold -title KLIENT2  -bg green -fg black -e ./$(CLIENT_BIN) &

#=============================================================================
# Czyszczenie
#=============================================================================
.PHONY: clean
clean:
	rm -f *.o $(SRC_DIR)/*.o $(CLIENT_BIN) $(SERVER_BIN)

# Pakowanie i archiwizacja
.PHONY: clean
tar: clean
	(cd .. && tar cvzf $(PARENT_DIR).tar.gz $(PARENT_DIR) && ls -altr)

#=============================================================================
# Pomoc
#=============================================================================
.PHONY: help
help:
	@echo "----------------------------------------------------"
	@echo " Dostępne cele:"
	@echo "  make           - kompiluje klienta i serwer"
	@echo "  make run       - uruchamia serwer i klienta w xterm"
	@echo "  make clean     - usuwa pliki obiektów i binaria"
	@echo "  make help      - wyświetla tę pomoc"
	@echo "----------------------------------------------------"

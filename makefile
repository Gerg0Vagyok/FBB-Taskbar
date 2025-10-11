CC=clang
CFLAGS=-Wall -Wextra -O2
LDFLAGS=

SRC_DIR=src
BIN_DIR=bin

TARGET=$(BIN_DIR)/main
SOURCES=main.c
SRC_OBJECTS=$(addprefix $(SRC_DIR)/, $(SOURCES))
BIN_OBJECTS=$(addprefix $(BIN_DIR)/, $(SOURCES:.c=.o))

.PHONY: all clean debug

all: $(TARGET)

debug: CFLAGS=-Wall -Wextra -g -O0
debug: clean $(TARGET)

$(TARGET): $(BIN_OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(BIN_OBJECTS) $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(TARGET) $(BIN_OBJECTS)

CC=clang
CXX=clang++
CFLAGS=-Wall -Wextra -O2
CXXFLAGS=-Wall -Wextra -O2 -std=c++17 $(shell pkg-config --cflags Qt6Gui Qt6Core Qt6Widgets) -I/usr/include/LayerShellQt
LDFLAGS=$(shell pkg-config --libs Qt6Gui Qt6Core Qt6Widgets) -lLayerShellQtInterface

SRC_DIR=src
BIN_DIR=bin

C_SOURCES=$(wildcard $(SRC_DIR)/*.c)
CPP_SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
C_OBJECTS=$(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o,$(C_SOURCES))
CPP_OBJECTS=$(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(CPP_SOURCES))
OBJECTS=$(C_OBJECTS) $(CPP_OBJECTS)
TARGET=$(BIN_DIR)/main

.PHONY: all clean debug bear compile

all: bear

compile: $(TARGET)

bear:
	bear -- make compile

debug: CFLAGS=-Wall -Wextra -g -O0
debug: CXXFLAGS=-Wall -Wextra -g -O0
debug: clean $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(TARGET) $(OBJECTS)

CC := g++
ARTIFACT := catan.exe
SRC_DIR := src
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_DIR := bin
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
INC := -Iinclude
LIB := -lpdcurses
CPPFLAGS := $(INC) -MMD -MP
CFLAGS   := -Wall -std=c++11 -g

all: $(ARTIFACT)

$(ARTIFACT): $(OBJ)
	$(CC) $^ $(LIB) $(CFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(ARTIFACT)

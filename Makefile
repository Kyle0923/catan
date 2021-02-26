CC = g++
ARTIFACT = catan.exe
SRC_DIR = src
SRC = $(SRC_DIR)/main.cpp
OBJ_DIR = bin
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(ARTIFACT)

$(ARTIFACT): $(OBJ) | $(BIN_DIR)
	$(CC) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c -o $@ $<

$(BIN_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o $(ARTIFACT)
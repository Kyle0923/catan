CC := g++
ARTIFACT := catan.exe
SRC_DIR := src
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_DIR := bin
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
INC := -Iinclude -Ithird_party/pdcurses/include
LIB := -Lthird_party/pdcurses/lib -lpdcurses
CPPFLAGS := $(INC) -MMD -MP
CFLAGS   := -Wall -std=c++11 -g
DEPS := $(OBJ:.o=.d)

all: $(ARTIFACT)

$(ARTIFACT): $(OBJ)
	$(CC) $^ $(LIB) $(CFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MF $(patsubst %.o,%.d,$@) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d $(ARTIFACT)

include $(DEPS)
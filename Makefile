CC := g++
LINT_REPORT := lint_report.txt
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

all: $(LINT_REPORT) $(ARTIFACT)

$(ARTIFACT): $(OBJ)
	$(CC) $^ $(LIB) $(CFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MF $(patsubst %.o,%.d,$@) -c $< -o $@

.PHONY: $(LINT_REPORT)
$(LINT_REPORT):
	rm -f $(LINT_REPORT)
	./lint.sh > $(LINT_REPORT) || (echo -e "\nLint failed\nSee $(LINT_REPORT)\n" && exit 1)

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d $(ARTIFACT)

-include $(DEPS)
CC := g++
ARTIFACT := catan.exe
SRC_DIR := src
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_DIR := bin
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

THIRD_PARTY_DIR := third_party
INC := -Iinclude -I$(THIRD_PARTY_DIR)/pdcurses-3.9/include
LIB := -L$(THIRD_PARTY_DIR)/pdcurses-3.9/lib -l:pdcurses.a
CPPFLAGS := $(INC) -MMD -MP
CFLAGS   := -Wall -std=c++11
DEPS := $(OBJ:.o=.d)


# use `RELEASE=1 make -j8` to build release
ifneq ($(RELEASE),)
ARTIFACT := $(ARTIFACT:%.exe=%_release.exe)
CFLAGS += -DRELEASE -O3
else
CFLAGS += -g
endif

all: lint $(THIRD_PARTY_DIR) $(ARTIFACT)
.PHONY: all lint $(THIRD_PARTY_DIR) clean

$(ARTIFACT): $(OBJ) $(THIRD_PARTY_DIR)
	$(CC) $(OBJ) $(LIB) $(CFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MF $(patsubst %.o,%.d,$@) -c $< -o $@

$(THIRD_PARTY_DIR):
	make -C $(THIRD_PARTY_DIR)

lint:
	bash ./lint.sh

clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d $(ARTIFACT)
	make -C $(THIRD_PARTY_DIR) clean

-include $(DEPS)
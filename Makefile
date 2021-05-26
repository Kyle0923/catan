#
# Makefile for Catan.exe
# automatically compiles all C++ source files under src/
# and recursively makes third party libraries
#
# add library path and name of the third party lib to $THIRD_PARTY_LIB to add to build
# e.g., THIRD_PARTY_LIB += $(THIRD_PARTY_DIR_ROOT)/foo/lib/libfoo.a
# the library binary and the header files must be under lib/ and include/ of the library directory respectively
# and the library name must be in the form of lib<name>.a or lib<name>.so (gcc ld -l format)
#

CXX := g++
ARTIFACT := catan.exe
SRC_DIR_BASE := src
SRC_DIR := $(SRC_DIR_BASE) \
	$(SRC_DIR_BASE)/commands \

SRC := $(wildcard $(SRC_DIR:%=%/*.cpp))

BIN_DIR_BASE := bin
ifneq ($(RELEASE),)
BIN_DIR := $(BIN_DIR_BASE)/release
else
BIN_DIR := $(BIN_DIR_BASE)/debug
endif

OBJ := $(SRC:$(SRC_DIR_BASE)/%.cpp=$(BIN_DIR)/%.o)
# OBJ := $(patsubst %.cpp,$(BIN_DIR)/%.o,$(notdir $(SRC)))
INC := -Iinclude
LIB :=

# third_party related
THIRD_PARTY_DIR_ROOT := third_party

# add the path to the libraries to add to build
THIRD_PARTY_LIB := $(THIRD_PARTY_DIR_ROOT)/pdcurses-3.9/lib/libpdcurses.a

THIRD_PARTY_LIB_DIR := $(patsubst %/lib/,%, $(dir $(THIRD_PARTY_LIB)))

INC += $(THIRD_PARTY_LIB_DIR:%=-I%/include)

LIB += $(THIRD_PARTY_LIB_DIR:%=-L%/lib) \
	$(patsubst lib%,-l%,$(basename $(notdir $(THIRD_PARTY_LIB)))) \

CPPFLAGS := $(INC) -MMD -MP
CFLAGS   := -std=c++14 -Wall -Werror
DEPS := $(OBJ:.o=.d)

# make up clean targets for third party libraries
CLEAN_THIRD_PARTY := $(addprefix CLEAN.,$(THIRD_PARTY_LIB_DIR))

# use `RELEASE=1 make` to build release
ifneq ($(RELEASE),)
ARTIFACT := $(ARTIFACT:.exe=_release.exe)
CFLAGS += -DRELEASE -O2
else
CFLAGS += -g
endif

all: lint $(THIRD_PARTY_LIB_DIR) $(ARTIFACT)
.PHONY: all lint clean clean_all $(THIRD_PARTY_LIB_DIR) $(CLEAN_THIRD_PARTY)

$(ARTIFACT): $(OBJ) $(THIRD_PARTY_LIB)
	$(CXX) $(OBJ) $(LIB) $(CFLAGS) -o $@
ifneq ($(RELEASE),)
	@echo -e "\nBuilding for RELEASE completed: $(ARTIFACT)"
endif

$(BIN_DIR)/%.o: $(SRC_DIR_BASE)/%.cpp | $(BIN_DIR) $(THIRD_PARTY_LIB_DIR)
	$(CXX) $(CPPFLAGS) $(CFLAGS) -MF $(patsubst %.o,%.d,$@) -c $< -o $@

$(THIRD_PARTY_LIB): $(THIRD_PARTY_LIB_DIR)
$(THIRD_PARTY_LIB_DIR):
	$(MAKE) -C $@

$(BIN_DIR):
	mkdir -p $(patsubst $(SRC_DIR_BASE)%,$(BIN_DIR)%,$(SRC_DIR))

lint:
	bash ./lint.sh

clean:
	rm -fr $(BIN_DIR_BASE)
	rm -f $(ARTIFACT) $(ARTIFACT:.exe=_release.exe)

clean_all: clean $(CLEAN_THIRD_PARTY)

$(CLEAN_THIRD_PARTY): CLEAN.%:
	$(MAKE) -C $* clean

-include $(DEPS)

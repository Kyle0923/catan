
LIB_DIRS := pdcurses-3.9

# phony targets to clean all subdirectories, using recipe - CLEAN_TARGET.%
CLEAN_ALL := $(addprefix CLEAN_TARGET.,$(LIB_DIRS))

.PHONY: libs $(LIB_DIRS) clean

libs: $(LIB_DIRS)

$(LIB_DIRS):
	make -C $@

clean: $(CLEAN_ALL)

CLEAN_TARGET.%:
	make -C $* clean

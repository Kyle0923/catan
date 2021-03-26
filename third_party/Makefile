#
# recursive make subdirectories
# add directory name to $LIB_DIRS to add to build
#

LIB_DIRS := pdcurses-3.9

# phony targets to clean all subdirectories, using recipe - CLEAN_TARGET.%
CLEAN_ALL := $(addprefix CLEAN_TARGET.,$(LIB_DIRS))

.PHONY: libs $(LIB_DIRS) clean

libs: $(LIB_DIRS)

$(LIB_DIRS):
	$(MAKE) -C $@

clean: $(CLEAN_ALL)

CLEAN_TARGET.%:
	$(MAKE) -C $* clean

# Get the absolute path of this Makefile, resolving symbolic links
MAKEFILE_PATH := $(realpath $(lastword $(MAKEFILE_LIST)))

# Get the directory of the Makefile
MAKEFILE_DIR := $(dir $(MAKEFILE_PATH))

.PHONY: show-path
show-path:
	@echo "Makefile Path: $(MAKEFILE_PATH)"
	@echo "Makefile Directory: $(MAKEFILE_DIR)"

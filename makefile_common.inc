##############################################################################
#
#
# Here follow the variables to define
#
#
##############################################################################


THIS_MAKEFILE_FILENAME := $(firstword $(MAKEFILE_LIST)) 
#ROOT_PATH       :=  $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
THIS_MAKEFILE_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
# default shell in makefile is sh, which is not sufficient for me
SHELL          := /bin/bash

.PHONY: prompt_me download_file clone-or-pull sparse-checkout



##############################################################################
#
#
# Here follow the target definitions
#
#
##############################################################################

prompt_me:
	@echo -n "Are you sure? [y/N] " && read ans && [ $${ans:-N} = y ]

# Download file if it doesnt exist or is different
#
#
# Define the target file and download URL
#
# DOWNLOAD_URL := http://example.com/path/to/your_file.txt
# TARGET_FILE := your_file.txt
# Download file if it doesnt exist or is different
download_file:
	@mkdir -p $(dir $(TARGET_FILE)); \
	if [ ! -f "$(TARGET_FILE)" ]; then \
		echo "File $(TARGET_FILE) does not exist. Downloading..."; \
		curl -o "$(TARGET_FILE)" "$(DOWNLOAD_URL)"; \
	else \
		echo "File $(TARGET_FILE) already exists. Checking for updates..."; \
		TEMP_FILE=$$(mktemp); \
		curl -o "$$TEMP_FILE" "$(DOWNLOAD_URL)"; \
		if ! cmp -s "$(TARGET_FILE)" "$$TEMP_FILE"; then \
			echo "File is different. Replacing $(TARGET_FILE)..."; \
			mv "$$TEMP_FILE" "$(TARGET_FILE)"; \
		else \
			echo "File is up to date. Removing temporary file."; \
			rm "$$TEMP_FILE"; \
		fi; \
	fi

# clone or pull repo, depending if its already exists or not
# GIT_REPO_URL := https://github.com/example/myrepo.git
# BASE_DIR := /path/to/dir
clone-or-pull:
	@repo_name=$$(basename "$(GIT_REPO_URL)" .git); \
	repo_dir="$(BASE_DIR)/$$repo_name"; \
	if [ -d "$$repo_dir" ]; then \
		echo "Repository '$$repo_name' already exists. Pulling latest changes..."; \
		cd "$$repo_dir" && git pull && cd - > /dev/null; \
	else \
		echo "Cloning repository '$$repo_name' into '$(BASE_DIR)'..."; \
		git clone "$(GIT_REPO_URL)" "$$repo_dir"; \
	fi


# Sparse Checkout: This feature allows you to only check out a specific part of the repository
# like a single directory.
# SPARSE_DIR ?= path/to/specific/directory
# REPO_URL ?= https://github.com/username/repository.git
# BRANCH ?= main
# TARGET_DIR ?= $(notdir $(SPARSE_DIR))
#
# example: make sparse-checkout SPARSE_DIR=path/to/your/dir REPO_URL=https://github.com/username/repo.git BRANCH=main
#
.PHONY: sparse-checkout
sparse-checkout:
	@echo "Cloning $(SPARSE_DIR) from $(REPO_URL) into $(TARGET_DIR)..."
	git init $(TARGET_DIR)
	cd $(TARGET_DIR) && git config core.sparseCheckout true
	echo "$(SPARSE_DIR)" > $(TARGET_DIR)/.git/info/sparse-checkout
	cd $(TARGET_DIR) && git remote add origin $(REPO_URL)
	cd $(TARGET_DIR) && git pull origin $(BRANCH)
	@echo "Directory $(SPARSE_DIR) cloned successfully to $(TARGET_DIR)."


# GIT_REPO_URL := https://github.com/example/myrepo.git
# BASE_DIR := /path/to/dir


THIS_FILENAME := $(firstword $(MAKEFILE_LIST)) 
ROOT_PATH       :=  $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
# default shell in makefile is sh, which is not sufficient for me
SHELL          := /bin/bash

.PHONY: clone-or-pull


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

prompt_me:
	@echo -n "Are you sure? [y/N] " && read ans && [ $${ans:-N} = y ]


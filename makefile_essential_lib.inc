# v.0.0001

# default Makefile behaviour
# example usage:
# 
# 
# maintained by: oliverpelz@gmail.com

.PHONY:  help
.DEFAULT_GOAL  := help
SHELL          := /bin/bash
FORCE:

RED        = $(shell tput -Txterm setaf 1)
GREEN      = $(shell tput -Txterm setaf 2)
DARKGREEN  = $(shell tput -Txterm setaf 6)
YELLOW     = $(shell tput -Txterm setaf 3)
WHITE      = $(shell tput -Txterm setaf 7)
BLUE       = $(shell tput -Txterm setaf 4)
CYAN       = \033[1;49;36m
GREY       = $(shell tput -Txterm setaf 243)
RESET      = $(shell tput -Txterm sgr0)
INFO       = $(GREEN)[INFO]$(RESET)
reverse    = $(if $(1),$(call reverse,$(wordlist 2,$(words $(1)),$(1)))) $(firstword $(1))

rootPath       :=  $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

# default shell in makefile is sh, which is not sufficient for me
SHELL := bash

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

###################################################################################
## Makefile management
update_make_includes:   ## update all makefile include libs from external sources
	$(MAKE) download_file
###################################################################################
## help
help: ##### show this help
	@echo ''
	@echo 'Usage:'
	@echo '  ${YELLOW}make${RESET} ${GREEN}<target>${RESET}'
	@echo ''
	@echo 'Targets:'
	@awk 'BEGIN {FS = ":.*?## "} { \
		if (/^[a-zA-Z_0-9\.-]+:.*?[^#]##[^#].*$$/) {printf "      ${RED}%-30s${RESET}${GREEN}%s${RESET}\n", $$1, $$2} \
		else if (/^[a-zA-Z0-9_\.-]+:.*?[^#]###[^#].*$$/) {printf "      ${YELLOW}%-30s${WHITE}%s${RESET}\n", $$1, $$2} \
		else if (/^[a-zA-Z0-9_\.-]+:.*?[^#]####[^#].*$$/) {printf "      ${CYAN}%-30s${WHITE}%s${RESET}\n", $$1, $$2} \
		else if (/^[a-zA-Z0-9_\.-]+:.*?[^#]#####[^#].*$$/) {printf "      ${GREY}%-30s${WHITE}%s${RESET}\n", $$1, $$2} \
		else if (/^## .*:.*$$/) {printf "  ${DARKGREEN}%s${RESET}\n", substr($$1,4)} \
		}' $(call reverse, $(MAKEFILE_LIST))

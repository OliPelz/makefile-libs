# v.0.0001

# default Makefile behaviour
# example usage:
# 
# 
# maintained by: oliverpelz@gmail.com


prompt_me:
	@echo -n "Are you sure? [y/N] " && read ans && [ $${ans:-N} = y ]

rootPath       :=  $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

# default shell in makefile is sh, which is not sufficient for me
SHELL := bash

.DEFAULT_GOAL  := help
SHELL          := /bin/bash
.PHONY:  help
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

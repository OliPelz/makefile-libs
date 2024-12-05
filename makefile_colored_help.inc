# v.0.0001

# default Makefile behaviour
# example usage:
# 
# 
# maintained by: oliverpelz@gmail.com

.PHONY:  help
.DEFAULT_GOAL  := help
THIS_MAKEFILE_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

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


include $(THIS_MAKEFILE_DIR)/makefile_common.inc

##########################################################################################
## help:
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

##########################################################################################
## Makefile management:
list-targets:
	@echo "Available targets:"; \
	make -pRrq -f $(MAKEFILE_LIST) : 2>/dev/null | \
	awk -F: '/^[a-zA-Z0-9][^$#\/\t=]*:/ {print $$1}' | \
	sort | uniq | grep -v '^$' | grep -v '^makefile$$'

update-make-includes:  ## update all makefile include libs from external sources
	$(MAKE) git-clone-or-pull GIT_REPO_URL=https://github.com/OliPelz/makefile-libs.git TARGET_DIR=makefiles_inc TO_GITIGNORE=true CLEAN=true

THIS_MAKEFILE_FILENAME := $(firstword $(MAKEFILE_LIST)) 
#ROOT_PATH       :=  $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
THIS_MAKEFILE_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
# default shell in makefile is sh, which is not sufficient for me
SHELL          := /bin/bash

.PHONY: prompt_me download_file git-clone-or-pull git-sparse-checkout

##############################################################################
#
#
# targets specific for python
#
#
##############################################################################

# create a new empty poetry project in the current dir and activate it
# Note: we use pip to locally install the poetry cli in the dir
#       Then to activate poetry in other targets just use "poetry shell && ...."

.PHONY: create-new-poetry-project init-existing-poetry-project

# Create a new Poetry project if it doesnt already exist
create-new-poetry-project:
	@if [ -f "pyproject.toml" ]; then \
		echo "Poetry project already exists. Skipping initialization."; \
	else \
		echo "Initializing new Poetry project..."; \
		python3 -m venv .venv && \
		source .venv/bin/activate && \
		pip install --upgrade pip && \
		pip install poetry && \
		poetry init --no-interaction && \
		poetry config virtualenvs.in-project true && \
		poetry install --no-root && \
		echo ".venv" >> .gitignore && \
		echo "Poetry project initialized successfully."; \
	fi
	@echo "To activate the virtual environment, run: 'poetry shell'"

# Initialize Poetry for an existing project with pyproject.toml but missing venv and Poetry CLI
init-poetry-for-project:
	@if [ ! -f "pyproject.toml" ]; then \
		echo "Error: pyproject.toml not found. Please run create-new-poetry-project first."; \
		exit 1; \
	else \
		echo "Initializing Poetry environment for existing project..."; \
		python3 -m venv .venv && \
		source .venv/bin/activate && \
		pip install --upgrade pip && \
		pip install poetry && \
		poetry config virtualenvs.in-project true && \
		poetry install --no-root && \
		echo ".venv" >> .gitignore && \
		echo "Poetry environment initialized successfully."; \
	fi
	@echo "To activate the virtual environment, run: 'poetry shell'"

clean-poetry-venv:
	rm -rf .venv



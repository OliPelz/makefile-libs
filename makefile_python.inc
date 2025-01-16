THIS_MAKEFILE_FILENAME := $(firstword $(MAKEFILE_LIST)) 
#ROOT_PATH       :=  $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
THIS_MAKEFILE_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
# default shell in makefile is sh, which is not sufficient for me
SHELL          := /bin/bash

.PHONY: check-pipx-setup check-poetry-installed create-new-poetry-project init-poetry-for-project

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

# Check if pipx is installed and its path is set
check-pipx-setup:
	@if ! command -v pipx > /dev/null; then \
		echo "Error: pipx is not installed. Please install pipx first."; \
		exit 1; \
	fi
	@if ! pipx list > /dev/null 2>&1; then \
		echo "Error: pipx path is not set. Ensure pipx is in your PATH."; \
		echo "Try adding the following line to your shell profile (e.g., ~/.bashrc):"; \
		echo "export PATH=\$$HOME/.local/bin:\$$PATH"; \
		exit 1; \
	fi
	@echo "pipx is installed and the path is correctly set."

# Check if Poetry is installed via pipx
check-poetry-installed: check-pipx-setup
	@if ! pipx list  2>/dev/null | grep -q 'poetry'; then \
		echo "Error: Poetry is not installed via pipx. Installing Poetry..."; \
		pipx install poetry; \
	else \
		echo "Poetry is already installed via pipx."; \
	fi

# Create a new Poetry project if it doesn't already exist
create-new-poetry-project: check-poetry-installed
	@if [ -f "pyproject.toml" ]; then \
		echo "Poetry project already exists. Skipping initialization."; \
	else \
		echo "Initializing new Poetry project..."; \
		poetry init --no-interaction; \
		poetry config virtualenvs.in-project true; \
		poetry install --no-root; \
		echo ".venv" >> .gitignore; \
		echo "Poetry project initialized successfully."; \
	fi

# Initialize Poetry for an existing project with pyproject.toml
init-poetry-for-project: check-poetry-installed
	@if [ ! -f "pyproject.toml" ]; then \
		echo "Error: pyproject.toml not found. Please run create-new-poetry-project first."; \
		exit 1; \
	else \
		echo "Initializing Poetry environment for existing project..."; \
		poetry config virtualenvs.in-project true; \
		poetry install --no-root; \
		echo ".venv" >> .gitignore; \
		echo "Poetry environment initialized successfully."; \
	fi

poetry-info:
	$(info PATH=$(PATH))
	$(info Which Python: $(shell which python))
	$(info Poetry Path: $(shell which poetry))
        $(info poetry run python: $(shell poetry run python --version))
        $(info poetry env info: $(poetry env info))
        $(info poetry config: $(poetry config --list))

poetry-info-2:
	@echo "PATH=$(PATH)"
	@echo "Which Python: $(shell which python)"
	@echo "Poetry Path: $(shell which poetry)"
	@echo "poetry run python version: $$(poetry run python --version)"
	@echo "poetry env info: $$(poetry env info)"
	@echo "poetry config: $$(poetry config --list)"

##############################################################################
#
#
# targets specific for pyenv
#
#
##############################################################################


.PHONY: check-pyenv python-versions installed-versions current-version pyenv-versions install-python set-python-local set-python-global set-python-project

# Check if pyenv is installed
check-pyenv:
	@if command -v pyenv > /dev/null; then \
		echo "✅ pyenv is installed"; \
	else \
		echo "❌ pyenv is not installed. Please install it from https://github.com/pyenv/pyenv"; \
	fi

# Print available Python versions
python-versions:
	@pyenv install --list

# Print installed Python versions
installed-versions:
	@pyenv versions

# Print the current Python version in use
current-version:
	@python --version

# Print the local, global, and shell (project) Python versions
pyenv-versions:
	@echo "Global Python version:"; pyenv global; \
	echo "Local Python version:"; pyenv local; \
	echo "Shell (project) Python version:"; pyenv shell

# Install a new Python version (requires PYTHON_VERSION)
install-python:
	@if [ -z "$(PYTHON_VERSION)" ]; then \
		echo "❌ Please provide a PYTHON_VERSION. Usage: make install-python PYTHON_VERSION=3.x.x"; \
	else \
		pyenv install $(PYTHON_VERSION); \
	fi

# Set a Python version locally (requires PYTHON_VERSION)
set-python-local:
	@if [ -z "$(PYTHON_VERSION)" ]; then \
		echo "❌ Please provide a PYTHON_VERSION. Usage: make set-python-local PYTHON_VERSION=3.x.x"; \
	else \
		pyenv local $(PYTHON_VERSION); \
		echo "✅ Local Python version set to $(PYTHON_VERSION)"; \
	fi

# Set a Python version globally (requires PYTHON_VERSION)
set-python-global:
	@if [ -z "$(PYTHON_VERSION)" ]; then \
		echo "❌ Please provide a PYTHON_VERSION. Usage: make set-python-global PYTHON_VERSION=3.x.x"; \
	else \
		pyenv global $(PYTHON_VERSION); \
		echo "✅ Global Python version set to $(PYTHON_VERSION)"; \
	fi

# Set a Python version for the current shell/project (requires PYTHON_VERSION)
set-python-project:
	@if [ -z "$(PYTHON_VERSION)" ]; then \
		echo "❌ Please provide a PYTHON_VERSION. Usage: make set-python-project PYTHON_VERSION=3.x.x"; \
	else \
		pyenv shell $(PYTHON_VERSION); \
		echo "✅ Shell Python version set to $(PYTHON_VERSION)"; \
	fi

# Initialize pyenv in .bashrc or .zshrc
init-pyenv:
	@SHELL_CONFIG="$${HOME}/.bashrc"; \
	if [ -n "$${ZSH_VERSION}" ]; then \
		SHELL_CONFIG="$${HOME}/.zshrc"; \
	fi; \
	INIT_CMD="export PYENV_ROOT=\\\"$${HOME}/.pyenv\\\" && export PATH=\\\"$${PYENV_ROOT}/bin:$${PATH}\\\" && eval \\\"\\$$(pyenv init --path)\\\""; \
	if ! grep -q "$${INIT_CMD}" "$${SHELL_CONFIG}"; then \
		echo "Initializing pyenv in $${SHELL_CONFIG}..."; \
		echo "" >> "$${SHELL_CONFIG}"; \
		echo "# Pyenv initialization" >> "$${SHELL_CONFIG}"; \
		echo "$${INIT_CMD}" >> "$${SHELL_CONFIG}"; \
		echo "✅ pyenv initialization added to $${SHELL_CONFIG}."; \
	else \
		echo "✅ pyenv is already initialized in $${SHELL_CONFIG}."; \
	fi

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

.PHONY: prompt_me download_file git-clone-or-pull git-sparse-checkout



##############################################################################
#
#
# Here follow the target definitions
#
#
##############################################################################

prompt_me:
	@echo -n "Are you sure? [y/N] " && read ans && [ $${ans:-N} = y ]

# General target to check if a variable is defined
# use like:
# $(MAKE) check-variable VAR=DJANGO_NEW_PROJECT_NAME
check-variable:
	@if [ -z "$($(VAR))" ]; then \
		echo "Error: Variable '$(VAR)' is not set."; \
		exit 1; \
	fi

# Download file if it doesnt exist or is different
#
#
# Define the target file and download URL
#
# DOWNLOAD_URL := http://example.com/path/to/your_file.txt
# TARGET_FILE := your_file.txt
# Download file if it doesnt exist or is different
# example: $(MAKE) download_file TARGET_FILE=/tmp/download.txt  DOWNLOAD_URL=https://xxx.com/download.txt
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

# git clone or pull repo, depending if it already exists or not
# GIT_REPO_URL := https://github.com/example/myrepo.git
# TARGET_DIR := ./path/to/dir # must be relative to THIS dir
# TO_GITIGNORE (optional) if you want to put the cloned dir into parent's .gitignore file
# CLEAN (optional) if set to "true", cleans up the directory before pulling
# CHECKOUT_REF (optional) allows passing a branch or tag name to check out, if not set defaults to master/main branch

# example: $(MAKE) git-clone-or-pull GIT_REPO_URL=https://github.com/OliPelz/makefile-libs.git TARGET_DIR=makefiles_inc
# to enable with gitignore: $(MAKE) git-clone-or-pull GIT_REPO_URL=https://github.com/OliPelz/makefile-libs.git TARGET_DIR=makefiles_inc TO_GITIGNORE=true
# to enable with cleanup: $(MAKE) git-clone-or-pull GIT_REPO_URL=https://github.com/OliPelz/makefile-libs.git TARGET_DIR=makefiles_inc CLEAN=true

# remove trailing ./ from dir
CLEANED_TARGET_DIR := $(patsubst ./%,%,$(TARGET_DIR))

git-clone-or-pull:
	@org_repo_name=$$(basename "$(GIT_REPO_URL)" .git); \
	repo_dir=$${TARGET_DIR:-$$org_repo_name}; \
	enable_gitignore=$${TO_GITIGNORE:-false}; \
	clean_up=$${CLEAN:-false}; \
	checkout_ref=$${CHECKOUT_REF:-}; \
	if [ -d "$$repo_dir" ]; then \
		echo "Repository '$$org_repo_name' already exists."; \
		if [ "$$clean_up" == "true" ]; then \
			echo "Cleaning up repository directory before pulling..."; \
			(cd "$$repo_dir" && git clean -fd && git reset --hard); \
		fi; \
		echo "Pulling latest changes..."; \
		(cd "$$repo_dir" && git fetch --all --tags && git pull || true); \
	else \
		echo "Cloning repository '$$org_repo_name' into '$(TARGET_DIR)'..."; \
		git clone "$(GIT_REPO_URL)" "$$repo_dir"; \
	fi; \
	if [ -n "$$checkout_ref" ]; then \
		echo "Checking out reference '$$checkout_ref'..."; \
		(cd "$$repo_dir" && git fetch --all --tags && \
		 if git show-ref --heads | grep -q "refs/heads/$$checkout_ref"; then \
			echo "Switching to branch '$$checkout_ref'..."; \
			git switch "$$checkout_ref"; \
		 elif git show-ref --tags | grep -q "refs/tags/$$checkout_ref"; then \
			echo "Checking out tag '$$checkout_ref' as detached HEAD..."; \
			git checkout "$$checkout_ref"; \
		 else \
			echo "Branch or tag '$$checkout_ref' not found locally. Fetching remote branch..."; \
			if git ls-remote --heads origin "$$checkout_ref" | grep -q "$$checkout_ref"; then \
				echo "Found remote branch '$$checkout_ref'. Creating local branch..."; \
				git checkout -b "$$checkout_ref" origin/$$checkout_ref; \
			else \
				echo "Error: Reference '$$checkout_ref' not found."; \
				exit 1; \
			fi; \
		 fi); \
	else \
		echo "No specific branch or tag provided. Checking out 'main' or 'master'..."; \
		(cd "$$repo_dir" && git fetch --all && \
		 (git rev-parse --verify main >/dev/null 2>&1 && git checkout main || git checkout master)); \
	fi; \
	if [ "$$enable_gitignore" == "true" ]; then \
		[ -f .gitignore ] && grep -q '^$(CLEANED_TARGET_DIR)$$' .gitignore || echo '$(CLEANED_TARGET_DIR)' >> .gitignore; \
	fi;


# Sparse Checkout: This feature allows you to only check out a specific part of the repository
# like a single directory.
# SPARSE_DIR ?= path/to/specific/directory
# REPO_URL ?= https://github.com/username/repository.git
# BRANCH ?= main
# TARGET_DIR ?= $(notdir $(SPARSE_DIR))
#
# example: make sparse-checkout SPARSE_DIR=path/to/your/dir REPO_URL=https://github.com/username/repo.git BRANCH=main
#
git-sparse-checkout:
	@echo "Cloning $(SPARSE_DIR) from $(REPO_URL) into $(TARGET_DIR)..."
	git init $(TARGET_DIR)
	cd $(TARGET_DIR) && git config core.sparseCheckout true
	echo "$(SPARSE_DIR)" > $(TARGET_DIR)/.git/info/sparse-checkout
	cd $(TARGET_DIR) && git remote add origin $(REPO_URL)
	cd $(TARGET_DIR) && git pull origin $(BRANCH)
	@echo "Directory $(SPARSE_DIR) cloned successfully to $(TARGET_DIR)."

# add a dir called git_hooks to put our git hooks under version control
git-add-hooks:
	@mkdir -p ./git_hooks && git config core.hooksPath git_hooks


##############################################################################
#
#
# targets specific for git submodules
#
#
##############################################################################


# Ensure all git submodules are initialized and up-to-date with their latest commits from the remote
git-update-all-submodules-changes:
	@git submodule sync --recursive;                             # Sync URLs in case they changed in .gitmodules
	@git submodule update --init --recursive;                    # Initialize if not done, update all nested submodules
	@git submodule foreach --recursive 'git fetch --all';        # Fetch changes in each submodule
	@git submodule update --recursive --remote;                  # Pull the latest changes from each submodule's remote

# If you want to completely remove the submodule checkouts but keep the submodule entries in .gitmodules:
git-remove-all-submodules:
	@git submodule deinit --all --force

# ignore all changes to submodules in parent git
git-ignore-all-submodule-changes:
	@git submodule foreach 'git config submodule.$sm_path.ignore all'


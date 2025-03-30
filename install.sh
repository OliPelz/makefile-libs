#!/usr/bin/env bash

set -e

# script which installs makefile-libs using a curl pipe
# e.g.:
#
# curl -sL https://raw.githubusercontent.com/OliPelz/makefile-libs/refs/heads/main/install.sh | bash
#
# curl -sL https://raw.githubusercontent.com/OliPelz/makefile-libs/refs/heads/main/install.sh | bash -s -- -d /opt/myapp
#
#

if ! command -v git 2>&1 >/dev/null
then
    echo "mandantory command 'git' could not be found"
    exit 1
fi

INSTALL_DIR=$(pwd)
GIT_REPO_URL="https://github.com/OliPelz/makefile-libs.git"

# Parse command-line options
while getopts "d:" opt; do
  case ${opt} in
    d ) INSTALL_DIR=$OPTARG ;;
    * ) echo "Usage: curl -sL <url> | bash -s -- [-d directory]" ; exit 1 ;;
  esac
done

FINAL_INSTALL_DIR=$INSTALL_DIR/.makefile-libs

# Create the install directory if it doesn’t exist
mkdir -p "$FINAL_INSTALL_DIR"

echo "🔹 Installing makefile-libs in: $FINAL_INSTALL_DIR"

echo "🔹 Git cloning 'makefile-libs'..."
if [ -d "$FINAL_INSTALL_DIR/.git" ]; then
    echo "Repository in '$FINAL_INSTALL_DIR' already exists.";
    echo "Pulling latest changes...";
    (cd "$FINAL_INSTALL_DIR" && git fetch --all --tags && git pull || true);
else
    echo "Cloning repository 'makefile-libs' into '$FINAL_INSTALL_DIR'...";
    git clone "$GIT_REPO_URL" "$FINAL_INSTALL_DIR";
fi;

echo "finishing installation..."

echo "🔹 adding makefile-libs to .gitignore file"
grep ".makefile-libs" $INSTALL_DIR/.gitignore || \
     echo ".makefile-libs" >> $INSTALL_DIR/.gitignore

echo "🔹 creating link to Makefile"
test -L $INSTALL_DIR/Makefile || ln -s $FINAL_INSTALL_DIR/Makefile $INSTALL_DIR/Makefile 

echo "✅ Installation complete! Run with:"



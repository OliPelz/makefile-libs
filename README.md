# Makefile libraries

nice helper functions and cool extra feature libs for your Makefiles
like adding colors, git helpers or crypting functions

for fast use, copy the Makefile template and init script file to your target dir
then run it for the first time to initalize it (never need to run again)

```bash

$ mkdir /your/brand/new/project/target/dir/
$ cd $_
$ curl https://raw.githubusercontent.com/OliPelz/makefile-libs/refs/heads/main/Makefile.init
$ make -f Makefile

```


Whenever you need to update makefile include libraries run:

```bash

$ make update-make-includes 

```

if you need poetry package manager in your new app, init with

```bash

$ make init-poetry-for-project 

```


if you need bash utilty support enable/remove comment the section in Makefile in requirements target around the area

```bash
#$(MAKE) git-clone-or-pull \
#    GIT_REPO_URL=https://github.com/OliPelz/utility-scripts.git \
#	 TARGET_DIR=./external-deps/utility-scripts \
#	 TO_GITIGNORE=true CLEAN=true \
#	 && cd ./external-deps/utility-scripts \
#	 && ./__makefile_init_run_only_once.sh; \
#); \
```

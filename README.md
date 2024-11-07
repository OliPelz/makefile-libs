# Makefile libraries

nice helper functions and cool extra feature libs for your Makefiles
like adding colors, git helpers or crypting functions

for fast use, copy the Makefile template and init script file to your target dir
then run it for the first time to initalize it 

```bash
$ cp Makefile.init __makefile_init_run_only_once.sh /your/brand/new/project/target/dir/
$ cd $_
$ __makefile_init_run_only_once.sh
```

this will download latest makefile-lib and create a Makefile and set it up!!!

long
```bash
$ mkdir /tmp/new-project
$ cp Makefile.init __makefile_init_run_only_once.sh /tmp/new-project/
$ cd /tmp/new-project
$ ./__makefile_init_run_only_once.sh # will prepare makefile for new project (download files)
$ make # now a Makefile is set up
$ make delete.it
```

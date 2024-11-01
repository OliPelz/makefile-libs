# Makefile libraries

nice helper functions and cool extra feature libs for your Makefiles
like adding colors, git helpers or crypting functions

for fast use, copy the Makefile template to your target dir
then run it for the first time without the target to initalize it 
Note: init will only be done for the first time you run it!

```bash
cp Makefile.init /your/target/dir/Makefile.init
cd $_
make -f Makefile.init
```

this will download latest makefile-lib and create a Makefile and set it up!!!


long
```bash
$ mkdir /tmp/new-project
$ cp Makefile.init /tmp/new-project/Makefile.init
$ cd /tmp/new-project
$ make -f Makefile.init # will prepare makefile for new project (download files)
$ make # now a Makefile is set up
$ make delete.it
```

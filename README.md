# Makefile libraries

nice helper functions and cool extra feature libs for your Makefiles
like adding colors, git helpers or crypting functions

for fast use, copy the Makefile template to your target dir
then run it for the first time without the target to initalize it 
Note: init will only be done for the first time you run it!

```bash
cp Makefile.template /your/target/Makefile
cd $_
make
```
afterwards Makefile libs are set up!!!


long
```bash
$ mkdir /tmp/new-project
$ cp Makefile.template /tmp/new-project/Makefile
$ cd /tmp/new-project
$ make # will prepare makefile for new project (download files)
$ make # run again will present menu
$ make delete.it
```

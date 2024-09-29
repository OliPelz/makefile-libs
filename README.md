# Makefile libraries

nice helper functions and cool extra feature libs for your Makefiles
like adding colors, git helpers or crypting functions

to use, include the Makefile lib in your custom Makefile like

```Makefile
include makefile_colored_help.inc
```

to test its usage use

```bash
$ mkdir /tmp/new-project
$ cp Makefile.template /tmp/new-project/Makefile
$ cd /tmp/new-project
$ make # will prepare makefile for new project (download files)
$ make # run again will present menu
$ make delete.it
```

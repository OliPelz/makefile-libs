#!/usr/bin/env bash

make -f Makefile.init
make requirements

mv $0 __makefile_init_DO_NOT_RUN_AGAIN.sh

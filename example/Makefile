#
# Copyright (c) 2022      IBM Corporation.  All rights reserved.
# $COPYRIGHT$
#
# Additional copyrights may follow
#
# $HEADER$
#
PROGS=abi-query-dlopen

all: $(PROGS)

PMIX_ABI_HEADERS=$(PWD)/../

abi-query-dlopen: abi-query-dlopen.c
	gcc abi-query-dlopen.c -O0 -g -ldl -I$(PMIX_ABI_HEADERS) -o abi-query-dlopen

run:
	@echo $(PMIX_ROOT)

clean:
	rm -f $(PROGS) core.*

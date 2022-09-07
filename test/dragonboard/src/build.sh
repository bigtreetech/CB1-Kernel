#!/bin/bash

SRCDIRS = core/ view/ $(shell find testcases/ -type d)
SOURCES = $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
echo "sources =$SOURCE"

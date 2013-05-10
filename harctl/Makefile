# Generated Makefile for "harctl" 

NAME = harctl
VERSION = 1.0
RELEASE = 2

PROJ_DIR=$(shell pwd)
INCLUDES=-I$(PROJ_DIR)/include
CFLAGS?=-c
LDFLAGS=-static

override CFLAGS += $(INCLUDES)

SOURCES= $(wildcard src/harctl/*.c) $(wildcard src/lib/*.c)

OBJECTS=$(SOURCES:.c=.o)

all: $(NAME) 

$(NAME): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(NAME) $(OBJECTS)

.PHONY: clean
clean : 
	@rm -f *.o
	@rm -f $(harctl)
	@echo Directory Cleaned!

rpm:
	tar jcf harctl-${VERSION}-${RELEASE}.tar.bz2 Makefile include src
	sed -e 's/Version:.*/Version:        ${VERSION}/' -i harctl.spec
	sed -e 's/Release:.*/Release:        ${RELEASE}/' -i harctl.spec
	rpmbuild --define "_sourcedir ." --define "_srcrpmdir ." -bs harctl.spec
	rm harctl-${VERSION}-${RELEASE}.tar.bz2

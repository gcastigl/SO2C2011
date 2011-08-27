# Binary Name
BINARY := bin/tp1.out

CFLAGS := -Wall
LDFLAGS := 

# Directories belonging to the project
PROJDIRS := src include
# All source files of the project
CSRCS := $(shell find -L $(PROJDIRS) -type f -name "*.c")
# All header files of the project
CHDRS := $(shell find -L $(PROJDIRS) -type f -name "*.h")

# Binary Directorys
BINDIR := bin

.PHONY: all clean

all: $(BINDIR) $(BINARY)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	-@$(RM) -rf $(BINDIR)

$(BINARY):
	gcc $(CFLAGS) -MMD -MP $(CSRCS) -o $(BINARY) -lpthread -lm

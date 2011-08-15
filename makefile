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

.PHONY: all clean

all: $(OBJDIR) $(BINARY)

$(OBJDIR):
	mkdir -p bin

clean:
	-@$(RM) -rf $(OBJDIRS)

$(BINARY):
	gcc $(CFLAGS) -MMD -MP $(CSRCS) -o $(BINARY)

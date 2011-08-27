# Binary Name
BINARY := bin/tp1.out

CFLAGS := -Wall -g

# Directories belonging to the project
PROJDIRS := src include
# All source files of the project
CSRCS := $(shell find -L $(PROJDIRS) -type f -name "*.c")

# All object files in the library
OBJS := $(patsubst src/%.c,bin/%.o,$(CSRCS))

OBJDIRS := $(sort $(dir $(OBJS)))

.PHONY: all clean

all: $(OBJDIRS) $(BINARY)

$(OBJDIRS):
	mkdir -p $@

$(BINARY): $(OBJS)
	gcc $(CFLAGS) $(OBJS) -o $@

clean:
	-@$(RM) -rf $(OBJDIRS)

$(OBJS): bin/%.o : src/%.c
	gcc $(CFLAGS) -MMD -MP -c $< -o $@

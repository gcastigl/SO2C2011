# Binary Name
BINARY := bin/tp1.out

IPC := msgQueue.c
#IPC := pipe.c

LIBS := -lpthread -lm -lncurses -pthread
CFLAGS := -Wall -g -std=c99 -D_XOPEN_SOURCE -D LOG_TO_FILE
INCLUDE_DIR = include
# Directories belonging to the project
PROJDIRS := src include
# All source files of the project
CSRCS := $(shell find -L $(PROJDIRS) -type f -name "*.c" | grep -v "src/ipcs/") src/ipcs/$(IPC)

# All object files in the library
OBJS := $(patsubst src/%.c,bin/%.o,$(CSRCS))
OBJDIRS := $(sort $(dir $(OBJS)))

# Includes
C_INCLUDE_PATH = include

.PHONY: all clean

all: $(OBJDIRS) $(BINARY)

$(OBJDIRS):
	mkdir -p $@

$(BINARY): $(OBJS)
	gcc $(CFLAGS) $(LIBS) $(OBJS) -o $@

clean:
	-@$(RM) -rf $(OBJDIRS)

$(OBJS): bin/%.o : src/%.c
	gcc $(CFLAGS) $(LIBS) -I $(INCLUDE_DIR) -MMD -MP -c $< -o $@

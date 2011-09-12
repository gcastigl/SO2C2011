# Binary Name
BINARY := bin/tp1.out

#IPC := msgQueue.c
#IPC := fifo.c
IPC := socket.c

LIBS := -lm -lncurses -pthread -lpthread -Iinclude
CFLAGS := -Wall -g -std=c99 -D_XOPEN_SOURCE=600 -DLOG_TO_FILE
# Directories belonging to the project
PROJDIRS := src include
# All source files of the project
CSRCS := $(shell find -L $(PROJDIRS) -type f -name "*.c" | grep -v "src/ipcs/") src/ipcs/$(IPC)
# All object files in the library
OBJS := $(patsubst src/%.c,bin/%.o,$(CSRCS))
OBJDIRS := $(sort $(dir $(OBJS)))

# Includes
C_INCLUDE_PATH = include

.PHONY: all clean debug

all: $(OBJDIRS) $(BINARY)

debug: override CFLAGS += -DDEBUG_MODE
debug: all

$(OBJDIRS):
	mkdir -p $@

$(BINARY): $(OBJS)
	gcc $(CFLAGS) $(LIBS) $(OBJS) -o $@

clean:
	-@$(RM) -rf $(BINARY) $(OBJDIRS) sosocket

$(OBJS): bin/%.o : src/%.c include/%.h
	gcc $(CFLAGS) $(LIBS) -c $< -o $@

%.h:
	#header change checking.

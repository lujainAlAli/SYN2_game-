
# Compiler
CC = gcc

# Executable name
TARGET = main

# Source files
SOURCES = main.c scenes.c helper.c 

# Compiler flags
CFLAGS = -Wall -Wextra -Wno-missing-field-initializers -O2
# Operating system detection
ifeq ($(OS),Windows_NT)
    # Windows: PDCurses
    CFLAGS += -D_WIN32
    LDFLAGS = -lpdcurses
else
    UNAME := $(shell uname)

    ifeq ($(UNAME),Darwin)
        # macOS: ncurses
        LDFLAGS = -lncurses
    else
        # Linux: ncurses
        LDFLAGS = -lncurses
    endif
endif

# Default rule
all: $(TARGET)

# Compile and link
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

# Remove compiled files
clean:
	rm -f $(TARGET) $(TARGET).exe

.PHONY: all clean


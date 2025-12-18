# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g -Iinclude

# Target executable
TARGET = build/temp_pressure_monitor

# Source and object files
SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, build/%.o, $(SRCS))

# Default rule
all: $(TARGET)

# Link object files into final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files inside build/
build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@


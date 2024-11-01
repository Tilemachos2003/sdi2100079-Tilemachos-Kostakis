# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Source files and object files
SRCS = miris.c graph.c hashMap.c operations.c
OBJS = $(SRCS:.c=.o)

# Output executable name
TARGET = miris

# Default target
all: $(TARGET)

# Linking step
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile each .c file into .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the build files
clean:
	rm -f $(OBJS) $(TARGET)

# Rebuild everything
rebuild: clean all
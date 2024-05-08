# Compiler settings - Can change to clang or others
CC = gcc
CFLAGS = -Wall

# Target executable name
TARGET = wunzip
SOURCE = src/wunzip

# Build target
$(TARGET): $(SOURCE).c
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE).c

# Clean up generated files
clean:
	rm -f $(TARGET)
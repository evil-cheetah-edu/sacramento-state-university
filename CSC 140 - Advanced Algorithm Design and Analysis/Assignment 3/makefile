# Compiler settings - Can change to clang or others
CC = g++
CFLAGS = -Wall -O2

# Target executable name
TARGET = sort
SOURCE = src/sort

# Build target
$(TARGET): $(SOURCE).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE).cpp

# Clean up generated files
clean:
	rm -f $(TARGET)
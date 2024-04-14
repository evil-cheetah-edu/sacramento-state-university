# Compiler settings - Can change to clang or others
CC = java
FLAGS = -jar antlr-4.13.1-complete.jar


GRAMMAR_DIR=grammars-v4/json
GRAMMAR_FILE=JSON

# Target executable name
SOURCE_DIR=src

# Build target
all: copy_grammar compile

copy_grammar:
	cp $(GRAMMAR_DIR)/$(GRAMMAR_FILE).g4 $(SOURCE_DIR)


compile:
	$(CC) $(FLAGS) $(SOURCE_DIR)/$(GRAMMAR_FILE).g4


# Clean up generated files
clean:
	rm -f 				   			     \
	$(OUTPUT_DIR)/(GRAMMAR_FILE)*.java   \
	$(OUTPUT_DIR)/(GRAMMAR_FILE)*.tokens \
	$(OUTPUT_DIR)/(GRAMMAR_FILE)*.interp
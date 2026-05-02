CC=gcc
CFLAGS=-lraylib -lm -lpthread -ldl -lrt -lX11

# Finds all include files to be use din the project
include_files=$(wildcard ./include/* ./include/*/* ./src/*)
# Finds all files that the project is dependent on
dep_files=$(wildcard makefile ./include/* ./include/*/* ./src/* ./src/*/*)
# Finds all the source files in the src folder
src_files=$(wildcard ./src/*.c)

main: $(dep_files)
	@echo "Making main"
	gcc -o main -g $(src_files) $(CFLAGS)

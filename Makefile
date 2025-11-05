CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = concat_exec
SRC = concat_interpreter.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

test: $(TARGET)
	./$(TARGET) examples.cat

interactive: $(TARGET)
	./$(TARGET)

help:
	./$(TARGET) --help

.PHONY: all clean test interactive help

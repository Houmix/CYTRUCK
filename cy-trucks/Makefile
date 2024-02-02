# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = progc
SRC_DIR = progc
OBJ_DIR = temp

all: $(TARGET)

$(TARGET): $(OBJ_DIR)/*.o
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

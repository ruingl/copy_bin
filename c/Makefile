CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = src/cJSON.c src/database.c src/main.c
OBJ = $(SRC:.c=.o)
TARGET = program

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean

TARGET = tagger
CC = gcc
CPPFLAGS = -I./src/headers
CFLAGS = -Wall -Wextra

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, %.o, $(SRCS))

.PHONY: clean
.DEFAULT: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o build/$(TARGET)

%.o: src/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

uninstall: clean

clean:
	(rm build/$(TARGET) $(OBJS)) || true
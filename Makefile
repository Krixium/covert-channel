# If DEBUG = 1, then the program will build with debugging symbols.
# If DEBUG = 0, then it will not.
DEBUG ?= 1
CC=gcc
CFLAGS += -Wall -Werror -I$(INC_DIR)
NAME=covert
LINKS=

SRC := forge.c net.c main.c
OBJ := $(SRC:.c=.o)


ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG -ggdb
else
	CFLAGS += -DNDEBUG -O3
endif

SRC_DIR=src
INC_DIR=include

.PHONY: default clean

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LINKS)

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $^

clean:
	rm -f *.o *.log $(NAME) $(DEBUGNAME)
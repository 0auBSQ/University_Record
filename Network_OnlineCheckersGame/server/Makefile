NAME = server
SRC = main.c utils/list.c utils/token.c utils/strsplit.c utils/game.c requests/wrapper.c
INC = includes/specs.h includes/funcs.h
OBJ = $(addsuffix .o,$(basename $(SRC)))
CC = gcc
CFLAGS = -Wall -Wextra -Iincludes -g3
LIBS = -L/usr/lib -lssl -lcrypto

all: $(NAME)

$(NAME): $(OBJ) $(INC)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBS)

%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	/bin/rm -f $(OBJ)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
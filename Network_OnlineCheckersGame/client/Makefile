NAME = client
SRC = main.c strsplit.c
OBJ = $(addsuffix .o,$(basename $(SRC)))
CC = gcc
CFLAGS = -Wall -Wextra

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	/bin/rm -f $(OBJ)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
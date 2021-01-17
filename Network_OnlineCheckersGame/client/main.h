#pragma once

#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>

#define BUF_SIZE 256
#define DEFAULT_PORT 42421

#define DAME_HOST -2
#define HOST -1
#define CLEAR 0
#define GUEST 1
#define DAME_GUEST 2

typedef struct jump_s {
	bool			take;
	bool			illegal;
	int				x;
	int				y;
}				jump_t;

typedef struct piece_info_s {
	bool			legal;
	struct jump_s	prise;
}				piece_info_t;

typedef struct client_s {
	int					s;
	struct sockaddr_in	sin;
	bool				logged;
	bool				spectating;
	bool				hosting;
	char				username[17];
	char				board[10][10];
}				client_t;

size_t			strsplit(char const *s, char ***tab_addr, char c);
void			deltab(void ***matrix, size_t size);


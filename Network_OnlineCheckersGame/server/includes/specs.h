#pragma once

#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/file.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h> 
#include <stdint.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>

#define BUF_SIZE 256
#define MAX_CLIENTS 16
#define DEFAULT_PORT 42421
#define ACCOUNTS_FILE "bdd/accounts"

typedef enum code_e {
	SUCCESS = 200,
	INVALID_TOKEN_COUNT = 400,
	ACCOUNT_ALREADY_EXISTS = 401,
	USER_NOT_PLAYING = 402,
	INVALID_CHARACTERS = 403,
	ACCOUNT_NOT_FOUND = 404,
	UNAUTHORIZED = 405,
	USERNAME_TOO_LONG = 406,
	NOT_YOUR_TURN = 407,
	INVALID_PLACEMENT = 408,
	USERNAME_OR_PASSWORD_TOO_SHORT = 409,
	REQUEST_DOES_NOT_EXIST = 410,
	ALREADY_LOGGED_IN = 411,
	ALREADY_HOSTING = 412,
	MATCH_FULL = 413,
	ALREADY_PLAYING_OR_SPECTATING = 414,
	I_M_A_GNOME_AND_YOU_VE_BEEN_GNOMED = 418,
	INTERNAL_SERVER_ERROR = 500,
	PLAYER_JOINED = 600,
	PIECE_PUT_BY_GUEST = 601,
	PIECE_PUT_BY_HOST = 602,
	CHEATING_ATTEMPT_CATCHED = 603,
	TURN_CHANGED = 604,
	STARTING_RAFLE = 605,
	GAME_ENDED = 606
}				code_t;

typedef enum turn_e {
	HOST = -1,
	GUEST = 1
}				turn_t;

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

typedef struct play_info_s {
	enum code_e			status;
	enum turn_e			role;
	int					oid;
	struct piece_info_s	info;
	char				hostname[17];
}				play_info_t;

typedef enum socket_status_e {
	DISCONNECTED = 0,
	CONNECTED = 1
}				socket_status_t;

typedef struct connect_status_s {
	bool			logged;
	char			username[17];
	unsigned char	session_token[128];
	bool			spectating;
	char			watching[17];
	bool			playing;
	char			host[17];
}				connect_status_t;

typedef struct client_s {
	struct						sockaddr caddr;
	socklen_t					clen;
	int							cs;
	char						buf[BUF_SIZE + 1];
	enum socket_status_e		connected;
	struct connect_status_s		status;
}				client_t;

typedef struct match_s {
	char	host[17];
	char	guest[17];
	bool	guest_found;
	int8_t	board[10][10];
	turn_t	turn;
}				match_t;

typedef struct list_s {
	struct list_s	*next;
	struct match_s	match;
}				list_t;

extern client_t clients[MAX_CLIENTS];
extern list_t	*matchs;
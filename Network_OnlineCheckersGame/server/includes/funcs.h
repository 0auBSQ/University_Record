#pragma once

#include "specs.h"

char			*process_request(char *msg, int cid);

code_t			add_match(list_t **begin_list, char *host, int cid);
code_t			fetch_match(list_t **begin_list, char **response);
code_t			clean_match(list_t **begin_list, char *host);
code_t			join_match(list_t **begin_list, char *host, int cid);
code_t			clear_list(list_t **alst);
code_t			display_matchs_debug(list_t **begin_list);

code_t			spectate(list_t **begin_list, char *host, int cid);
code_t			stop_spectate(int cid);

unsigned char	*generate_token(char *username);
char			*decrypt_token(unsigned char *token);
code_t			login(char *username, char *password, int cid, unsigned char **ret_tok);
code_t			signup(char *username, char *password);

void			deltab(void ***matrix, size_t size);
size_t			strsplit(char const *s, char ***tab_addr, char c);

play_info_t		place_piece(list_t **begin_list, int cid, int xo, int yo, int x, int y);
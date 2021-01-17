#include "funcs.h"

list_t	*create_match(char *host) {
	list_t	*chain;
	if (!(chain = (list_t *)calloc(1, sizeof(list_t))))
		return (NULL);
	chain->next = NULL;
	strncpy(chain->match.host, host, 16);
	bzero(chain->match.guest, 17);
	chain->match.guest_found = false;
	chain->match.turn = (turn_t)HOST;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 10; j++) {
			if ((i + j) % 2 == 1) {
				chain->match.board[i][j] = (turn_t)HOST;
			}
		}
	}
	for (int i = 6; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if ((i + j) % 2 == 1) {
				chain->match.board[i][j] = (turn_t)GUEST;
			}
		}
	}
	return (chain);
}

code_t	add_match(list_t **begin_list, char *host, int cid) {
	list_t	*list;

	if (!begin_list)
		return (INTERNAL_SERVER_ERROR);
	if (clients[cid].status.spectating == true || clients[cid].status.playing == true)
		return (ALREADY_PLAYING_OR_SPECTATING);
	else if (*begin_list) {
		list = *begin_list;
		while (list->next) {
			if (!strcmp(list->match.host, host))
				return (ALREADY_HOSTING);
			list = list->next;
		}
		if (!strcmp(list->match.host, host))
				return (ALREADY_HOSTING);
		if (!(list->next = create_match(host)))
			return (INTERNAL_SERVER_ERROR);
	}
	else if (!(*begin_list) && !(*begin_list = create_match(host)))
		return (INTERNAL_SERVER_ERROR);
	clients[cid].status.playing = true;
	strncpy(clients[cid].status.host, host, 16);
	return (SUCCESS);
}

code_t	join_match(list_t **begin_list, char *host, int cid) {
	list_t	*list;
	
	if (!begin_list)
		return (INTERNAL_SERVER_ERROR);
	if (strlen(host) > 16)
		return (USERNAME_TOO_LONG);
	if (strlen(host) < 3)
		return (USERNAME_OR_PASSWORD_TOO_SHORT);
	if (clients[cid].status.spectating == true || clients[cid].status.playing == true)
		return (ALREADY_PLAYING_OR_SPECTATING);
	if (*begin_list) {
		list = *begin_list;
		while (list) {
			if (!strcmp(list->match.host, host)) {
				if (list->match.guest_found == true)
					return (MATCH_FULL);
				list->match.guest_found = true;
				strcpy(list->match.guest, clients[cid].status.username);
				clients[cid].status.playing = true;
				strncpy(clients[cid].status.host, host, 16);
				return (SUCCESS);
			}
			list = list->next;
		}
	}
	return (USER_NOT_PLAYING);
}

code_t	fetch_match(list_t **begin_list, char **response) {
	list_t	*list;
	uint8_t	i = 5;

	if (!begin_list) {
		sprintf(*response, "%d", INTERNAL_SERVER_ERROR);
		return (INTERNAL_SERVER_ERROR);
	}
	sprintf(*response, "%d", SUCCESS);
	if (*begin_list) {
		list = *begin_list;
		while (list && i > 0) {
			if (list->match.guest_found == false) {
				strcat(*response, "|");
				strcat(*response, list->match.host);
				i--;
			}
			list = list->next;
		}
	}
	return (SUCCESS);
}

code_t	clean_match(list_t **begin_list, char *host) {
	list_t	*list;
	list_t	*prev;
	
	if (!begin_list)
		return (INTERNAL_SERVER_ERROR);
	if (*begin_list) {
		list = *begin_list;
		prev = NULL;
		while (list) {
			if (!strcmp(list->match.host, host)) {
				printf("Deleted match : %s\n", list->match.host);
				if (!prev) {
					list = list->next;
					free(*begin_list);
					*begin_list = list;
					break ;
				}
				else {
					prev->next = list->next;
					free(list);
					break ;
				}
			}
			prev = list;
			list = list->next;
		}
	}
	return (SUCCESS);
}

code_t			spectate(list_t **begin_list, char *host, int cid) {
	list_t	*list;
	
	if (!begin_list)
		return (INTERNAL_SERVER_ERROR);
	if (strlen(host) > 16)
		return (USERNAME_TOO_LONG);
	if (strlen(host) < 3)
		return (USERNAME_OR_PASSWORD_TOO_SHORT);
	if (clients[cid].status.spectating == true || clients[cid].status.playing == true)
		return (ALREADY_PLAYING_OR_SPECTATING);
	if (*begin_list) {
		list = *begin_list;
		while (list) {
			if (!strcmp(list->match.host, host)) {
				if (list->match.guest_found == false)
					return (USER_NOT_PLAYING);
				clients[cid].status.spectating = true;
				strcpy(clients[cid].status.watching, host);
				return (SUCCESS);
			}
			list = list->next;
		}
	}
	return (USER_NOT_PLAYING);
}

code_t			stop_spectate(int cid) {
	clients[cid].status.spectating = false;
	bzero(clients[cid].status.watching, 17);
	return (SUCCESS);
}

code_t	display_matchs_debug(list_t **begin_list) {
	list_t	*list;

	if (!begin_list)
		return (INTERNAL_SERVER_ERROR);
	list = *begin_list;
	while (list) {
		printf("Host : %s, Current turn : ", list->match.host);
		if (list->match.turn == HOST)
			puts("HOST");
		else
			puts("GUEST");
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				printf("%2d ", list->match.board[i][j]);
			}
			puts("");
		}
		list = list->next;
	}
	return (SUCCESS);
}

code_t	clear_list(list_t **alst)
{
	list_t	*next;

	while (*alst)
	{
		next = (*alst)->next;
		free(*alst);
		*alst = next;
	}
	return (SUCCESS);
}

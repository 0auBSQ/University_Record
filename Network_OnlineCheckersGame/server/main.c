#include "funcs.h"

client_t clients[MAX_CLIENTS] = {0};
list_t	*matchs = NULL;

int s;
fd_set active_set;
fd_set rd_set;
ssize_t msg_len;

/*
int main(void) {
	printf("%d\n", add_match(&matchs, "test"));
	printf("%d\n", add_match(&matchs, "me"));
	printf("%d\n", add_match(&matchs, "you"));
	display_matchs_debug(&matchs);
	unsigned char *tok = generate_token("aaaaaaaaaaaaaaaa");
	char *ret = decrypt_token(tok);
	puts(ret);
	free(tok);
	free(ret);
	clear_list(&matchs);
	printf("%d\n", login("test", "pass", 2));
	printf("%s\n", clients[2].status.username);
	printf("%d\n", signup("test", "pass"));
	return (0);
}
*/

int			seek_next_free_client(void) {
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].connected == DISCONNECTED)
			return (i);
	}
	return (-1);
}

int			client_fd_to_int(int fd) {
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].cs == fd)
			return (i);
	}
	return (-1);
}

int			main(void) {
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons((unsigned short)DEFAULT_PORT);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	for (int i = 0; i < 8; i++) {
		sin.sin_zero[i] = 0;
	}

	if (bind(s, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)) < 0) {
		perror("bind");
		exit(1);
	}

	if (listen(s, 64) < 0) {
		perror("listen");
		exit(1);
	}

	FD_ZERO(&active_set);
	FD_SET(s, &active_set);
	
	puts("=== Server launched ===");
	
	while (1) {
		
		rd_set = active_set;
		if (select(FD_SETSIZE, &rd_set, NULL, NULL, NULL) < 0) {
			perror("select");
			exit(1);
		}
	
		for (int j = 0; j < FD_SETSIZE; j++) {
			if (FD_ISSET(j, &rd_set)) {
				printf("FD %d\n", j);
				if (j == s) {
					int next = seek_next_free_client();
					if (next >= 0) {
						if ((clients[next].cs = accept(s, &(clients[next].caddr), &(clients[next].clen))) < 0) {
							perror("accept");
							exit(1);
						}
						FD_SET(clients[next].cs, &active_set);
						clients[next].connected = (socket_status_t)CONNECTED;
						clients[next].status.logged = false;
						clients[next].status.spectating = false;
						clients[next].status.playing = false;
						printf("Client %d connected !\n", next);
					}
				}
				
				else {
					int id = client_fd_to_int(j);
					if (id < 0)
						continue ;
					bzero(clients[id].buf, BUF_SIZE + 1);
					if ((msg_len = recv(clients[id].cs, (void *)clients[id].buf, BUF_SIZE, 0)) >= 0) {
						// Disconnect if recv 0
						if (msg_len == 0) {
							printf("Client %d disconnected (%s)", id, strerror(errno));
							if (clients[id].status.logged == true)
								clean_match(&matchs, clients[id].status.username);
							close(clients[id].cs);
							clients[id].connected = (socket_status_t)DISCONNECTED;
							FD_CLR(clients[id].cs, &active_set);
						}
						// Remove \r or \r\n depending of the system
						if (clients[id].buf[msg_len - 1] == '\n') {
							clients[id].buf[msg_len - 1] = 0;
							msg_len--;
						}
						if (clients[id].buf[msg_len - 1] == '\r') {
							clients[id].buf[msg_len - 1] = 0;
							msg_len--;
						}
						char *response = process_request(&(clients[id].buf[0]), id);
						printf("%s\n", response);
						// Some info can need to be sent to other clients, to update
						printf("client %d => %s [length : %zd]\n", id, clients[id].buf, msg_len);
						if (response)
							send(clients[id].cs, response, BUF_SIZE, 0);
						else
							send(clients[id].cs, "500|La requete a échouée du à un dysfonctionnement du serveur", 64, 0);
						free(response);
					}
				}
			}
		}
	}
}
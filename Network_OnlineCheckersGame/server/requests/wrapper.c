#include "funcs.h"

// If return = NULL : return error 500
char		*process_request(char *msg, int cid) {
	code_t			return_code = SUCCESS;
	char			**tokens = NULL;
	size_t			nb_tokens;
	unsigned char	*extra_info = NULL;
	char			*response;
	
	
	nb_tokens = strsplit(msg, &tokens, '|');
	if (!tokens) {
		return (NULL);
	}
	if (!(response = (char *)calloc(BUF_SIZE, sizeof(char)))) {
		goto end;
	}
	if (nb_tokens < 1) {
		sprintf(response, "%d|Requete Invalide", INVALID_TOKEN_COUNT);
		goto end;
	}
	else if (!strcmp(tokens[0], "CONNECT")) {
		if (nb_tokens != 3) {
			sprintf(response, "%d|Requete Invalide", INVALID_TOKEN_COUNT);
			goto end;
		}
		printf("Client %d requested CONNECT with [%s] [%s]\n", cid, tokens[1], tokens[2]);
		return_code = login(tokens[1], tokens[2], cid, &extra_info);
		if (return_code == SUCCESS)
			sprintf(response, "%d|", return_code);
		else
			sprintf(response, "%d|Une erreur est survenue", return_code);
		if (return_code == SUCCESS && extra_info)
			memcpy((void *)(response + 4), (void *)extra_info, 128 * sizeof(unsigned char));
	}
	else if (!strcmp(tokens[0], "CREATE")) {
		if (nb_tokens != 3) {
			sprintf(response, "%d|Requete Invalide", INVALID_TOKEN_COUNT);
			goto end;
		}
		printf("Client %d requested CREATE with [%s] [%s]\n", cid, tokens[1], tokens[2]);
		return_code = signup(tokens[1], tokens[2]);
		if (return_code == SUCCESS)
			sprintf(response, "%d|Le compte à été crée !", return_code);
		else
			sprintf(response, "%d|Une erreur est survenue", return_code);
	}
	else if (!strcmp(tokens[0], "HOST")) {
		if (nb_tokens != 1) {
			sprintf(response, "%d|Requete Invalide", INVALID_TOKEN_COUNT);
			goto end;
		}
		printf("Client %d requested HOST\n", cid);
		if (clients[cid].status.logged == false) {
			sprintf(response, "%d|Vous devez être connecté pour continuer", UNAUTHORIZED);
			goto end;
		}
		return_code = add_match(&matchs, clients[cid].status.username, cid);
		if (return_code == SUCCESS)
			sprintf(response, "%d|La partie est désormais en ligne !", return_code);
		else
			sprintf(response, "%d|Une erreur est survenue", return_code);
	}
	else if (!strcmp(tokens[0], "FETCH")) {
		if (nb_tokens != 1) {
			sprintf(response, "%d|Requete Invalide", INVALID_TOKEN_COUNT);
			goto end;
		}
		printf("Client %d requested FETCH\n", cid);
		if (clients[cid].status.logged == false) {
			sprintf(response, "%d|Vous devez être connecté pour continuer", UNAUTHORIZED);
			goto end;
		}
		return_code = fetch_match(&matchs, &response);
	}
	else if (!strcmp(tokens[0], "SPECTATE")) {
		if (nb_tokens != 2) {
			sprintf(response, "%d|Requete Invalide", INVALID_TOKEN_COUNT);
			goto end;
		}
		printf("Client %d requested SPECTATE with [%s]\n", cid, tokens[1]);
		return_code = spectate(&matchs, tokens[1], cid);
		if (return_code == SUCCESS)
			sprintf(response, "%d|Vous êtes désormais en mode spectateur", return_code);
		else
			sprintf(response, "%d|Une erreur est survenue", return_code);
	}
	else if (!strcmp(tokens[0], "STOP")) {
		if (nb_tokens != 1) {
			sprintf(response, "%d|Requete Invalide", INVALID_TOKEN_COUNT);
			goto end;
		}
		printf("Client %d requested STOP\n", cid);
		return_code = stop_spectate(cid);
		if (return_code == SUCCESS)
			sprintf(response, "%d|Vous avez quitté le mode spectateur", return_code);
		else
			sprintf(response, "%d|Une erreur est survenue", return_code);
	}
	else if (!strcmp(tokens[0], "JOIN")) {
		if (nb_tokens != 2) {
			sprintf(response, "%d|Requete Invalide", INVALID_TOKEN_COUNT);
			goto end;
		}
		printf("Client %d requested JOIN with [%s]\n", cid, tokens[1]);
		if (clients[cid].status.logged == false) {
			sprintf(response, "%d|Vous devez être connecté pour continuer", UNAUTHORIZED);
			goto end;
		}
		return_code = join_match(&matchs, tokens[1], cid);
		if (return_code == SUCCESS) {
			char	info[25] = {0};
			
			for (int i = 0; i < MAX_CLIENTS; i++) {
				if (!strcmp(clients[i].status.username, tokens[1]) && clients[i].connected == CONNECTED && clients[i].status.logged == true) {
					sprintf(response, "%d|Partie rejointe", return_code);
					sprintf(info, "%d|%s", PLAYER_JOINED, clients[cid].status.username);
					send(clients[i].cs, info, strlen(info), 0);
					// Debug after each join (telnet)
					display_matchs_debug(&matchs);
					goto end;
				}
			}
			sprintf(response, "%d|Une erreur est survenue", INTERNAL_SERVER_ERROR);
		}
		else {
			sprintf(response, "%d|Une erreur est survenue", return_code);
		}
	}
	else if (!strcmp(tokens[0], "PLAY")) {
		if (nb_tokens != 5) {
			sprintf(response, "%d|Requete Invalide", INVALID_TOKEN_COUNT);
			goto end;
		}
		printf("Client %d requested PLAY with [%s] [%s] [%s] [%s]\n", cid, tokens[1], tokens[2], tokens[3], tokens[4]);
		if (clients[cid].status.logged == false) {
			sprintf(response, "%d|Vous devez être connecté pour continuer", UNAUTHORIZED);
			goto end;
		}
		
		play_info_t play = place_piece(&matchs, cid, atoi(tokens[1]), atoi(tokens[2]), atoi(tokens[3]), atoi(tokens[4]));
		if (play.status == SUCCESS) {
			// To do : Handle rafles when necessary soon
			// To do : Send all messages to spectators too
			// To do : Cancel game if won/cheat found to let players be able to play again
			char	info[256] = {0};
			
			if (play.role == GUEST) {
				sprintf(info, "%d|%s|%s|%s|%s|%d|%d", PIECE_PUT_BY_GUEST, tokens[1], tokens[2], tokens[3], tokens[4], play.info.prise.x, play.info.prise.y);
				send(clients[cid].cs, info, strlen(info), 0);
				sprintf(info, "%d|%s|%s|%s|%s|%d|%d", PIECE_PUT_BY_GUEST, tokens[1], tokens[2], tokens[3], tokens[4], play.info.prise.x, play.info.prise.y);
				send(clients[play.oid].cs, info, strlen(info), 0);
				for (int jl = 0; jl < MAX_CLIENTS; jl++) {
					if (clients[jl].status.spectating == true && !strcmp(clients[jl].status.watching, play.hostname)) {
						sprintf(info, "%d|%s|%s|%s|%s|%d|%d", PIECE_PUT_BY_GUEST, tokens[1], tokens[2], tokens[3], tokens[4], play.info.prise.x, play.info.prise.y);
						send(clients[jl].cs, info, strlen(info), 0);
						printf("Message [%s] sent to spectator [%d]\n", info, clients[jl].cs);
					}
				}
			}
			if (play.role == HOST) {
				sprintf(info, "%d|%s|%s|%s|%s|%d|%d", PIECE_PUT_BY_HOST, tokens[1], tokens[2], tokens[3], tokens[4], play.info.prise.x, play.info.prise.y);
				send(clients[cid].cs, info, strlen(info), 0);
				sprintf(info, "%d|%s|%s|%s|%s|%d|%d", PIECE_PUT_BY_HOST, tokens[1], tokens[2], tokens[3], tokens[4], play.info.prise.x, play.info.prise.y);
				send(clients[play.oid].cs, info, strlen(info), 0);
				for (int jl = 0; jl < MAX_CLIENTS; jl++) {
					if (clients[jl].status.spectating == true && !strcmp(clients[jl].status.watching, play.hostname)) {
						sprintf(info, "%d|%s|%s|%s|%s|%d|%d", PIECE_PUT_BY_HOST, tokens[1], tokens[2], tokens[3], tokens[4], play.info.prise.x, play.info.prise.y);
						send(clients[jl].cs, info, strlen(info), 0);
						printf("Message [%s] sent to spectator [%d]\n", info, clients[jl].cs);
					}
				}
			}
			// Turn change notification (Outside rafles)
			sprintf(response, "%d|", TURN_CHANGED);
			sprintf(info, "%d|", TURN_CHANGED);
			printf("Message [%s] sent to [%d]\n", info, clients[play.oid].cs);
			printf("Message [%s] sent to [%d]\n", response, clients[cid].cs);
			send(clients[play.oid].cs, info, strlen(info), 0);
			
			// Debug after each turn (telnet)
			display_matchs_debug(&matchs);
		}
		else if (play.status != NOT_YOUR_TURN && play.status != INVALID_PLACEMENT) {
			sprintf(response, "%d|Une erreur est survenue", return_code);
		}
		else {
			char	info[50] = {0};
			
			sprintf(response, "%d|Votre placement constitue un cas de triche", CHEATING_ATTEMPT_CATCHED);
			sprintf(info, "%d|Le joueur adverse a triché", CHEATING_ATTEMPT_CATCHED);
			send(clients[play.oid].cs, info, strlen(info), 0);
		}
	}
	else
		sprintf(response, "%d|La Requete n'existe pas", REQUEST_DOES_NOT_EXIST);
	end:
	deltab((void ***)&tokens, nb_tokens);
	return (response);
}
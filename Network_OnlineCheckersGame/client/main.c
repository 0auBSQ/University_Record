#include "main.h"

client_t	init_client(char **argv) {
	client_t	client = {0};
	
	client.s = socket(AF_INET, SOCK_STREAM, 0);
	if (client.s < 0) {
		perror("socket");
		exit(1);
	}
	client.sin.sin_family = AF_INET;
	client.sin.sin_port = htons((unsigned short)DEFAULT_PORT);
	inet_aton(argv[1], &client.sin.sin_addr);
	for (int i = 0; i < 8; i++) {
		client.sin.sin_zero[i] = 0;
	}
	if (connect(client.s, (struct sockaddr *)&client.sin, sizeof(struct sockaddr_in)) < 0) {
		perror("connect");
		exit(1);
	}
	client.logged = false;
	client.spectating = false;
	client.hosting = false;
	bzero(client.username, 17);
	return client;
}

jump_t	take_piece(client_t *match, int xo, int yo, int x, int y, int role) {
	jump_t	prise;
	bool	x_off_positive = true;
	bool	y_off_positive = true;
	
	prise.take = false;
	prise.x = -1;
	prise.y = -1;
	if (xo > 9 || xo < 0 || x > 9 || x < 0 || y > 9 || y < 0 || yo > 9 || yo < 0)
		return (prise);
	if (match->board[x][y] != 0) {
		prise.illegal = true;
		return (prise);
	}
	if ((abs(x - xo) != abs(y - yo)) && ((abs(x - xo) < 2 && abs(match->board[xo][yo]) == 2) || (abs(match->board[xo][yo]) == 2 && abs(x - xo) != 2)))
		return (prise);
	if (xo > x)
		x_off_positive = false;
	if (yo > y)
		y_off_positive = false;
	if (x_off_positive == true && y_off_positive == true) {
		for (int i = 1; i < abs(x - xo); i++) {
			if ((match->board[xo + i][yo + i] < 0 && role == GUEST) || (match->board[xo + i][yo + i] > 0 && role == HOST)) {
				if (prise.take == true) {
					prise.illegal = true;
					return (prise);
				}
				prise.take = true;
				prise.x = xo + i;
				prise.y = yo + i;
			}
		}
	}
	if (x_off_positive == false && y_off_positive == true) {
		for (int i = 1; i < abs(x - xo); i++) {
			if ((match->board[xo - i][yo + i] < 0 && role == GUEST) || (match->board[xo - i][yo + i] > 0 && role == HOST)) {
				if (prise.take == true) {
					prise.illegal = true;
					return (prise);
				}
				prise.take = true;
				prise.x = xo - i;
				prise.y = yo + i;
			}
		}
	}
	if (x_off_positive == true && y_off_positive == false) {
		for (int i = 1; i < abs(x - xo); i++) {
			if ((match->board[xo + i][yo - i] < 0 && role == GUEST) || (match->board[xo + i][yo - i] > 0 && role == HOST)) {
				if (prise.take == true) {
					prise.illegal = true;
					return (prise);
				}
				prise.take = true;
				prise.x = xo + i;
				prise.y = yo - i;
			}
		}
	}
	if (x_off_positive == false && y_off_positive == false) {
		for (int i = 1; i < abs(x - xo); i++) {
			if ((match->board[xo - i][yo - i] < 0 && role == GUEST) || (match->board[xo - i][yo - i] > 0 && role == HOST)) {
				if (prise.take == true) {
					prise.illegal = true;
					return (prise);
				}
				prise.take = true;
				prise.x = xo - i;
				prise.y = yo - i;
			}
		}
	}
	return (prise);
}

// Voir partie serveur pour la documentation
piece_info_t	can_place_piece(client_t *match, int xo, int yo, int x, int y, int role) {
	piece_info_t	info;
	
	info.legal = false;
	if (xo > 9 || xo < 0 || x > 9 || x < 0 || y > 9 || y < 0 || yo > 9 || yo < 0)
		return (info);
	if (match->board[xo][yo] > -1 && role == HOST)
		return (info);
	if (match->board[xo][yo] < 1 && role == GUEST)
		return (info);
	if (abs(x - xo) != abs(y - yo))
		return (info);
	if (abs(x - xo) < 1 || abs(y - yo) < 1)
		return (info);
	
	jump_t	prise = take_piece(match, xo, yo, x, y, role);
	if (abs(x - xo) > 1 && match->board[x][y] == 0 && abs(match->board[xo][yo]) == 1 && prise.take == false)
		return (info);
	if (prise.illegal == true)
		return (info);
	if (x < xo && role == HOST && abs(match->board[xo][yo]) == 1 && prise.take == false)
		return (info);
	if (x > xo && role == GUEST && abs(match->board[xo][yo]) == 1 && prise.take == false)
		return (info);
	info.legal = true;
	info.prise = prise;
	return (info);
}

void spectate(client_t *cl, char *host) {
	client_t c = *cl;
	
	ssize_t taille_msg;

	char buf[BUF_SIZE + 1];
	bzero(buf, BUF_SIZE + 1);
	
	printf("! Vous suivez %s !\n", host);
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 10; j++) {
			if ((i + j) % 2 == 1) {
				c.board[i][j] = HOST;
			}
		}
	}
	for (int i = 6; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if ((i + j) % 2 == 1) {
				c.board[i][j] = GUEST;
			}
		}
	}
	char o[5] = "Xx oO";
	char **token;
	
	while (1) {
		bzero(buf, BUF_SIZE + 1);
		puts("== Tableau ==");
		puts("  0123456789");
		puts(" ------------");
		for (int i = 0; i < 10; i++) {
			char *b = c.board[i];
			printf("%d|%c%c%c%c%c%c%c%c%c%c|\n", i, o[b[0]+2], o[b[1]+2], o[b[2]+2], o[b[3]+2], o[b[4]+2], o[b[5]+2], o[b[6]+2], o[b[7]+2], o[b[8]+2], o[b[9]+2]);
		}
		puts(" ------------");
		
		if ((taille_msg = recv(c.s, (void *)buf, BUF_SIZE, 0)) <= 0) {
			printf("! Serveur hors ligne : %s\n", strerror(errno));
			exit(1);
		}
		if (!strncmp(buf, "601", 3) || !strncmp(buf, "602", 3)) {
			size_t tok = strsplit(buf + 3, &token, '|');
			int code = atoi(buf);
			if (tok != 6) {
				puts("! Message corrompu");
				deltab((void ***)&token, tok);
				exit(1);
			}
			if (atoi(token[5]) >= 0 && atoi(token[4]) >= 0) {
				c.board[atoi(token[4])][atoi(token[5])] = CLEAR;
			}
			c.board[atoi(token[2])][atoi(token[3])] = c.board[atoi(token[0])][atoi(token[1])];
			c.board[atoi(token[0])][atoi(token[1])] = CLEAR;
			if (code == 601 && atoi(token[2]) == 0)
				c.board[atoi(token[2])][atoi(token[3])] = DAME_GUEST;
			else if (code == 602 && atoi(token[2]) == 9)
				c.board[atoi(token[2])][atoi(token[3])] = DAME_HOST;
			deltab((void ***)&token, tok);
		}
		else {
			buf[3] = ' ';
			printf("! %s\n", buf);
			exit(1);
		}
		
	}
}


void handle_game(client_t *cl, char *host) {
	client_t c = *cl;
	
	ssize_t taille_msg;
	bool my_turn = c.hosting;

	char buf[BUF_SIZE + 1];
	bzero(buf, BUF_SIZE + 1);
	
	char opponent[17];
	bzero(opponent, 17);
	
	if (c.hosting == true) {
		puts("! En attente d'un challenger...");
		if ((taille_msg = recv(c.s, (void *)buf, BUF_SIZE, 0)) <= 0) {
			printf("! Serveur hors ligne : %s\n", strerror(errno));
			return ;
		}
		if (!strncmp(buf, "600", 3)) {
			strncpy(opponent, buf + 4, 16);
			printf("! %s a rejoint la partie !\n", opponent);
		}
	}
	else {
		strncpy(opponent, host, 16);
		printf("! Vous avez rejoint %s !\n", opponent);
	}
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 10; j++) {
			if ((i + j) % 2 == 1) {
				c.board[i][j] = HOST;
			}
		}
	}
	for (int i = 6; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if ((i + j) % 2 == 1) {
				c.board[i][j] = GUEST;
			}
		}
	}
	char o[5] = "Xx oO";
	char **token;
	
	while (1) {
		bzero(buf, BUF_SIZE + 1);
		puts("== Tableau ==");
		puts("  0123456789");
		puts(" ------------");
		for (int i = 0; i < 10; i++) {
			char *b = c.board[i];
			printf("%d|%c%c%c%c%c%c%c%c%c%c|\n", i, o[b[0]+2], o[b[1]+2], o[b[2]+2], o[b[3]+2], o[b[4]+2], o[b[5]+2], o[b[6]+2], o[b[7]+2], o[b[8]+2], o[b[9]+2]);
		}
		puts(" ------------");
		
		if (my_turn == false) {
			puts("! En attente du placement de l'adversaire");
		}
		else {
			puts("== Placer une piece (colonne_origine|ligne_origine|colonne_cible|ligne_cible) ==");
			printf("# : ");
			strcpy(buf, "PLAY|");
			fgets(buf + 5, BUF_SIZE - 5, stdin);
			
			size_t tok = strsplit(buf + 5, &token, '|');
			if (tok != 4) {
				deltab((void ***)&token, tok);
				puts("! Placement incorrect");
				continue ;
			}
			piece_info_t info = can_place_piece(&c, atoi(token[0]), atoi(token[1]), atoi(token[2]), atoi(token[3]), (c.hosting == true) ? HOST : GUEST);
			if (info.legal == false) {
				deltab((void ***)&token, tok);
				puts("! Placement incorrect");
				continue ;
			}
			deltab((void ***)&token, tok);
			
			if ((taille_msg = send(c.s, (void *)buf, strlen(buf), 0)) < 0) {
				dprintf(2, "! Echec de l'envoi : %s\n", strerror(errno));
				exit(1);
			}
		}
			
		if ((taille_msg = recv(c.s, (void *)buf, BUF_SIZE, 0)) <= 0) {
			printf("! Serveur hors ligne : %s\n", strerror(errno));
			exit(1);
		}
		if (!strncmp(buf, "601", 3) || !strncmp(buf, "602", 3)) {
			size_t tok = strsplit(buf + 3, &token, '|');
			int code = atoi(buf);
			if (tok != 6) {
				puts("! Message corrompu");
				deltab((void ***)&token, tok);
				exit(1);
			}
			if (atoi(token[5]) >= 0 && atoi(token[4]) >= 0) {
				c.board[atoi(token[4])][atoi(token[5])] = CLEAR;
			}
			c.board[atoi(token[2])][atoi(token[3])] = c.board[atoi(token[0])][atoi(token[1])];
			c.board[atoi(token[0])][atoi(token[1])] = CLEAR;
			if (code == 601 && atoi(token[2]) == 0)
				c.board[atoi(token[2])][atoi(token[3])] = DAME_GUEST;
			else if (code == 602 && atoi(token[2]) == 9)
				c.board[atoi(token[2])][atoi(token[3])] = DAME_HOST;
			deltab((void ***)&token, tok);
		}
		else {
			buf[3] = ' ';
			printf("! %s\n", buf);
			exit(1);
		}
			
		bzero(buf, BUF_SIZE + 1);
			
		if ((taille_msg = recv(c.s, (void *)buf, BUF_SIZE, 0)) <= 0) {
			printf("! Serveur hors ligne : %s\n", strerror(errno));
			exit(1);
		}
		if (!strncmp(buf, "604", 3)) {
			my_turn = !my_turn;
		}
		else {
			buf[3] = ' ';
			printf("! %s\n", buf);
			exit(1);
		}
		
		
	}
}

int main(int argc, char **argv) {
  if (argc != 2) {
    puts("usage : /main [server ip]");
    exit(1);
  }
  
  client_t	c = init_client(argv);
 
  char buf[BUF_SIZE + 1];
  bzero(buf, BUF_SIZE + 1);
  ssize_t taille_msg;
  
  char *joining = malloc(sizeof(char) * 256);
  if (!joining)
	exit(1);

   while (1) {
	
	bzero(buf, BUF_SIZE + 1);
	if (c.logged == false) {
		puts("# Menu :");
		puts("# 1 : Créer un compte");
		puts("# 2 : Se connecter");
		puts("# 3 : Mode spectateur");
		puts("# 4 : Quitter");
	}
	else {
		printf("== Bienvenue %s ==\n", c.username);
		puts("# 1 : Créer une partie");
		puts("# 2 : Joindre une partie");
		puts("# 3 : Lister les parties en attente");
		puts("# 4 : Quitter");
	}
	
	int cur_opt = 0;
	char ent[10];
	bzero(ent, 10);
	printf("# : ");
	fgets(ent, 9, stdin);
	cur_opt = atoi(ent);
	if (cur_opt > 4 || cur_opt < 0) {
		puts("! Commande invalide");
		continue ;
	}
	else if (cur_opt == 4) {
		break ;
	}
	else if (cur_opt == 1) {
		if (c.logged == false) {
			puts("== Entrer les informations sous la forme user|password ==");
			printf("# : ");
			strcpy(buf, "CREATE|");
			fgets(buf + 7, BUF_SIZE - 7, stdin);
		}
		else {
			strcpy(buf, "HOST");
		}
	}
	else if (cur_opt == 2) {
		if (c.logged == false) {
			puts("== Entrer les informations sous la forme user|password ==");
			printf("# : ");
			strcpy(buf, "CONNECT|");
			fgets(buf + 8, BUF_SIZE - 8, stdin);
			
			if (strchr(buf + 8, '|'))
				strncpy(c.username, buf + 8, strchr(buf + 8, '|') - (buf + 8));
		}
		else {
			puts("== Entrer le nom de l'utilisateur à rejoindre ==");
			printf("# : ");
			strcpy(buf, "JOIN|");
			fgets(buf + 5, BUF_SIZE - 5, stdin);
			joining = buf + 5;
		}
	}
	else if (cur_opt == 3) {
		if (c.logged == false) {
			puts("== Entrer le nom de l'utilisateur à regarder ==");
			printf("# : ");
			strcpy(buf, "SPECTATE|");
			fgets(buf + 9, BUF_SIZE - 9, stdin);
			joining = buf + 9;
			
		}
		else {
			strcpy(buf, "FETCH");
		}
	}

    if (!strlen(buf)) {
      break;
    }

    if ((taille_msg = send(c.s, (void *)buf, strlen(buf), 0)) < 0) {
      dprintf(2, "! Echec de l'envoi : %s\n", strerror(errno));
      break ;
    }

    if ((taille_msg = recv(c.s, (void *)buf, BUF_SIZE, 0)) <= 0) {
      printf("! Serveur hors ligne : %s\n", strerror(errno));
	  break ;
    }
    else {
	  buf[3] = ' ';
	  if (!strncmp(buf, "200", 3) && c.logged == false && cur_opt == 2) {
		strcpy(buf + 4, "Utilisateur connecté !");
		c.logged = true;
	  }
	  else if (!strncmp(buf, "200", 3) && c.logged == true && cur_opt == 1) {
		c.hosting = true;
		handle_game(&c, NULL);
	  }
	  else if (!strncmp(buf, "200", 3) && c.logged == true && cur_opt == 2) {
		c.hosting = false;
		handle_game(&c, joining);
	  }
	  else if (!strncmp(buf, "200", 3) && c.logged == false && cur_opt == 3) {
		spectate(&c, joining);
	  }
      printf("! %s\n", buf);
    }
  }

  puts("! Fin de programme");
  free(joining);
  close(c.s);
  return (0);
}

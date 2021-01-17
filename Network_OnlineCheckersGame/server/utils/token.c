#include "funcs.h"

static const char *key = "jWnZq4t7w!z%C*F-JaNdRgUkXp2s5u8x";

unsigned char	*generate_token(char *username) {
	unsigned char	*token;
	
	if (strlen(username) > 16)
		return (NULL);
	if (!(token = (unsigned char *)calloc(128, sizeof(unsigned char))))
		return (NULL);
	AES_KEY enc_key;
	AES_set_encrypt_key((unsigned char *)key, 256, &enc_key);
    AES_encrypt((unsigned char *)username, token, &enc_key);   
	return (token);
}

char			*decrypt_token(unsigned char *token) {
	char			*username;
	
	if (!(username = (char *)calloc(17, sizeof(char))))
		return (NULL);
	AES_KEY dec_key;
	AES_set_decrypt_key((unsigned char *)key, 256, &dec_key);
    AES_decrypt(token, (unsigned char *)username, &dec_key);
	return (username);
}

code_t			login(char *username, char *password, int cid, unsigned char **ret_tok) {
	unsigned char	*token = NULL;
	FILE			*fs;
	ssize_t			ret;
	size_t			len;
	char			*line = NULL;
	char			**tokens;
	size_t			nb_words;

	if (strlen(username) > 16)
		return (USERNAME_TOO_LONG);
	if (strlen(username) < 3 || strlen(password) < 2)
		return (USERNAME_OR_PASSWORD_TOO_SHORT);
	if (clients[cid].status.logged == true)
		return (ALREADY_LOGGED_IN);
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].connected == CONNECTED && clients[i].status.logged == true && !strcmp(clients[i].status.username, username))
			return (ALREADY_LOGGED_IN);
	}
	if ((fs = fopen(ACCOUNTS_FILE, "r")) == NULL)
		return (INTERNAL_SERVER_ERROR);
	flock(fileno(fs), LOCK_EX);
	while ((ret = getline(&line, &len, fs)) != -1) {
		if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = 0;
		nb_words = strsplit(line, &tokens, '|');
		if (tokens && (nb_words == 2)) {
			if (!strcmp(username, tokens[0]) && !strcmp(password, tokens[1])) {
				token = generate_token(username);
				deltab((void ***)&tokens, nb_words);
				break ;
			}
		}
		deltab((void ***)&tokens, nb_words);
	}
	free(line);
	flock(fileno(fs), LOCK_UN);
	fclose(fs);
	if (!token)
		return (ACCOUNT_NOT_FOUND);
	memcpy(clients[cid].status.session_token, token, sizeof(unsigned char) * 128);
	strcpy(clients[cid].status.username, username);
	clients[cid].status.logged = true;
	clients[cid].status.spectating = false;
	*ret_tok = &(clients[cid].status.session_token[0]);
	printf("Login : %d|%s|%d\n", cid, clients[cid].status.username, clients[cid].status.logged);
	free(token);
	return (SUCCESS);
}

code_t			signup(char *username, char *password) {
	FILE			*fs;
	ssize_t			ret;
	size_t			len;
	char			*line = NULL;
	char			**tokens;
	size_t			nb_words;
	
	if (strlen(username) > 16)
		return (USERNAME_TOO_LONG);
	if (strlen(username) < 3 || strlen(password) < 2)
		return (USERNAME_OR_PASSWORD_TOO_SHORT);
	if ((fs = fopen(ACCOUNTS_FILE, "a+")) == NULL)
		return (INTERNAL_SERVER_ERROR);
	flock(fileno(fs), LOCK_EX);
	while ((ret = getline(&line, &len, fs)) != -1) {
		if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = 0;
		nb_words = strsplit(line, &tokens, '|');
		if (tokens && (nb_words == 2)) {
			if (!strcmp(username, tokens[0])) {
				deltab((void ***)&tokens, nb_words);
				free(line);
				flock(fileno(fs), LOCK_UN);
				fclose(fs);
				return (ACCOUNT_ALREADY_EXISTS);
			}
		}
		deltab((void ***)&tokens, nb_words);
	}
	free(line);
	fwrite(username, 1, strlen(username), fs);
	fwrite("|", 1, 1, fs);
	fwrite(password, 1, strlen(password), fs);
	fwrite("\n", 1, 1, fs);
	flock(fileno(fs), LOCK_UN);
	fclose(fs);
	return (SUCCESS);
}

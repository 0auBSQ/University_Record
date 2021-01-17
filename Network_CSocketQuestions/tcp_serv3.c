#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#define BUF_SIZE 256
#define MAX_CLIENTS 3
#define WAIT_MICROS 10000

typedef struct client_s {
  struct sockaddr caddr;
  socklen_t clen;
  int cs;
  char buf[BUF_SIZE + 1];
} client_t;

int main(int argc, char **argv) {
  if (argc != 2) {
    puts("usage : ./tcp_serv3 [port]");
    exit(1);
  }
  int s = socket(AF_INET, SOCK_STREAM, 0);
  int flags_tmp;
  if (s < 0) {
    dprintf(2, "Socket failed\n");
    exit(1);
  }
  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons((unsigned short)atol(argv[1]));
  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  for (int i = 0; i < 8; i++) {
    sin.sin_zero[i] = 0;
  }

  if (bind(s, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)) < 0) {
    dprintf(2, "Bind failed\n");
    exit(1);
  }

  if (listen(s, 5) < 0) {
    dprintf(2, "Listen failed\n");
    exit(1);
  }

  // Clients
  client_t clients[MAX_CLIENTS] = {0};
  ssize_t msg_len;
  
  while (1) {

    // Block server socket to accept clients
    flags_tmp = fcntl(s, F_GETFL, 0);
    fcntl(s, F_SETFL, flags_tmp & ~O_NONBLOCK);

    // Wait all clients (limit defined by the preprocessor macro MAX_CLIENTS)
    for (int i = 0; i < MAX_CLIENTS; i++) {
      printf("Waiting for client %d...\n", i + 1);
      if ((clients[i].cs = accept(s, &(clients[i].caddr), &(clients[i].clen))) < 0) {
	dprintf(2, "Client acceptation failed\n");
	exit(1);
      }
      flags_tmp = fcntl(clients[i].cs, F_GETFL, 0);
      fcntl(clients[i].cs, F_SETFL, flags_tmp | O_NONBLOCK);
    }

    // Launch then the chat in non-blocking mode
    flags_tmp = fcntl(s, F_GETFL, 0);
    fcntl(s, F_SETFL, flags_tmp | O_NONBLOCK);

    puts("Clients found ! Chat session can begin.");

    while (1) {
      // Check if message received between each usleep call
      for (int j = 0; j < MAX_CLIENTS; j++) {
	bzero(clients[j].buf, BUF_SIZE + 1);
	if ((msg_len = recv(clients[j].cs, (void *)clients[j].buf, BUF_SIZE, 0)) >= 0) {
	  // Quit if no message (client disconnected, recv returns 0)
	  if (msg_len == 0) {
	    printf("Client %d disconnected (%s), the server will close.", j + 1, strerror(errno));
	    for (int l = 0; l < MAX_CLIENTS; l++) {
	      close(clients[l].cs);
	    }
	    close(s);
	    return (0);
	  }
	  // if message received from a client, print it on the server and send it to all other clients
	  printf("client %d => %s [length : %zd]\n", j, clients[j].buf, msg_len);
	  for (int k = 0; k < MAX_CLIENTS; k++) {
	    if (k != j) {
	      // if a client is disconnected, close all sockets and quit the server
	      if (send(clients[k].cs, clients[j].buf, msg_len, 0) < 0) {
		printf("Client %d lost connection (%s), the server will close.", k + 1, strerror(errno));
		for (int l = 0; l < MAX_CLIENTS; l++) {
		  close(clients[l].cs);
		}
		close(s);
		return (0);
	      }
	    }
	  }
	}
      }

      usleep(WAIT_MICROS);
    }
  } 
}

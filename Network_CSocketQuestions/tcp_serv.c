#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define BUF_SIZE 256

int main(int argc, char **argv) {
  if (argc != 2) {
    puts("usage : ./tcp_serv [port]");
    exit(1);
  }
  int s = socket(AF_INET, SOCK_STREAM, 0);
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

  // Msg buffer
  char buf[BUF_SIZE + 1];
  bzero(buf, BUF_SIZE + 1);

  // Client socket
  struct sockaddr caddr;
  socklen_t clen;
  int cs;

  ssize_t msg_len;
  
  while (1) {
    puts("Waiting for client...");
    if ((cs = accept(s, &caddr, &clen)) < 0) {
      dprintf(2, "Accept failed\n");
      exit(1);
    }

    puts("Client found !");

    while (1) {
      if (!strcmp(buf, "quit")) {
	puts("Connection closed by client");
	close(s);
	return (0);
      }
      bzero(buf, BUF_SIZE + 1);
      if ((msg_len = recv(cs, (void *)buf, BUF_SIZE, 0)) <= 0) {
	printf("Client disconnected : %s\n", strerror(errno));
	break ;
      }
      if (send(cs, "Message received\n", 18, 0) < 0) {
	puts("Connection with client lost");
	break ;
      }
      printf("received => %s [length : %zd]\n", buf, msg_len);
    }
  }
  
}

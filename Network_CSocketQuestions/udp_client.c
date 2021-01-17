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
#define CLIENT_PORT 12346

int main(int argc, char **argv) {
  if (argc != 3) {
    puts("usage : ./udp_client [addr] [port]");
    exit(1);
  }
  int s = socket(AF_INET, SOCK_DGRAM, 0);
  if (s < 0) {
    dprintf(2, "Socket failed\n");
    exit(1);
  }
  int on = 1;
  setsockopt(s, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));


  int cs = socket(AF_INET, SOCK_DGRAM, 0);
  if (cs < 0) {
    dprintf(2, "Socket failed\n");
    exit(1);
  }

  // Client info
  struct sockaddr_in csin;
  csin.sin_family = AF_INET;
  csin.sin_port = htons(CLIENT_PORT);
  csin.sin_addr.s_addr = htonl(INADDR_ANY);
  for (int i = 0; i < 8; i++) {
    csin.sin_zero[i] = 0;
  }

  // Server info
  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons((unsigned short)atol(argv[2]));
  inet_aton(argv[1], &sin.sin_addr);
  for (int i = 0; i < 8; i++) {
    sin.sin_zero[i] = 0;
  }

  // Debug
  printf("Server: %s, Client: %s\n", inet_ntoa(sin.sin_addr), inet_ntoa(csin.sin_addr));

  if (bind(cs, (struct sockaddr *)&csin, sizeof(struct sockaddr_in)) < 0) {
    dprintf(2, "Bind failed\n");
    exit(1);
  }

  // Msg buffer
  char buf[BUF_SIZE + 1];
  bzero(buf, BUF_SIZE + 1);
  ssize_t msg_len;

  while (1) {
    bzero(buf, BUF_SIZE + 1);
    printf("Message: ");
    fgets(buf, BUF_SIZE, stdin);
    if (!strlen(buf))
      break ;
    msg_len = sendto(s, (void *)buf, strlen(buf), 0, (struct sockaddr *)&sin, (socklen_t)sizeof(struct sockaddr));
    if (msg_len < 0) {
      dprintf(2, "Send failed : %s\n", strerror(errno));
      close(s);
      exit(1);
    }
    printf("sent => %s [length : %zd]\n", buf, msg_len);
  }
  puts("Connection ended");
  close(s);
  close(cs);
  return (0);
}

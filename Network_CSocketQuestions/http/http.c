#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h> 

#define BUF_SIZE 4096

typedef struct url_s {
  char *server;
  char *uri;
} url_t;

url_t splitURL(char *url) {
  url_t info;
  char *no_protocol = strstr(url, "//") + 2;
  // NULL (0) + 2 if no protocol
  if ((size_t)no_protocol == 2) {
    no_protocol = url;
  }
  // NULL URI
  char *delta;
  if (!(delta = strchr(no_protocol, '/'))) {
    info.server = strdup(no_protocol);
    info.uri = strdup("/");
  }
  else {
    info.server = strndup(no_protocol, (size_t)delta - (size_t)no_protocol);
    info.uri = strdup(delta);
  }
  return (info);
}

void downloadURI(url_t url_info, char *file_name) {
  // Format request header
  char *header = calloc(strlen(url_info.server) + strlen(url_info.uri) + 30, sizeof(char));
  sprintf(header, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", url_info.uri, url_info.server);

  // Socket init
  int s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0) {
    dprintf(2, "Socket failed\n");
    exit(1);
  }
  struct sockaddr_in sin;
  struct hostent *host = gethostbyname(url_info.server);
  if (!host) {
    dprintf(2, "Host lookup failed\n");
    exit(1);
  }
  sin.sin_family = AF_INET;
  sin.sin_port = htons(80);
  sin.sin_addr.s_addr = *((unsigned long*)host->h_addr);
  for (int i = 0; i < 8; i++) {
    sin.sin_zero[i] = 0;
  }
  if (connect(s, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)) < 0) {
    dprintf(2, "Connect failed : %s\n", strerror(errno));
    exit(1);
  }
  
  // File init
  int fd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC);
  
  // Request ressource
  send(s, header, strlen(header), 0);
  
  ssize_t size;
  ssize_t count = 0;
  char buf[BUF_SIZE + 1] = {0};
  while ((size = recv(s, buf, BUF_SIZE, 0)) != 0) {
    printf("[%s]\n", buf);
    count += size;
    write(fd, buf, size);
    bzero(buf, BUF_SIZE + 1);
  }
  close(fd);
  printf("%s : Fetched %zd bytes.\n", file_name, count);
  
}

int main(int argc, char **argv) {
  if (argc != 2)
    puts("usage: [url]");
  else {
    url_t splitted = splitURL(argv[1]);
    printf("[%s] [%s]\n", splitted.server, splitted.uri);
    downloadURI(splitted, "page.html");
  }
  return (0);
}

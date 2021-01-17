EX4=udp_serv
EX8=udp_client
EX13=tcp_serv
EX17=tcp_client
EX18=tcp_serv3
EX20=tcp_servfd
CC=gcc
CFLAGS= -Wall -Wextra -O2

all: $(EX4) $(EX8) $(EX13) $(EX17) $(EX18) $(EX20)

$(EX4):
	@$(CC) $(CFLAGS) udp_serv.c -o $(EX4)

$(EX8):
	@$(CC) $(CFLAGS) udp_client.c -o $(EX8)

$(EX13):
	@$(CC) $(CFLAGS) tcp_serv.c -o $(EX13)

$(EX17):
	@$(CC) $(CFLAGS) tcp_client.c -o $(EX17)

$(EX18):
	@$(CC) $(CFLAGS) tcp_serv3.c -o $(EX18)

$(EX19):
	@$(CC) $(CFLAGS) tcp_servfd.c -o $(EX20)

clean:
	@rm -f $(EX4)
	@rm -f $(EX8)
	@rm -f $(EX13)
	@rm -f $(EX17)
	@rm -f $(EX18)
	@rm -f $(EX20)

re: clean all

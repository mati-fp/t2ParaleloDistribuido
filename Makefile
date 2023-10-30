CC = gcc
CFLAGS = -I/usr/include/tirpc/ -ltirpc

all:
	rpcgen banco.x
	$(CC) -o cliente cliente.c $(CFLAGS)
	$(CC) -o servidor fatoracao_svc.c servidor.c $(CFLAGS)
		
clean:
	rm -f fatoracao_svc.c fatoracao_clnt.c fatoracao.h servidor cliente *~

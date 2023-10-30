CC = gcc
CFLAGS = -I/usr/include/tirpc/ -ltirpc

all:
	rpcgen banco.x
	$(CC) -o caixa caixa_eletronico_cliente.c $(CFLAGS)
	$(CC) -o agencia agencia_cliente.c $(CFLAGS)
	$(CC) -o servidor banco.c administracao_servidor.c $(CFLAGS)
		
clean:
	rm -f banco_clnt.c banco_svc.c banco_xdr.c banco.h servidor caixa agencia *~

CC = gcc
CFLAGS = -I/usr/include/tirpc/ -ltirpc

all:
	rpcgen banco.x
	$(CC) -o caixa caixa_cliente.c banco_xdr.c banco_clnt.c $(CFLAGS)
	$(CC) -o agencia agencia_cliente.c banco_xdr.c banco_clnt.c$(CFLAGS)
	$(CC) -o servidor administracao_servidor.c banco_xdr.c banco_svc.c $(CFLAGS)
		
clean:
	rm -f banco_clnt.c banco_svc.c banco_xdr.c banco.h servidor caixa agencia *~

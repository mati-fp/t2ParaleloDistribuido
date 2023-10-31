CC = gcc
CFLAGS = -I/usr/include/tirpc/ -ltirpc

all:
	rpcgen banco.x
	$(CC) banco_xdr.c banco_clnt.c caixa_cliente.c -o caixa $(CFLAGS)
	$(CC) banco_xdr.c banco_clnt.c agencia_cliente.c -o agencia $(CFLAGS)
	$(CC) banco_xdr.c banco_svc.c administracao_servidor.c -o servidor $(CFLAGS)
		
clean:
	rm -f banco_clnt.c banco_svc.c banco_xdr.c banco.h servidor caixa agencia *~

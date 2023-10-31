#include <stdio.h>
#include <string.h>
#include <rpc/rpc.h>
#include "operacoes.h"
#include "banco.h"


int main(int argc, char *argv[])
{
	// int valor;
	// int stat;
	// long fatorado;

	// if (argc!=3) {
	// 	fprintf(stderr,"Uso:\n%s <nome_do_servidor> <nome>\n\n",argv[0]);
	// 	return 1;
	// }

	// valor = atoi(argv[2]);
	// stat = callrpc(argv[1], FATORACAO_PROG, FATORACAO_VERSION, OBTEM_VALOR_FATORADO,
	// 	       (xdrproc_t)xdr_int, (char *)&valor, 
	// 	       (xdrproc_t)xdr_long, (char *)&fatorado );
	// if (stat!= 0) {
	// 	clnt_perrno(stat);
	// 	printf("\n");
	// 	return 1;
	// }

	// printf("Valor: %d\n",valor);
	// printf("Fatoração: %ld\n", fatorado);

	// return 0;
}



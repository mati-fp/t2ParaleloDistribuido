#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <rpc/rpc.h>
#include "operacoes.h"
#include "banco.h"

char servidor[50];
int operacao;

int stat;
int retorno;


int saque(int *id, float *valor){
	SAQUE saque;
	saque.id = *id;
	saque.valor = *valor;
	saque.operacao = rand();

	for (int i = 0; i < 3 ; i++) {
		stat = callrpc(servidor, BANCO_PROG, BANCO_VERSION, SAQUE,
			       (xdrproc_t)xdr_SAQUE, (char *)&saque, 
			       (xdrproc_t)xdr_int, (char *)&retorno );
		if (stat!= 0) {
			clnt_perrno(stat);
			printf("\n");
			return -1;
		}
		if (retorno == 1 || retorno == 2) {
			printf("Operação realizada com sucesso.\n");
			return 0;
		}
	}

	if (retorno < 0) {
		printf("Erro ao realizar operação.\n");
		return -1;
	}

}

int deposito(int *id, float *valor){
	return 0;
}

int verifica_saldo(int *id){
	return 0;
}


int main(int argc, char *argv[])
{
	srand(time(NULL));

    if (argc!=2) {
        fprintf(stderr,"Uso:\n%s <nome_do_servidor>\n\n",argv[0]);
        return 1;
    }

	strcpy(servidor, argv[1]); 

	char choice;
    do {
		int id;
		float valor;
        printf("Selecione uma operação: S - Saque, D - Deposito, V - Verificar Saldo, Q - Quit\n");
        scanf(" %c", &choice);
		choice = toupper(choice);

        switch(choice) {
            case 'S':
				printf("Digite o ID da conta: ");
				scanf("%d", &id);
				printf("Digite o valor: ");
				scanf("%f", &valor);
				saque(&id, &valor);
				break;
            case 'D':
                printf("Digite o ID da conta: ");
                scanf("%d", &id);
                printf("Digite o valor: ");
                scanf("%f", &valor);
				deposito(&id, &valor);
                break;
            case 'V':
                printf("Digite o ID da conta: ");
                scanf("%d", &id);
				verifica_saldo(&id);
                break;
            case 'Q':
                printf("Saindo...\n");
                return 0;
            default:
                printf("Operação inválida.\n");
                break;
        }

    } while(choice != 'Q');

    return 0;
}

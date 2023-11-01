#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <rpc/rpc.h>
#include "operacoes.h"
#include "banco.h"

char servidor[50];

int stat;
int retorno;


int saque(int *id, float *valor){
	SAQUE saque;
	saque.id = *id;
	saque.valor = *valor;
	saque.operacao = rand();

	for (int i = 0; i < 3 ; i++) {
		stat = callrpc(servidor, BANCO_PROG, BANCO_VERSION, SACAR,
			       (xdrproc_t)xdr_SAQUE, (char *)&saque, 
			       (xdrproc_t)xdr_int, (char *)&retorno );
		if (stat!= 0) {
			clnt_perrno(stat);
			printf("\n");
			return -1;
		} else {
			break;
		}
	}

	if (retorno == 1 || retorno == 2) {
		printf("Operação realizada com sucesso. Código retorno: %d\n", retorno);
		return 0;
	} else {
		printf("Erro ao realizar operação.Código retorno: %d\n", retorno);
		return -1;
	}

}

int deposito(int *id, float *valor){
	DEPOSITO deposito;
	deposito.id = *id;
	deposito.valor = *valor;
	deposito.operacao = rand();

	for (int i = 0; i < 3 ; i++) {
		stat = callrpc(servidor, BANCO_PROG, BANCO_VERSION, DEPOSITAR,
			       (xdrproc_t)xdr_DEPOSITO, (char *)&deposito, 
			       (xdrproc_t)xdr_int, (char *)&retorno );

		if (stat!= 0) {
			clnt_perrno(stat);
			printf("\n");
			continue;
		} else {
			break;
		}		   
	}

	if (retorno == 1 || retorno == 2) {
		printf("Operação realizada com sucesso. Código retorno: %d\n", retorno);
		return 0;
	} else {
		printf("Erro ao realizar operação.Código retorno: %d\n", retorno);
		return -1;
	}
}

int verifica_saldo(int id){

	float saldo;

	for (int i = 0; i < 3 ; i++) {
		stat = callrpc(servidor, BANCO_PROG, BANCO_VERSION, CONSULTA_SALDO,
			       (xdrproc_t)xdr_int, (char *)&id, 
			       (xdrproc_t)xdr_float, (char *)&saldo );
		if (stat!= 0) {
			clnt_perrno(stat);
			printf("\n");
			return -1;
		}
		if (saldo >= 0) {
			printf("Operação realizada com sucesso. Saldo: %f\n\n", saldo);
			return 0;
		}
		if (saldo == -1) {
			printf("Conta inexistente ou saldo negativo.\n");
			return -1;
		}
	}
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
	int id;
	float valor;
    do {
        printf("Selecione uma operação: S - Saque, D - Deposito, V - Verificar Saldo, Q - Quit\n");
        scanf(" %c", &choice);
		choice = toupper(choice);
		getchar(); // consome o '\n' deixado por scanf

        switch(choice) {
            case 'S':
				printf("Digite o ID da conta: ");
				scanf("%d", &id);
				getchar();
				printf("Digite o valor: ");
				scanf("%f", &valor);
				getchar();
				saque(&id, &valor);
				break;
            case 'D':
                printf("Digite o ID da conta: ");
                scanf("%d", &id);
				getchar();
                printf("Digite o valor: ");
                scanf("%f", &valor);
				getchar();
				deposito(&id, &valor);
                break;
            case 'V':
                printf("Digite o ID da conta: ");
                scanf("%d", &id);
				getchar();
				verifica_saldo(id);
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

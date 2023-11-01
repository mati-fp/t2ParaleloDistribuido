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
			continue;
		} else {
			break;
		}
	}

	if (retorno == 1 || retorno == 2) {
		printf("Operação realizada com sucesso. Código retorno: %d\n", retorno);
		return 0;
	} else if (retorno == -1) {
		printf("Erro ao realizar operação, saldo insuficiente. Código retorno: %d\n", retorno);
		return -1;
	} else if (retorno == -2) {
		printf("Erro ao realizar operação, número de conta inexistente. Código retorno: %d\n", retorno);
		return -1;
	} else if (retorno == -3) {
		printf("Erro ao realizar operação, conta desativada. Código retorno: %d\n", retorno);
		return -1;
	} else {
		printf("Erro ao realizar operação, problemas na conexão.\n");
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
			printf("\n\n");
			continue;
		} else {
			break;
		}		   
	}

	if (retorno == 1 || retorno == 2) {
		printf("Operação realizada com sucesso. Código retorno: %d\n\n", retorno);
		return 0;
	} else if (retorno == -1) {
		printf("Erro ao realizar operação, naõ existe depósito negativo.Código retorno: %d\n\n", retorno);
		return -1;
	} else if (retorno == -2) {
		printf("Erro ao realizar operação, número de conta inexistente. Código retorno: %d\n\n", retorno);
		return -1;
	} else if (retorno == -3) {
		printf("Erro ao realizar operação, conta desativada. Código retorno: %d\n\n", retorno);
		return -1;
	} else {
		printf("Erro ao realizar operação, problemas na conexão.\n\n");
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
			continue;
		} else {
			break;
		}
	}

	if (saldo >= 0) {
		printf("Operação realizada com sucesso. Saldo: %f\n\n", saldo);
		return 0;
	} else if (saldo == -1) {
		printf("Conta inexistente.\n\n");
		return -1;
	} else if (saldo == -2){
		printf("Conta desativada.\n\n");
		return -1;
	} else {
		printf("Erro ao realizar operação, problema na conexão\n\n");
		return -1;
	}
}

int abertura_conta(char *nome, float *saldo){
	ABERTURA_CONTA abertura_conta;
	strcpy(abertura_conta.nome, nome);
	abertura_conta.saldo = *saldo;
	abertura_conta.operacao = rand();

	for (int i = 0; i < 3 ; i++) {
		stat = callrpc(servidor, BANCO_PROG, BANCO_VERSION, CRIA_CONTA,
			       (xdrproc_t)xdr_ABERTURA_CONTA, (char *)&abertura_conta, 
			       (xdrproc_t)xdr_int, (char *)&retorno );
		if (stat!= 0) {
			clnt_perrno(stat);
			printf("\n\n");
			continue;;
		} else {
			break;
		}
	}

	if (retorno == 1 || retorno == 2) {
		printf("Operação realizada com sucesso. Código retorno: %d\n\n", retorno);
		return 0;
	} else if (retorno == -1) {
		printf("Erro ao realizar operação, saldo menor que 0. Código retorno: %d\n\n", retorno);
		return -1;
	} else {
		printf("Erro ao realizar operação, problemas na conexão.\n\n");
		return -1;
	}

}

int fechamento_conta(int id){
	for (int i = 0; i < 3 ; i++) {
		stat = callrpc(servidor, BANCO_PROG, BANCO_VERSION, FECHAMENTO_CONTA,
			       (xdrproc_t)xdr_int, (char *)&id, 
			       (xdrproc_t)xdr_int, (char *)&retorno );
		if (stat!= 0) {
			clnt_perrno(stat);
			printf("\n\n");
			continue;
		} else {
			break;
		}
	}

	if (retorno == 1 || retorno == 2) {
		printf("Operação realizada com sucesso. Código retorno: %d\n\n", retorno);
		return 0;
	} else if (retorno == -2){
		printf("Erro ao realizar operação, número de conta inexistente. Código retorno: %d\n\n", retorno);
		return -1;
	} else if (retorno == -1) {
		printf("Erro ao realizar operação, cota já está desativada. Código retorno: %d\n\n", retorno);
		return -1;
	} else {
		printf("Erro ao realizar operação, problemas na conexão.\n\n");
		return -1;
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
        printf("Selecione uma operação: \n");
		printf("S - Saque, D - Deposito, V - Verificar Saldo\n");
		printf("A - Abrir Conta, F - Fechar Conta, Q - Quit\n");
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
			case 'A':
				printf("Digite o seu nome: ");
				char nome[51];
				fgets(nome, sizeof(nome), stdin);
				nome[strcspn(nome, "\n")] = 0; // Remove o newline do final
				printf("Digite o valor do deposito inicial: ");
				scanf("%f", &valor);
				getchar();
				abertura_conta(nome, &valor);
				break;
			case 'F':
				printf("Digite o ID da conta: ");
				scanf("%d", &id);
				getchar();
				fechamento_conta(id);
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

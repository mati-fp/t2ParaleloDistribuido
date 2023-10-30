#include <stdio.h>
#include <string.h>

#define MAX_CONTAS 100

typedef struct {
	int id;
	char nome[50];
	float saldo;
	int ativa;
} Conta;


static int idContas = 0;
Conta contas[MAX_CONTAS];

int *cria_conta_1_svc(char **nome, float *saldo) {
    static long erro = -1;
    if (*saldo < 0 || idContas >= MAX_CONTAS) {
        return &erro;
    }

    Conta *conta = &contas[idContas];
    conta->id = idContas++;
    strcpy(conta->nome, *nome);
    conta->saldo = *saldo;
    conta->ativa = 1;

    static int retorno = 1;
    return &retorno;
}

int *fechamento_conta_1_svc(int *id) {
    static long erro = -1;
    if (*id < 0 || *id >= idContas) {
        return &erro;
    }

    if (contas[*id].ativa) {
        contas[*id].ativa = 0;
        static int retorno = 1;
        return &retorno;
    }

    return &erro;
}

double *consulta_saldo_1_svc(int *id) {
    static double erro = -1.0;
    if (*id < 0 || *id >= idContas || !contas[*id].ativa) {
        return &erro;
    }

    return &contas[*id].saldo;
}

int *saque_1_svc(int *id, float *valor) {
    static long erro = -1;
    if (*id < 0 || *id >= idContas || !contas[*id].ativa || contas[*id].saldo < *valor) {
        return &erro;
    }

    contas[*id].saldo -= *valor;
    static int retorno = 1;
    return &retorno;
}

int *deposito_1_svc(int *id, float *valor) {
    static long erro = -1;
    if (*valor < 0 || *id < 0 || *id >= idContas || !contas[*id].ativa) {
        return &erro;
    }

    contas[*id].saldo += *valor;
    static int retorno = 1;
    return &retorno;
}


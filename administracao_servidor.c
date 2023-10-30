#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "operacoes.h"
#include "banco.h"

#define MAX_CONTAS 100
#define MAX_OPERACOES 100
static int isInit = 0;
static int erro = -1;
static int retorno = 1;

pthread_mutex_t lock;

static int idContas = 0;
Conta contas[MAX_CONTAS];
OPERACAO operacoes[MAX_OPERACOES];

void criaMutex(){
    if (!isInit){
        pthread_mutex_init(&lock, NULL);
    }
}

int *cria_conta_1_svc(CRIA_CONTA *contaParam) {
    criaMutex();

    if (contaParam->saldo < 0 || idContas >= MAX_CONTAS) {
        return &erro;
    }

    Conta *conta = &contas[idContas];
    conta->id = idContas;
    strcpy(conta->nome, contaParam->nome);
    conta->saldo = contaParam->saldo;
    conta->ativa = 1;

    return &retorno;
}

int *fechamento_conta_1_svc(int *id) {
    criaMutex();
   
    if (*id < 0 || *id >= idContas) {
        return &erro;
    }

    if (contas[*id].ativa) {
        contas[*id].ativa = 0;
        return &retorno;
    }

    return &erro;
}

double *consulta_saldo_1_svc(int *id) {
    criaMutex();
    static double erro = -1.0;
    if (*id < 0 || *id >= idContas || !contas[*id].ativa) {
        return &erro;
    }

    return &contas[*id].saldo;
}

int *saque_1_svc(SAQUE saque) {
    criaMutex();

    pthread_mutex_lock(&lock);
    if (saque->id < 0 || saque->id >= idContas || !contas[saque->id].ativa || contas[saque->id].saldo < saque->valor) {
        return &erro;
    }

    contas[saque->id].saldo -= saque->valor;
    pthread_mutex_unlock(&lock);
    return &retorno;
}

int *deposito_1_svc(DEPOSITO deposito) {
    criaMutex();

    pthread_mutex_lock(&lock);
    if (deposito->valor < 0 || deposito->id < 0 || deposito->id >= idContas || !contas[deposito->id].ativa) {
        return &erro;
    }

    contas[deposito->id].saldo += deposito->valor;
    pthread_mutex_unlock(&lock);
    return &retorno;
}


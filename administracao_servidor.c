#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "operacoes.h"
#include "banco.h"

#define MAX_CONTAS 100
#define MAX_OPERACOES 100

const static int erro = -1;
const static int erroOperacaoJaRealizada = -2;
const static int retorno = 1;

int isInit = 0;
int idContas = 0;
int contOperacoes = 0;

Conta contas[MAX_CONTAS];
OPERACAO operacoes[MAX_OPERACOES];
pthread_mutex_t locks[MAX_CONTAS];

void criaMutex(){
    if (!isInit){
        for (int i = 0; i < MAX_CONTAS; i++){
            pthread_mutex_init(&locks[i], NULL);
        }
        isInit = 1;
    }
}

int verifica_operacao(int id) {

    for (int i = 0; i < contOperacoes; i++) {
        if (operacoes[i].id == id) {
            return i;
        }
    }

    return -1;
}

int cria_operacao(int id) {

    operacoes[contOperacoes].id = id;
    operacoes[contOperacoes].realizada = 0;
    int index = contOperacoes;
    contOperacoes++;

    return index;
}

void confirma_operacao(int index) {
    operacoes[index].realizada = 1;
}

int *cria_conta_1_svc(CRIA_CONTA *contaParam) {
    criaMutex();

    int index_verifica = verifica_operacao(contaParam->operacao);
    if (index_verifica != -1) {
        if (operacoes[index_verifica].realizada) {
            return &erroOperacaoJaRealizada;
        }
    } 

    int index = cria_operacao(contaParam->operacao);

    if (contaParam->saldo < 0 || idContas >= MAX_CONTAS) {
        return &erro;
    }

    Conta *conta = &contas[idContas];
    conta->id = idContas;
    strcpy(conta->nome, contaParam->nome);
    conta->saldo = contaParam->saldo;
    conta->ativa = 1;

    confirma_operacao(index);

    return &retorno;
}

int *fechamento_conta_1_svc(int *id) {
    criaMutex();

    int index_verifica = verifica_operacao(contaParam->operacao);
    if (index_verifica != -1) {
        if (operacoes[index_verifica].realizada) {
            return &erroOperacaoJaRealizada;
        }
    } 

    int index = cria_operacao(contaParam->operacao);
   
    if (*id < 0 || *id >= idContas) {
        return &erro;
    }

    if (contas[*id].ativa) {
        contas[*id].ativa = 0;
        return &retorno;
    }

    confirma_operacao(index);

    return &erro;
}

double *consulta_saldo_1_svc(int *id) {
    criaMutex();

    int index_verifica = verifica_operacao(contaParam->operacao);
    if (index_verifica != -1) {
        if (operacoes[index_verifica].realizada) {
            return &erroOperacaoJaRealizada;
        }
    } 

    int index = cria_operacao(contaParam->operacao);

    static double erro = -1.0;
    if (*id < 0 || *id >= idContas || !contas[*id].ativa) {
        return &erro;
    }

    confirma_operacao(index);

    return &contas[*id].saldo;
}

int *saque_1_svc(SAQUE *saque) {
    criaMutex();

    int index_verifica = verifica_operacao(contaParam->operacao);
    if (index_verifica != -1) {
        if (operacoes[index_verifica].realizada) {
            return &erroOperacaoJaRealizada;
        }
    } 

    int index = cria_operacao(contaParam->operacao);

    pthread_mutex_lock(&locks[saque->id]);
    if (saque->id < 0 || saque->id >= idContas || !contas[saque->id].ativa || contas[saque->id].saldo < saque->valor) {
        return &erro;
    }

    contas[saque->id].saldo -= saque->valor;
    pthread_mutex_unlock(&locks[saque->id]);

    confirma_operacao(index);

    return &retorno;
}

int *deposito_1_svc(DEPOSITO *deposito) {
    criaMutex();

    int index_verifica = verifica_operacao(contaParam->operacao);
    if (index_verifica != -1) {
        if (operacoes[index_verifica].realizada) {
            return &erroOperacaoJaRealizada;
        }
    } 

    int index = cria_operacao(contaParam->operacao);

    pthread_mutex_lock(&locks[deposito->id]);
    if (deposito->valor < 0 || deposito->id < 0 || deposito->id >= idContas || !contas[deposito->id].ativa) {
        return &erro;
    }

    contas[deposito->id].saldo += deposito->valor;
    pthread_mutex_unlock(&locks[deposito->id]);

    confirma_operacao(index);

    return &retorno;
}


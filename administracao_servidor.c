#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "operacoes.h"
#include "banco.h"

#define MAX_CONTAS 100
#define MAX_OPERACOES 100

static int erroContaDesativada = -3;
static int erroIdInexistente = -2;
static int erro = -1;
static int erroOperacaoJaRealizada = 2;
static int retorno = 1;

int isInit = 0;
int qtdContas = 0;
int contOperacoes = 0;

Conta contas[MAX_CONTAS];
pthread_mutex_t locks[MAX_CONTAS];
OPERACAO operacoes[MAX_OPERACOES];


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

    int index_verify = verifica_operacao(id);
    if (index_verify != -1){
        return index_verify;
    }

    operacoes[contOperacoes].id = id;
    operacoes[contOperacoes].realizada = 0;
    int index = contOperacoes;
    contOperacoes++;

    return index;
}

void confirma_operacao(int index) {
    operacoes[index].realizada = 1;
}

int *cria_conta_1_svc(ABERTURA_CONTA *contaParam, struct svc_req *) {
    criaMutex();

    int index_verifica = verifica_operacao(contaParam->operacao);
    if (index_verifica != -1) {
        if (operacoes[index_verifica].realizada) {
            return &erroOperacaoJaRealizada;
        }
    } 

    int index = cria_operacao(contaParam->operacao);

    if (contaParam->saldo < 0 || qtdContas >= MAX_CONTAS) {
        confirma_operacao(index);
        return &erro;
    }

    Conta *conta = &contas[qtdContas];
    conta->id = qtdContas;
    strcpy(conta->nome, contaParam->nome);
    conta->saldo = contaParam->saldo;
    conta->ativa = 1;

    qtdContas++;

    confirma_operacao(index);

    printf("Conta criada com sucesso. ID: %d, Saldo: %f\n", conta->id, conta->saldo);

    return &retorno;
}

int *fechamento_conta_1_svc(int *id, struct svc_req *) {
    criaMutex();
   
    if (*id < 0 || *id >= qtdContas) {
        return &erroIdInexistente;
    }

    if (contas[*id].ativa) {
        contas[*id].ativa = 0;
        return &retorno;
    }

    return &erro;
}

float *consulta_saldo_1_svc(int *id, struct svc_req *) {
    criaMutex();

    static float erroIdNaoExiste = -1.0;
    static float erroContaEstaDesativada = -2.0;
    if (*id < 0 || *id >= qtdContas) {
        return &erroIdNaoExiste;
    }
    if (!contas[*id].ativa){
        return &erroContaEstaDesativada;
    }

    return &contas[*id].saldo;
}

int *sacar_1_svc(SAQUE *contaParam, struct svc_req *) {
    criaMutex();

    int index_verifica = verifica_operacao(contaParam->operacao);
    if (index_verifica != -1) {
        if (operacoes[index_verifica].realizada) {
            return &erroOperacaoJaRealizada;
        }
    } 

    int index = cria_operacao(contaParam->operacao);
    pthread_mutex_lock(&locks[contaParam->id]);

    if (contaParam->id < 0 || contaParam->id >= qtdContas) {
        confirma_operacao(index);
        pthread_mutex_unlock(&locks[contaParam->id]);
        return &erroIdInexistente;
    }
    if (!contas[contaParam->id].ativa) {
        confirma_operacao(index);
        pthread_mutex_unlock(&locks[contaParam->id]);
        return &erroContaDesativada;
    }
    if (contas[contaParam->id].saldo < contaParam->valor){
        confirma_operacao(index);
        pthread_mutex_unlock(&locks[contaParam->id]);
        return &erro;
    }

    contas[contaParam->id].saldo -= contaParam->valor;
    pthread_mutex_unlock(&locks[contaParam->id]);

    confirma_operacao(index);

    return &retorno;
}

int *depositar_1_svc(DEPOSITO *contaParam, struct svc_req *) {
    criaMutex();

    int index_verifica = verifica_operacao(contaParam->operacao);
    if (index_verifica != -1) {
        if (operacoes[index_verifica].realizada) {
            return &erroOperacaoJaRealizada;
        }
    } 

    int index = cria_operacao(contaParam->operacao);
    
    if (contaParam->id < 0 || contaParam->id >= qtdContas) {
        confirma_operacao(index);
        return &erroIdInexistente;
    }
    if (!contas[contaParam->id].ativa) {
        confirma_operacao(index);
        return &erroContaDesativada;
    }
    if (contaParam->valor < 0) {
        confirma_operacao(index);
        return &erro;
    }

    pthread_mutex_lock(&locks[contaParam->id]);
    contas[contaParam->id].saldo += contaParam->valor;
    pthread_mutex_unlock(&locks[contaParam->id]);

    confirma_operacao(index);

    return &retorno;
}


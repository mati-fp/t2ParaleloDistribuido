/* banco.x */

#define PROGRAM_NUMBER 0x20000000
#define VERSION_NUMBER 1

struct CRIA_CONTA {
    char nome[50];
    float saldo;
    int operacao;
};

struct SAQUE {
    int id;
    float valor;
    int operacao; 
};

struct DEPOSITO {
    int id;
    float valor;
    int operacao;
};

program BANCO_PROG {                /* Programa */
    version BANCO_VERSION {         /* Versao */
        int CRIA_CONTA(CRIA_CONTA)  = 1; /* Funcao e numero */
        int FECHAMENTO_CONTA(int)   = 2;    
        double CONSULTA_SALDO(int)  = 3;
        int SAQUE(SAQUE)            = 4;
        int DEPOSITO(DEPOSITO)      = 5;
    } = VERSION_NUMBER;             /* Numero de versao */
} = PROGRAM_NUMBER;                /* Numero de programa */

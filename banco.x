/* banco.x */

#define PROGRAM_NUMBER 0x20000000
#define VERSION_NUMBER 1

program BANCO_PROG {                /* Programa */
    version BANCO_VERSION {         /* Versao */
        int CRIA_CONTA(char nome[50], float saldo) = 1; /* Funcao e numero */
        int FECHAMENTO_CONTA(int id) = 2;    
        double CONSULTA_SALDO(int id) = 3;
        int SAQUE(int id, float valor) = 4;
        int DEPOSITO(int id, float valor) = 5;
    } = VERSION_NUMBER;             /* Numero de versao */
} = PROGRAM_NUMBER;                /* Numero de programa */

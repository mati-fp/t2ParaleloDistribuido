typedef struct {
	int id;
	char nome[50];
	float saldo;
	int ativa;
} Conta;

typedef struct {
    int id;
    int realizada;
} OPERACAO;

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
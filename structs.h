typedef struct
{
    char nick[50];
    char pass[50];
    int id;
    char nome[50];
    char email[40];
    char turma[10];
    int saldo;
	int online;
} Tcliente;

typedef struct
{
    char ID[20];
    char cor[20];
    char marca[50];
    char modelo[30];
    char tipo[20];
    int mudancas;
    char matricula[15];
	int disponivel;
} Tviatura;

typedef struct {
	char viaturaID[20];
	int clienteID;
	time_t time;
} Treserva;

typedef struct {
	char viaturaID[20];
	int clienteID;
	time_t time;
} Taluguer;

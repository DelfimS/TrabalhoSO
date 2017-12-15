#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>
#include "defines.h"
#include "definesM.h"
#include "structs.h"
#include "msgstruct.h"
#define spid "servidor.pid"
#define slog "servidor.log"
FILE *serpid;
FILE *serlog;
int fiscal;
int idM;

Tcliente* clientes;
Tviatura* viaturas;
Treserva* reservas;
Taluguer* alugueres;

void associar_memorias() {

	int idC = shmget(shmKeyU, 0, 0666);
	exit_on_error(idC, "Memória de clientes não existe");

	clientes = (Tcliente*) shmat(idC, 0, 0);
	exit_on_null(clientes, "Lista de clientes nula");	

	int idV = shmget(shmKeyV, 0, 0666);
	exit_on_error(idV, "Memória de viaturas não existe");

	viaturas = (Tviatura*) shmat(idV, 0, 0);
	exit_on_null(viaturas, "Lista de viaturas nula");	
}

void trata_sinal_fiscal(int sinal){
	time_t currentt;
	struct tm *info;
	char tstring[100];

	time(&currentt);
	info = localtime(&currentt);
	strftime(tstring, 20, "%Y-%m-%d %H:%M:%S", info);

	if(sinal == SIGALRM){
		serlog = fopen(slog, "a");
		strcat(tstring, " Fiscal vai verificar dados.");
		fprintf(serlog, "%s\n", tstring);
		fclose(serlog);
		printf("%s\n", tstring);

		//percorre reservas e alugueres
	} else if(sinal == SIGINT){
		serlog = fopen(slog, "a");
		strcat(tstring, " Fiscal vai terminar sua atividade.");
		fprintf(serlog, "%s\n", tstring);
		fclose(serlog);
		printf("%s\n", tstring);
		exit(0);
	}
}

void trata_sinal(int sinal){
	time_t currentt;
	struct tm *info;
	char tstring[100];

	time(&currentt);
	info = localtime(&currentt);
	strftime(tstring, 20, "%Y-%m-%d %H:%M:%S", info);

	if(sinal == SIGINT){
		kill(fiscal, SIGINT);
		wait(NULL);
		remove(spid);
		serlog = fopen(slog, "a");
		strcat(tstring, " O servidor terminou sua atividade.");
		fprintf(serlog, "%s\n", tstring);
		fclose(serlog);
		printf("%s\n", tstring);
		if(msgctl(idM, IPC_RMID, NULL) == -1)
			printf("Não foi possível apagar a fila de mensagens\n");
		printf("Fila de mensagens apagada com sucesso\n");
		exit(0);
	}
}

int main(){
	associar_memorias();

	idM = msgget(msgKey, IPC_CREAT | IPC_EXCL | 0666);
	exit_on_error(idM, "Erro no msgget");

	printf("Fila de mensagens criada.\n");

	int pid = getpid();
	serpid = fopen(spid, "w");

	fprintf(serpid, "%d\n", pid);
	fclose(serpid);

	printf("PID: %d\n", pid);

	fiscal = fork();
	if(fiscal == 0) {
		signal(SIGALRM, trata_sinal_fiscal);
		signal(SIGINT, trata_sinal_fiscal);
		while(1){
			alarm(60);
			pause();
		}
	}
	else{
		signal(SIGINT, trata_sinal);
		int status;
		MsgClientServer mcs;
		MsgServerClient msc;
		while(1){
			int i, id;
			status = msgrcv(idM, &mcs, sizeof(mcs.dados), 1, 0);
			exit_on_error(status, "error on receiving");

			msc.tipo = mcs.dados.myid;
			if(strcmp(mcs.dados.operacao, "Login") == 0) {
				i = 0;
				msc.dados.valor1 = -1; 
				while(clientes[i].id != -1) {
					if(strcmp(clientes[i].nick, mcs.dados.info1) == 0)
						if(strcmp(clientes[i].pass, mcs.dados.info2) == 0) {
							clientes[i].online = mcs.dados.myid;
							msc.dados.valor1 = clientes[i].id;
							strcpy(msc.dados.texto, "Sessão iniciada");
							break;
						}
					i++;
				}
			} else if(strcmp(mcs.dados.operacao, "Viaturas") == 0) {
				i = 0;
				while(viaturas[i].mudancas != -1) {
					if(viaturas[i].disponivel != -1) {
						strcpy(msc.dados.texto, viaturas[i].ID);
						strcat(msc.dados.texto, " - ");
						strcat(msc.dados.texto, viaturas[i].cor);
						msc.dados.valor1 = viaturas[i].mudancas;
						
						status = msgsnd(idM, &msc, sizeof(msc.dados), 0);
						exit_on_error(status, "Error on sending");
					}
					i++;
				}
				strcpy(msc.dados.texto, "done");								
			} else if(strcmp(mcs.dados.operacao, "Reservar") == 0) {
				i=0;
				id = atoi(mcs.dados.info2);
				while(clientes[i].id != id)
					i++;
				if(clientes[i].saldo > 0) {
					i=0;
					while(strcmp(mcs.dados.info1, viaturas[i].ID) != 0 || viaturas[i].mudancas != -1)
						i++;
					if(viaturas[i].mudancas == -1)
						strcpy(msc.dados.texto, "ID inválido");
					else {
						viaturas[i].disponivel = id;
						strcpy(msc.dados.texto, "Viatura reservada");
					}
				} else 
					strcpy(msc.dados.texto, "Saldo insuficiente");
			} else if(strcmp(mcs.dados.operacao, "Alugar") == 0) {
	
			} else if(strcmp(mcs.dados.operacao, "Finalizar") == 0) {

			} else if(strcmp(mcs.dados.operacao, "Carregar") == 0) {
				i = 0;
				int adicionar = atoi(mcs.dados.info1);
				id = atoi(mcs.dados.info2);
				while(clientes[i].id != id)
					i++;
				msc.dados.valor1 = clientes[i].saldo;
				clientes[i].saldo += adicionar;
				msc.dados.valor2 = clientes[i].saldo;
			} else if(strcmp(mcs.dados.operacao, "Saldo") == 0) {
				i = 0;
				id = atoi(mcs.dados.info2);
				while(clientes[i].id != id)
					i++;
				msc.dados.valor1 = clientes[i].saldo;
			} else if(strcmp(mcs.dados.operacao, "Logout") == 0) {
				i = 0;
				id = atoi(mcs.dados.info2);
				while(clientes[i].id != id)
					i++;
				clientes[i].online = -1;
				break;
			}
			status = msgsnd(idM, &msc, sizeof(msc.dados), 0);
			exit_on_error(status, "error on sending");
		}
	}
	return 0;
}

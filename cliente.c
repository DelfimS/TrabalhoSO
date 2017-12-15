#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include "defines.h"
#include "definesM.h"
#include "msgstruct.h"

void tratar_sinal(int sinal){

	time_t currentt;
	struct tm *info;

	time(&currentt);
	info = localtime(&currentt);
	char tstring[20];
	strftime(tstring, 20, "%Y-%m-%d %H:%M:%S", info);

	if(sinal == SIGUSR1){
		printf("%s O tempo de reserva terminou.\n", tstring);
	} else if(sinal == SIGUSR2){
		printf("%s Saldo insuficiente. Regresse assim que possível.\n", tstring);
	}
}

int main(){
	
	int idM = msgget(msgKey, 0666);
	exit_on_error(idM, "Erro no msgget");

	printf("PID: %d\n", getpid());
	
	int status, clientID = -1, option = -1;
	char input[20];
	MsgClientServer mcs;
	MsgServerClient msc;
	mcs.tipo = 1;
	mcs.dados.myid = getpid();

	while(option != 0 && clientID == -1) {
		printf("1. Login\n""0. Sair\n");

		fgets(input, 20, stdin);
		option = atoi(input);

		switch(option) {
			case 1:
				strcpy(mcs.dados.operacao, "Login");
	
				printf("Username: ");
				fgets(mcs.dados.info1, 20, stdin);
				mcs.dados.info1[strlen(mcs.dados.info1) - 1] = 0;
				printf("Password: ");
				fgets(mcs.dados.info2, 20, stdin);
				mcs.dados.info2[strlen(mcs.dados.info2) - 1] = 0;

				status = msgsnd(idM, &mcs, sizeof(mcs.dados), 0);
				exit_on_error(status, "error on login, enviar pedido");

				status = msgrcv(idM, &msc, sizeof(msc.dados), getpid(), 0);
				exit_on_error(status, "error on login, receber pedido");
		
				clientID = msc.dados.valor1;
				if(clientID == -1)
					printf("Username ou password erradas\n");
				break;
			case 0:
				break;
			default:
				printf("Comando inválido\n");
		}
	}
	
	if(clientID != -1) {
		printf("%s ID: %d\n", msc.dados.texto, clientID);
		
		signal(SIGUSR1, tratar_sinal);
		signal(SIGUSR2, tratar_sinal);
	
		while(option != 0){
			sprintf(mcs.dados.info2, "%d", clientID);
			printf("1. Listar viaturas disponíveis\n""2. Iniciar reserva\n""3. Iniciar aluguer\n""4. Terminar pedido\n"
					"5. Adicionar saldo\n""6. Ver saldo\n""0. Sair\n");
		
			fgets(input, 20, stdin);
			option = atoi(input);
	
			switch (option) {
				case 1:
					strcpy(mcs.dados.operacao, "Viaturas");
					
					status = msgsnd(idM, &mcs, sizeof(mcs.dados), 0);
					exit_on_error(status, "Error on request");
					
					status = msgrcv(idM, &msc, sizeof(msc.dados), getpid(), 0);
					exit_on_error(status, "Error on receving");

					while(strcmp(msc.dados.texto, "done") != 0) {
						printf("%s. Mudancas: %d\n", msc.dados.texto, msc.dados.valor1);
	
						status = msgrcv(idM, &msc, sizeof(msc.dados), getpid(), 0);
						exit_on_error(status, "Error on receving");
					}
					break;
				case 2:
					strcpy(mcs.dados.operacao, "Reservar");
				
					printf("ID da viatura a reservar: ");
					fgets(mcs.dados.info1, 20, stdin);

					status = msgsnd(idM, &mcs, sizeof(mcs.dados), 0);
					exit_on_error(status, "Error or request");
		
					status = msgrcv(idM, &msc, sizeof(msc.dados), getpid(), 0);
					exit_on_error(status, "Error on receiving");
				
					printf("%s\n", msc.dados.texto);
					break;
				case 3:
					strcpy(mcs.dados.operacao, "Alugar");

					printf("ID da viatura a reservar: ");
					fgets(mcs.dados.info1, 20, stdin);
					
					status = msgsnd(idM, &mcs, sizeof(mcs.dados), 0);
					exit_on_error(status, "Error or request");
					
					status = msgrcv(idM, &msc, sizeof(msc.dados), getpid(), 0);
					exit_on_error(status, "Error on receiving");
					
					printf("%s\n", msc.dados.texto);
					break;
				case 4:
					strcpy(mcs.dados.operacao, "Finalizar");
					break;
				case 5:
					strcpy(mcs.dados.operacao, "Carregar");

					printf("Valor a adicionar: ");
					fgets(mcs.dados.info1, 20, stdin);

					status = msgsnd(idM, &mcs, sizeof(mcs.dados), 0);
					exit_on_error(status, "Error or request");
					
					status = msgrcv(idM, &msc, sizeof(msc.dados), getpid(), 0);
					exit_on_error(status, "Error on receiving");
					
					printf("Saldo antigo: %d\n", msc.dados.valor1);
					printf("Saldo atual: %d\n", msc.dados.valor2);
					break;
				case 6:
					strcpy(mcs.dados.operacao, "Saldo");

					status = msgsnd(idM, &mcs, sizeof(mcs.dados), 0);
					exit_on_error(status, "Error or request");
					
					status = msgrcv(idM, &msc, sizeof(msc.dados), getpid(), 0);
					exit_on_error(status, "Error on receiving");
					
					printf("Saldo atual: %d\n", msc.dados.valor1);
					break;
				case 0:
					strcpy(mcs.dados.operacao, "Logout");

					status = msgsnd(idM, &mcs, sizeof(mcs.dados), 0);
					exit_on_error(status, "Error or request");	
					break;
				default:
					printf("Comando inválido\n");
					break;
			}
		}
	}
	return 0;
}

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
#define RKeySem 7382
#define AKeySem 7547
#define slogSemKey 83467
FILE *serpid;
FILE *serlog;
int fiscal;
int idM;
int listssize;
int slogSemId;

Tcliente* clientes;
int clientesSemId;
Tviatura* viaturas;
int viaturasSemId;
Treserva* reservas;
int reservasSemId;
Taluguer* alugueres;
int alugueresSemId;

void addReserva(char id[20], int id1);

void setDisponivel(int i, char idviatura[100]);

int existsReserva(int id, char id1[20]);

void addAluguer(char id[20], int id1);

void writelog(char message[]);


void iniciar_listagens(){
    Treserva inicialR={"empty",-1,-1};
    Taluguer inicialA={"empty",-1,-1};
    int size=0;
    for (int i = 0; viaturas[i].mudancas!=-1; i++) {
        size++;
    }
    Treserva listr[size];
    Taluguer lista[size];
    for (int j = 0; j < size; ++j) {
        listr[j]=inicialR;
        lista[j]=inicialA;
    }
    reservas= listr;
    alugueres=lista;
    listssize=size;
};

void setup_semaforos(){
    int status;
	viaturasSemId=semget(shmKeyV,1,0);
	exit_on_error(viaturasSemId, "Falha no semget viaturas");
	clientesSemId=semget(shmKeyU,1,0);
	exit_on_error(viaturasSemId, "Falha no semget clientes");
    reservasSemId =semget(RKeySem,1,IPC_CREAT|IPC_EXCL|0666);
    if (reservasSemId<0)reservasSemId=semget(RKeySem,1,0);
    status=semctl(reservasSemId,0,SETVAL,1);
    exit_on_error(status,"falha no semctl");
    alugueresSemId=semget(AKeySem,1,IPC_CREAT|IPC_EXCL|0666);
    if (alugueresSemId<0)alugueresSemId=semget(AKeySem,1,0);
    status=semctl(alugueresSemId,0,SETVAL,1);
    exit_on_error(status,"falha no semctl");
    slogSemId=semget(slogSemId,1,IPC_CREAT|IPC_EXCL|0666);
    if (slogSemId<0)clientesSemId=semget(slogSemKey,1,0);
    status=semctl(slogSemId,0,SETVAL,1);
    exit_on_error(status,"falha no semctl");
}
void USemUp(){int status=semop(clientesSemId,&UP,1);exit_on_error(status,"erro no UP")};
void USemDown(){int status=semop(clientesSemId,&DOWN,1);exit_on_error(status,"erro no DOWN")};
void VSemUp(){int status=semop(viaturasSemId,&UP,1);exit_on_error(status,"erro no UP")};
void VSemDown(){int status=semop(viaturasSemId,&DOWN,1);exit_on_error(status,"erro no DOWN")};
void RSemUp(){int status=semop(reservasSemId,&UP,1);exit_on_error(status,"erro no UP")};
void RSemDown(){int status=semop(reservasSemId,&DOWN,1);exit_on_error(status,"erro no DOWN")};
void ASemUp(){int status=semop(alugueresSemId,&UP,1);exit_on_error(status,"erro no UP")};
void ASemDown(){int status=semop(alugueresSemId,&DOWN,1);exit_on_error(status,"erro no DOWN")};
void SSemUp(){int status=semop(slogSemId,&UP,1);exit_on_error(status,"erro no UP")};
void SSemDown(){int status=semop(slogSemId,&DOWN,1);exit_on_error(status,"erro no DOWN")};



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
    time_t * currentt;
    time(&currentt);
    if(sinal == SIGALRM){
        serlog = fopen(slog, "a");
        writelog(" Fiscal vai verificar dados.");
        int i;
        char message[200];
        for(i = 0; i<listssize; i++) {
            RSemDown();
            if(difftime( currentt, reservas[i].time) >= 300) {
                int j = 0;
                VSemDown();
                while(strcmp(viaturas[j].ID, reservas[i].viaturaID) != 0)
                    j++;
                viaturas[j].disponivel = 0;
                VSemUp();
                j = 0;
                USemDown();
                while(reservas[i].clienteID != clientes[j].id)j++;
                sprintf(message, " end_reserva, id=%d, viatura=%s", reservas[i].clienteID, reservas[i].viaturaID);
                writelog(message);
                if(clientes[j].online != 0)
                    kill(clientes[j].online, SIGUSR1);
                USemUp();
            }
            RSemUp();
            int k = 0;
            USemDown();
            ASemDown();
            while(clientes[k].id != alugueres[i].clienteID)
                k++;
            clientes[k].saldo--;
            if(clientes[k].saldo <= 0) {
                int dur = (int) (difftime( currentt, alugueres[i].time) / 60);
                sprintf(message, " end_aluguer, id=%d, viatura=%s, duracao=%d", clientes[k].id, alugueres[i].viaturaID, dur);
                writelog(message);
                sprintf(message, " update_saldo, id=%d, initial=%d, used=%d, final=%d", clientes[k].id, dur, dur, 0);
                writelog(message);
                if(clientes[k].online != 0)
                    kill(clientes[k].online, SIGUSR2);
            }
            USemUp();
            ASemUp();
        }
    } else if(sinal == SIGINT){
        writelog( " Fiscal vai terminar sua atividade.");
        exit(0);
    }
}

void trata_sinal(int sinal){
    if(sinal == SIGINT){
        kill(fiscal, SIGINT);
        wait(NULL);
        remove(spid);
        writelog("O servidor fechou");
        if(msgctl(idM, IPC_RMID, NULL) == -1)
            printf("Não foi possível apagar a fila de mensagens\n");
        printf("Fila de mensagens apagada com sucesso\n");
        exit(0);
    }
}
void writelog(char message[]) {
    time_t currentt;
    struct tm *info;
    char logmessage[100];

    time(&currentt);
    info = localtime(&currentt);
    strftime(logmessage, 20, "%Y-%m-%d %H:%M:%S", info);

    strcat(logmessage, message);
    SSemDown();
    serlog = fopen(slog, "a");
    fprintf(serlog, "%s\n", logmessage);
    fclose(serlog);
    SSemUp();
}

int main(){
	associar_memorias();
	setup_semaforos();
	iniciar_listagens();

	idM = msgget(msgKey, IPC_CREAT | IPC_EXCL | 0666);
	exit_on_error(idM, "Erro no msgget");

	writelog("Fila de mensagens criada.");

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
        char message[200];
		MsgClientServer mcs;
		MsgServerClient msc;
		while(1){
			int i, id;
			status = (int)msgrcv(idM, &mcs, sizeof(mcs.dados), 1, 0);
			exit_on_error(status, "error on receiving");

			msc.tipo = mcs.dados.myid;
			if(strcmp(mcs.dados.operacao, "Login") == 0) {
				i = 0;
				msc.dados.valor1 = -1;
				USemDown();
				while(clientes[i].id != -1) {
					if(strcmp(clientes[i].nick, mcs.dados.info1) == 0)
						if(strcmp(clientes[i].pass, mcs.dados.info2) == 0) {
							clientes[i].online = mcs.dados.myid;
							msc.dados.valor1 = clientes[i].id;
							strcpy(msc.dados.texto, "Sessão iniciada");
                            sprintf(message, " login_ok, name=%s, id=%d, pid=%d", clientes[i].nick, clientes[i].id, mcs.dados.myid);
                            writelog(message);
							break;
						}else {
                            sprintf(message, " login_error, name=%s, pid=%d, invalid_password", clientes[i].nick, mcs.dados.myid);
                            writelog(message);
                            break;
                        }
					i++;
				}if(msc.dados.valor1 == -1) {
                    sprintf(message, " login_error, name=%s, pid=%d, unknown_client", mcs.dados.info1, mcs.dados.myid);
                    writelog(message);
                }
				USemUp();
			} else if(strcmp(mcs.dados.operacao, "Viaturas") == 0) {
				i = 0;
				VSemDown();
                Tviatura v=viaturas[i];
				while(v.mudancas != -1) {
                    printf("%d\n",viaturas[i].disponivel);
					if(v.disponivel == 0) {
						strcpy(msc.dados.texto, v.ID);
						strcat(msc.dados.texto, " - ");
						strcat(msc.dados.texto, v.cor);
						msc.dados.valor1 = v.mudancas;
						
						status = msgsnd(idM, &msc, sizeof(msc.dados), 0);
						exit_on_error(status, "Error on sending");

                        sprintf(message, " livres_list, to=%d, info=%s", id, viaturas[i].ID);
                        writelog(message);
					}
					i++;
                    v=viaturas[i];
				}
				VSemUp();
				strcpy(msc.dados.texto, "done");								
			} else if(strcmp(mcs.dados.operacao, "Reservar") == 0) {
				i=0;
				id = atoi(mcs.dados.info2);
				USemDown();
				while(clientes[i].id != id)
					i++;
				if(clientes[i].saldo > 0) {
					i=0;
					VSemDown();
					while(strcmp(mcs.dados.info1, viaturas[i].ID) != 0 || viaturas[i].mudancas != -1)
						i++;
					if(viaturas[i].mudancas == -1)
						strcpy(msc.dados.texto, "ID inválido");
					else {
						viaturas[i].disponivel = id;
                        addReserva(viaturas[i].ID,id);
						strcpy(msc.dados.texto, "Viatura reservada");
                        sprintf(message, " start_reserva, id=%d, viatura=%s", id, viaturas[i].ID);
                        writelog(message);
					}
					VSemUp();
				} else 
					strcpy(msc.dados.texto, "Saldo insuficiente");
				USemUp();
			} else if(strcmp(mcs.dados.operacao, "Alugar") == 0) {
                i=0;
                id = atoi(mcs.dados.info2);
                USemDown();
                while(clientes[i].id != id)
                    i++;
                if(clientes[i].saldo > 0) {
                    i=0;
                    VSemDown();
                    while(strcmp(mcs.dados.info1, viaturas[i].ID) != 0 || viaturas[i].mudancas != -1)
                        i++;
                    if(viaturas[i].mudancas == -1)
                        strcpy(msc.dados.texto, "ID inválido");
                        else if (viaturas[i].disponivel!=0||existsReserva(id,viaturas[i].ID)){
                        viaturas[i].disponivel = id;
                        addAluguer(viaturas[i].ID,id);
                        strcpy(msc.dados.texto, "Viatura alugada");
                        sprintf(message, " start_aluguer, id=%d, viatura=%s", id, viaturas[i].ID);
                        writelog(message);
                        }else{strcpy(msc.dados.texto,"viatura ocupada");}
                    VSemUp();
                } else
                    strcpy(msc.dados.texto, "Saldo insuficiente");
                USemUp();
			} else if(strcmp(mcs.dados.operacao, "Finalizar") == 0) {
                id=atoi(mcs.dados.info2);
                char idviatura[100];
                strcpy(idviatura,mcs.dados.info1);
                for (int j = 0; j < listssize; j++) {
                    ASemDown();
                    if ((alugueres[j].clienteID==id)&&(strcmp(idviatura,alugueres[i].viaturaID)==0)){
                        strcpy(alugueres[j].viaturaID,"empty");
                        alugueres[j].clienteID=-1;
                        alugueres[j].time=-1;
                        setDisponivel(0,idviatura);
                        sprintf(message, " stop_aluguer, id=%d, viatura=%s", id,idviatura);
                        writelog(message);
                        break;
                    }
                    ASemUp();
                    RSemDown();
                    if ((reservas[j].clienteID==id)&&(strcmp(idviatura,reservas[i].viaturaID)==0)){
                        strcpy(reservas[j].viaturaID,"empty");
                        reservas[j].clienteID=-1;
                        reservas[j].time=-1;
                        setDisponivel(0,idviatura);
                        sprintf(message, " stop_reserva, id=%d, viatura=%s", id, idviatura);
                        writelog(message);
                        break;
                    }
                    RSemUp();
                }
			} else if(strcmp(mcs.dados.operacao, "Carregar") == 0) {
				i = 0;
				int adicionar = atoi(mcs.dados.info1);
				id = atoi(mcs.dados.info2);
				USemDown();
				while(clientes[i].id != id)
					i++;
				msc.dados.valor1 = clientes[i].saldo;
				clientes[i].saldo += adicionar;
				msc.dados.valor2 = clientes[i].saldo;
				USemUp();
                sprintf(message, " add_saldo, id=%d, initial=%d, added=%d, final=%d", id, msc.dados.valor1, adicionar, msc.dados.valor2);
                writelog(message);
			} else if(strcmp(mcs.dados.operacao, "Saldo") == 0) {
				i = 0;
				id = atoi(mcs.dados.info2);
				USemDown();
				while(clientes[i].id != id)
					i++;
				msc.dados.valor1 = clientes[i].saldo;
				USemUp();
                sprintf(message, " check_saldo, id=%d, saldo=%d, saldo_ok", id, msc.dados.valor1);
                writelog(message);
			}
			status = msgsnd(idM, &msc, sizeof(msc.dados), 0);
			exit_on_error(status, "error on sending");

            if(strcmp(mcs.dados.operacao, "Logout") == 0) {
                i = 0;
                id = atoi(mcs.dados.info2);
                USemDown();
                while(clientes[i].id != id)
                    i++;
                clientes[i].online = 0;
                USemUp();
                sprintf(message, " logout, id=%d", id);
                writelog(message);
            }
		}
	}
}

void addAluguer(char id[20], int id1) {
    ASemDown();
    for (int i = 0; i < listssize; i++) {
        if (alugueres[i].clienteID==-1){
            strcpy(alugueres[i].viaturaID,id);
            alugueres[i].clienteID=id1;
            alugueres[i].time=time(NULL);
            break;
        }
    }
    ASemUp();
}

int existsReserva(int id, char id1[20]) {
    RSemDown();
    for (int i = 0; i < listssize; i++) {
        if (strcmp(reservas[i].viaturaID,id1)==0&&id==reservas[i].clienteID){
            RSemUp();
            return 1;
        }
    }
    RSemUp();
    return 0;
}

void setDisponivel(int i, char idviatura[100]) {
    VSemDown();
    for (int j = 0; j < listssize; j++) {
        if(strcmp(viaturas[i].ID,idviatura)==0){
            viaturas[i].disponivel=i;
        }
    }
    VSemUp();
}

void addReserva(char id[20], int id1) {
    RSemDown();
    for (int i = 0; i < listssize; i++) {
        if (reservas[i].clienteID==-1){
            strcpy(reservas[i].viaturaID,id);
            reservas[i].clienteID=id1;
            reservas[i].time=time(NULL);
            break;
        }
    }
    RSemUp();
}


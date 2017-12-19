//
// Created by delfim on 10-11-2017.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#include "defines.h"
#define MaxSize 128
#define v viaturas[i]
#define c clientes[i]

Tviatura* viaturas;
int viaturassize=0;
int viaturasSemId;
Tcliente* clientes;
int clientessize=0;
int clientesSemId;
Tcliente endUser={"empty","empty",-1,"empty","empty","empty",-1,-1};
Tviatura endBike={"empty","empty","empty","empty","empty",-1,"empty",-1};

void setupSems(){
    viaturasSemId =semget(shmKeyV,1,IPC_CREAT|IPC_EXCL|0666);
    if (viaturasSemId<0)viaturasSemId=semget(shmKeyV,1,0);
    int status=semctl(viaturasSemId,0,SETVAL,1);
    exit_on_error(status,"falha no semctl");
    clientesSemId=semget(shmKeyU,1,IPC_CREAT|IPC_EXCL|0666);
    if (clientesSemId<0)clientesSemId=semget(shmKeyU,1,0);
    status=semctl(clientesSemId,0,SETVAL,1);
    exit_on_error(status,"falha no semctl");
};
void USemUp(){int status=semop(clientesSemId,&UP,1);exit_on_error(status,"erro no UP")};
void USemDown(){int status=semop(clientesSemId,&DOWN,1);exit_on_error(status,"erro no DOWN")};
void VSemUp(){int status=semop(viaturasSemId,&UP,1);exit_on_error(status,"erro no UP")};
void VSemDown(){int status=semop(viaturasSemId,&DOWN,1);exit_on_error(status,"erro no DOWN")};

void calcSize() {
	clientessize = 0;
	viaturassize = 0;
	if(fopen("./utilizadores.dat", "r") != NULL) {
		FILE * f = fopen("./utilizadores.dat", "r");
		Tcliente cdat;
		while(fread(&cdat, sizeof(cdat), 1, f) > 0)
			clientessize++;
		fclose(f);
	} else if(fopen("./utilizadores.txt", "r") != NULL) {
		char linha[MaxSize];
		FILE * f = fopen("./utilizadores.txt", "r");
		while(fgets(linha, MaxSize, f) != NULL)
			clientessize++;
		fclose(f);
	} else 
		printf("No client file\n");

	if(fopen("./viaturas.dat", "r") != NULL) {
		FILE * f = fopen("./viaturas.dat", "r");
		Tviatura vdat;
		while(fread(&vdat, sizeof(vdat), 1, f) > 0)
			viaturassize++;
		fclose(f);
	} else if(fopen("./viaturas.txt", "r") != NULL) {
		char linha[MaxSize];
		FILE * f = fopen("./viaturas.txt", "r");
		while(fgets(linha, MaxSize, f) != NULL)
			viaturassize++;
		fclose(f);
	} else
		printf("No bike file\n");
}

void setupshm() {
	calcSize();
	
	int cshm = shmget(shmKeyU, (clientessize+1) * sizeof(Tcliente), IPC_CREAT | IPC_EXCL | 0666);

	if(cshm < 0){
		cshm = shmget(shmKeyU, 0, 0);
		exit_on_error(cshm, "cshm error");

		clientes = (Tcliente*) shmat(cshm, 0, 0);
		exit_on_null(clientes, "c shmat error");

		clientessize = 0;
		while(clientes[clientessize].id != -1)
			clientessize++;
	} else {
		clientes = (Tcliente*) shmat(cshm, 0, 0);
		exit_on_null(clientes, "c shmat error");
	}

	int vshm = shmget(shmKeyV, (viaturassize+1) * sizeof(Tviatura), IPC_CREAT | IPC_EXCL | 0666);
		
	if(vshm < 0){
		vshm = shmget(shmKeyV, 0, 0);
		exit_on_error(vshm, "vshm error");

		viaturas = (Tviatura*) shmat(vshm, 0, 0);
		exit_on_null(viaturas, "v shmat error");

		viaturassize = 0;
		while(viaturas[viaturassize].mudancas != -1)
			viaturassize++;
	} else {
		viaturas = (Tviatura*) shmat(vshm, 0, 0);
		exit_on_null(viaturas, "v shmat error");
	}
}

void createShmU(){
	calcSize();
    int id=shmget(shmKeyU,(clientessize+1)* sizeof(Tcliente),IPC_CREAT | IPC_EXCL | 0666);
    exit_on_error(id,"falha no shmget");
    clientes=shmat(id,0,0);
    exit_on_null(clientes,"falha no shmat");
}
void getShmU(){
    int id=shmget(shmKeyU,(clientessize+1)* sizeof(Tcliente),0);
    if(id<0)createShmU();
    else {
        clientes = shmat(id, 0, 0);
        if (clientes == NULL)createShmU();

		clientessize = 0;
		while(clientes[clientessize].id != -1)
			clientessize++;
    }
}
void createShmV(){
	calcSize();
    int id=shmget(shmKeyV,(viaturassize+1)* sizeof(Tviatura),IPC_CREAT | IPC_EXCL | 0666);
    exit_on_error(id,"falha no shmget");
    viaturas=shmat(id,0,0);
    exit_on_null(viaturas,"falha no shmat");
}
void getShmV(){
    int id=shmget(shmKeyV,0,0);
    if (id<0)createShmV();
    else {
        viaturas = shmat(id, 0, 0);
        if (viaturas == NULL)createShmV();

		viaturassize = 0;
		while(viaturas[viaturassize].mudancas != -1)
			viaturassize++;
    }
}

void readMemory(){
    setupSems();
    int idx=0;
    viaturassize=0;
    clientessize=0;
    if(fopen("./utilizadores.dat", "r")!=NULL){
        //ler utilizadores.dat para o clientes
        FILE * fb=fopen("./utilizadores.dat", "r");
        Tcliente cdat;
        while(fread(&cdat, sizeof(cdat), 1,fb )>0) {
            clientessize++;
        }
        fclose(fb);
        fb=fopen("./utilizadores.dat", "r");
        idx=0;
        USemDown();
        while(fread(&cdat, sizeof(cdat), 1,fb )>0) {
            clientes[idx] = cdat;
            clientes[idx].online = 0;
            idx++;
        }
        clientes[idx]=endUser;
        USemUp();
        fclose(fb);
    }else if(fopen("./utilizadores.txt","r")!=NULL){
        printf("ficheiro encontrado\n");
        //ler utilizadores.txt para o clientes
        char linhau[MaxSize];
        char* tokenu;
        FILE * futxt=fopen("./utilizadores.txt","r");
        while(fgets(linhau,MaxSize,futxt)!= NULL) {
            clientessize++;
        }
        fclose(futxt);
        futxt=fopen("./utilizadores.txt","r");
        idx=0;
        USemDown();
        while(fgets(linhau,MaxSize,futxt)!= NULL) {
            Tcliente cl;
            tokenu=strtok(linhau,";");
            strcpy(cl.nick, tokenu);
            tokenu=strtok(NULL,";");
            strcpy(cl.pass, tokenu);
            tokenu=strtok(NULL,";");
            cl.id = atoi(tokenu);
            tokenu=strtok(NULL,";");
            strcpy(cl.nome, tokenu);
            tokenu=strtok(NULL,";");
            strcpy(cl.email, tokenu);
            tokenu=strtok(NULL,";");
            strcpy(cl.turma, tokenu);
            tokenu=strtok(NULL,";");
            cl.saldo = atoi(tokenu);
            cl.online=0;
            clientes[idx++] = cl;
        }
        idx++;
        clientes[idx]=endUser;
        USemUp();
        fclose(futxt);
    }
    if(fopen("./viaturas.dat", "r")!=NULL){
        //ler   viaturas.dat para o viaturas
        FILE * fb=fopen("./viaturas.dat", "r");
        Tviatura vdat;
        while(fread(&vdat, sizeof(vdat), 1,fb )>0) {
            viaturassize++;
        }
        fclose(fb);
        fb=fopen("./viaturas.dat", "r");
        idx=0;
        VSemDown();
        while(fread(&vdat, sizeof(vdat), 1,fb )>0) {
            viaturas[idx].disponivel=0;
            viaturas[idx] = vdat;
            idx++;
        }
        viaturas[idx]=endBike;
        fclose(fb);
        VSemUp();
    }else if(fopen("./viaturas.txt", "r")!=NULL){
        //ler viaturas.txt para o viaturas
        char linhav[MaxSize];
        char* tokenv;
        FILE * futxt=fopen("./viaturas.txt","r");
        while(fgets(linhav,MaxSize,futxt)!= NULL) {
            viaturassize++;
        }
        fclose(futxt);
        futxt=fopen("./viaturas.txt","r");
        idx=0;
        VSemDown();
        while(fgets(linhav,MaxSize,futxt)!= NULL) {
            Tviatura vi;
            tokenv=strtok(linhav,";");
            strcpy(vi.ID, tokenv);
            tokenv=strtok(NULL,";");
            strcpy(vi.cor, tokenv);
            tokenv=strtok(NULL,";");
            strcpy(vi.marca,tokenv);
            tokenv=strtok(NULL,";");
            strcpy(vi.modelo, tokenv);
            tokenv=strtok(NULL,";");
            strcpy(vi.tipo, tokenv);
            tokenv=strtok(NULL,";");
            vi.mudancas = atoi(tokenv);
            tokenv=strtok(NULL,";");
            strcpy(vi.matricula, tokenv);
            vi.disponivel=0;
            viaturas[idx++] = vi;
        }
        idx++;
        viaturas[idx]=endBike;
        VSemUp();
        fclose(futxt);
    }
}

void printMem(){
    printf("Clientes: \n");
    USemDown();
    for (int i=0;i<clientessize;i++){
        printf("%s;%s;%d;%s;%s;%s;%d\n",c.nick,c.pass,c.id,c.nome,c.email,c.turma,c.saldo);
    }
    USemUp();
    printf("total: %d\n",clientessize);
    printf("Viaturas: \n");
    VSemDown();
    for (int i=0;i<viaturassize;i++){
        printf("%s;%s;%s;%s;%s;%d;%s",v.ID,v.cor,v.marca,v.modelo,v.tipo,v.mudancas,v.matricula);
    }
    VSemUp();
    printf("total: %d\n",viaturassize);
}

void changeUser(){
    char nick[MaxSize];
    int opcao=0;
    printf("Nickname do utilizador:\n");
    scanf("%s",nick);
    USemDown();
    for (int (i) = 0; (i) < clientessize; ++(i)) {
        if(strcmp(clientes[i].nick,nick)==0){
            printf("%s encontrado!!\n",clientes[i].nick);
            printf("O que pertende alterar?\n1.A password\n2.O saldo\n");
            scanf("%d",&opcao);
            if(opcao==1){
                printf("Coloque a nova password\n");
                scanf("%s",clientes[i].pass);
                USemUp();
                return;
            }else if(opcao==2){
                printf("Coloque o novo saldo\n");
                scanf("%d",&clientes[i].saldo);
                USemUp();
                return;
            }
            USemUp();
            printf("Opcao Invalida");
            return;
        }
    }
    USemUp();
    printf("Utilizador nao encontrado");
}


void changeBike(){
    char ID[MaxSize];
    int option=0;
    printf("ID da viatura:\n");
    scanf("%s",ID);
    VSemDown();
    for (int (i) = 0; (i) < viaturassize; ++(i)) {
        if(strcmp(viaturas[i].ID,ID)==0){
            printf("%s encontrado!!\n",v.ID);
            printf("O que pertende alterar?\n1.A cor\n2.A marca\n3.O modelo\n4.O tipo\n5.As mudancas\n6.A matricula\n");
            scanf("%d",&option);
            switch (option){
                case 1:printf("Cor nova:\n");
                    scanf("%s",v.cor);
                    VSemUp();
                    return;
                case 2:printf("Marca nova:\n");
                    scanf("%s",v.marca);
                    VSemUp();
                    return;
                case 3:printf("Modelo novo:\n");
                    scanf("%s",v.modelo);
                    VSemUp();
                    return;
                case 4:printf("Tipo novo:\n");
                    scanf("%s",v.tipo);
                    VSemUp();
                    return;
                case 5:printf("Mudancas novas:\n");
                    scanf("%d",&v.mudancas);
                    VSemUp();
                    return;
                case 6:printf("Matricula nova:\n");
                    scanf("%s",v.matricula);
                    VSemUp();
                    return;
                default:
                    printf("Opcao invalida\n");
                    VSemUp();
                    return;
            }
        }
    }
    VSemUp();
    printf("Veiculo nao encontrado\n");
}

void saveMemToFile(){
    FILE * udat=fopen("./utilizadores.dat","w");
    USemDown();
    for (int i = 0; i < clientessize ; ++i) {
        fwrite(&c,sizeof(c),1,udat);
    }
    fclose(udat);

    FILE * utxt=fopen("./utilizadores.txt","w");
    for (int i = 0; i < clientessize ; ++i) {
        fprintf(utxt,"%s;%s;%d;%s;%s;%s;%d\n",c.nick,c.pass,c.id,c.nome,c.email,c.turma,c.saldo);
    }
    USemUp();
    fclose(utxt);

    FILE * vdat=fopen("./viaturas.dat","w");
    VSemDown();
    for (int i = 0; i < viaturassize ; ++i) {
        fwrite(&v,sizeof(v),1,vdat);
    }
    fclose(vdat);

    FILE * vtxt=fopen("./viaturas.txt","w");
    for (int i = 0; i < viaturassize ; ++i) {
        fprintf(vtxt,"%s;%s;%s;%s;%s;%d;%s",v.ID,v.cor,v.marca,v.modelo,v.tipo,v.mudancas,v.matricula);
    }
    VSemUp();
    fclose(vtxt);
}

int busyBikes(){
    int count;
    VSemDown();
    for (int i = 0; i < viaturassize; i++) {
        if (v.disponivel!=0){
            printf("%s;%s;%s;%s;%s;%d;%s",v.ID,v.cor,v.marca,v.modelo,v.tipo,v.mudancas,v.matricula);
            count++;
        }
    }
    VSemUp();
    return count;
}
int avaliableBikes(){
    int count;
    for (int i = 0; i < viaturassize; i++) {
        if (v.disponivel==0){
            printf("%s;%s;%s;%s;%s;%d;%s",v.ID,v.cor,v.marca,v.modelo,v.tipo,v.mudancas,v.matricula);
            count++;
        }
    }
    return count;
}
int main(){
    int option=-1;
	setupshm();
    setupSems();
    while(option!=0){
        printf("Escolha uma opcao:\n1.Ler dados para a memoria\n2.Imprimir memoria\n3.Alterar utilizador\n"
                       "4.Alterar viatura\n5.Guardar dados\n6.Viaturas ocupadas\n7.Viaturas disponiveis\n0.Sair\n");
        scanf("%d",&option);
        switch (option){
            case 0:
                break;
            case 1:readMemory();
                break;
            case 2:printMem();
                break;
            case 3:changeUser();
                break;
            case 4:changeBike();
                break;
            case 5:saveMemToFile();
                break;
            case 6:busyBikes();
                break;
            case 7:avaliableBikes();
                break;
            default:
                printf("Opcao invalida");
                break;
        }
    }
    return 0;
}

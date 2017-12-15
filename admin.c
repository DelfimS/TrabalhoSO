#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#include "defines.h"
#define MaxSize 128
Tviatura* viaturas;
int viaturassize=0;
Tcliente* clientes;
int clientessize=0;

void createShmU() {
	int idSU = shmget(shmKeyU, maxU * sizeof(Tcliente), IPC_CREAT | IPC_EXCL | 0666);
	
	if(idSU > 0) {	
		clientes = (Tcliente*) shmat(idSU, 0, 0);
		exit_on_null(clientes, "Failed to attach clients");

		int i;
		for(i=0; i<maxU; i++) {
			strcpy(clientes[i].nick, "empty");
			strcpy(clientes[i].pass, "empty");
			clientes[i].id = -1;
			strcpy(clientes[i].nome, "empty");
			strcpy(clientes[i].email, "empty");
			strcpy(clientes[i].turma, "empty");
			clientes[i].saldo = 0;	
		} 
	
		printf("Client memory created\n");
	} else {
		int id = shmget(shmKeyU, 0, 0666);
		clientes = (Tcliente*) shmat(id, 0, 0);
		exit_on_null(clientes, "Failed to attach clients2");

		printf("Attached to existing client memory\n");
	}
}

void createShmV() {
	int idSV = shmget(shmKeyV, maxV * sizeof(Tviatura), IPC_CREAT | IPC_EXCL | 0666);

	if(idSV > 0) {
		viaturas = (Tviatura*) shmat(idSV, 0, 0);
		exit_on_null(viaturas, "Failed to attach vehicle");

		int i;
		for(i=0; i<maxV; i++) {
			strcpy(viaturas[i].ID, "empty");
			strcpy(viaturas[i].cor, "empty");
			strcpy(viaturas[i].marca, "empty");
			strcpy(viaturas[i].modelo, "empty");
			strcpy(viaturas[i].tipo, "empty");
			viaturas[i].mudancas = -1;
			strcpy(viaturas[i].matricula, "emprty");
		}

		printf("Vehicle memory created\n");
	} else {
		int id = shmget(shmKeyV, 0, 0666);
		viaturas = (Tviatura*) shmat(id, 0, 0);
		exit_on_null(viaturas, "Failed to attach vehicle2");

		printf("Attached to existing vehicle memory\n");
	}
}

void readMemory(){
	free(viaturas);
	free(clientes);
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
		clientes=(Tcliente*)malloc((clientessize) * ((sizeof(char*)*5) + (sizeof(char)*200)   + 2*sizeof(int)) );
		fb=fopen("./utilizadores.dat", "r");
		idx=0;
		while(fread(&cdat, sizeof(cdat), 1,fb )>0) {
			clientes[idx++] = cdat;
		}
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
		clientes=(Tcliente*)malloc((clientessize) * ((sizeof(char*)*5) + (sizeof(char)*200)   + 2*sizeof(int)) );
		futxt=fopen("./utilizadores.txt","r");
		idx=0;
		while(fgets(linhau,MaxSize,futxt)!= NULL) {
			Tcliente c;
			tokenu=strtok(linhau,";");
			strcpy(c.nick, tokenu);
			tokenu=strtok(NULL,";");
			strcpy(c.pass, tokenu);
			tokenu=strtok(NULL,";");
			c.id = atoi(tokenu);
			tokenu=strtok(NULL,";");
			strcpy(c.nome, tokenu);
			tokenu=strtok(NULL,";");
			strcpy(c.email, tokenu);
			tokenu=strtok(NULL,";");
			strcpy(c.turma, tokenu);
			tokenu=strtok(NULL,";");
			c.saldo = atoi(tokenu);
			clientes[idx++] = c;
		}
		fclose(futxt);
	} else {
		printf("Não forma encontrados ficheiros para utilizadores.\n");
	}
	if(fopen("./viaturas.dat", "r")!=NULL){
		//ler   viaturas.dat para o viaturas
		FILE * fb=fopen("./viaturas.dat", "r");
		Tviatura vdat;
		while(fread(&vdat, sizeof(vdat), 1,fb )>0) {
			viaturassize++;
		}
		fclose(fb);
		viaturas=(Tviatura*)malloc((viaturassize) * ((sizeof(char*)*6) + (sizeof(char) * 155) + sizeof(int)));
		fb=fopen("./viaturas.dat", "r");
		idx=0;
		while(fread(&vdat, sizeof(vdat), 1,fb )>0) {
			viaturas[idx++] = vdat;
		}
		fclose(fb);
	}else if(fopen("./viaturas.txt", "r")!=NULL){
		//ler viaturas.txt para o viaturas
		char linhav[MaxSize];
		char* tokenv;
		FILE * futxt=fopen("./viaturas.txt","r");
		while(fgets(linhav,MaxSize,futxt)!= NULL) {
			viaturassize++;
		}
		fclose(futxt);
		viaturas=(Tviatura*)malloc((viaturassize) * ((sizeof(char*)*6) + (sizeof(char) * 155) + sizeof(int)));
		futxt=fopen("./viaturas.txt","r");
		idx=0;
		while(fgets(linhav,MaxSize,futxt)!= NULL) {
			Tviatura v;
			tokenv=strtok(linhav,";");
			strcpy(v.ID, tokenv);
			tokenv=strtok(NULL,";");
			strcpy(v.cor, tokenv);
			tokenv=strtok(NULL,";");
			strcpy(v.marca,tokenv);
			tokenv=strtok(NULL,";");
			strcpy(v.modelo, tokenv);
			tokenv=strtok(NULL,";");
			strcpy(v.tipo, tokenv);
			tokenv=strtok(NULL,";");
			v.mudancas = atoi(tokenv);
            tokenv=strtok(NULL,";");
            tokenv[strlen(tokenv) - 1] = '\0';
            strcpy(v.matricula, tokenv);
            viaturas[idx++] = v;
        }
        fclose(futxt);
    } else {
      printf("Não foram encontrados ficheiros para viaturas.\n");
    }
}

void printMem(){
    Tcliente c;
    printf("Clientes: \n");
    for (int i=0;i<clientessize;i++){
        c=clientes[i];
        printf("%s;%s;%d;%s;%s;%s;%d\n",c.nick,c.pass,c.id,c.nome,c.email,c.turma,c.saldo);
    }
    printf("total: %d\n",clientessize);
    Tviatura v;
    printf("Viaturas: \n");
    for (int i=0;i<viaturassize;i++){
        v=viaturas[i];
        printf("%s;%s;%s;%s;%s;%d;%s\n",v.ID,v.cor,v.marca,v.modelo,v.tipo,v.mudancas,v.matricula);
    }
    printf("total: %d\n",viaturassize);
}

void changeUser(){
    char nick[MaxSize];
    int opcao=0;
    printf("Nickname do utilizador:\n");
    scanf("%s",nick);
    for (int (i) = 0; (i) < clientessize; ++(i)) {
        if(strcmp(clientes[i].nick,nick)==0){
            printf("%s encontrado!!\n",clientes[i].nick);
            printf("O que pertende alterar?\n1.A password\n2.O saldo\n");
            scanf("%d",&opcao);
            if(opcao==1){
                printf("Coloque a nova password\n");
                scanf("%s",clientes[i].pass);
                return;
            }else if(opcao==2){
                printf("Coloque o novo saldo\n");
                scanf("%d",&clientes[i].saldo);
                return;
            }
            printf("Opcao Invalida");
        }
    }
    printf("Utilizador nao encontrado\n");
}

void changeBike(){
    #define v viaturas[i]
    char ID[MaxSize];
    int option=0;
    printf("ID da viatura:\n");
    scanf("%s",ID);
    for (int (i) = 0; (i) < viaturassize; ++(i)) {
        if(strcmp(viaturas[i].ID,ID)==0){
            printf("%s encontrado!!\n",v.ID);
            printf("O que pertende alterar?\n1.A cor\n2.A marca\n3.O modelo\n4.O tipo\n5.As mudancas\n6.A matricula\n");
            scanf("%d",&option);
            switch (option){
                case 1:printf("Cor nova:\n");
                    scanf("%s",v.cor);
                    return;
                case 2:printf("Marca nova:\n");
                    scanf("%s",v.marca);
                    return;
                case 3:printf("Modelo novo:\n");
                    scanf("%s",v.modelo);
                    return;
                case 4:printf("Tipo novo:\n");
                    scanf("%s",v.tipo);
                    return;
                case 5:printf("Mudancas novas:\n");
                    scanf("%d",&v.mudancas);
                    return;
                case 6:printf("Matricula nova:\n");
                    scanf("%s",v.matricula);
                    return;
                default:
                    printf("Opcao invalida\n");
                    return;
            }
        }
    }
    printf("Veiculo nao encontrado\n");
}

void saveMemToFile(){
    FILE * udat=fopen("./utilizadores.dat","w");
    for (int i = 0; i < clientessize ; ++i) {
        fwrite(&clientes[i],sizeof(clientes[i]),1,udat);
    }
    fclose(udat);
    FILE * vdat=fopen("./viaturas.dat","w");
    for (int i = 0; i < viaturassize ; ++i) {
        fwrite(&viaturas[i],sizeof(viaturas[i]),1,vdat);
    }
    fclose(vdat);
    FILE * utxt=fopen("./utilizadores.txt","w");
    #define c clientes[i]
    for (int i = 0; i < clientessize ; ++i) {
        fprintf(utxt,"%s;%s;%d;%s;%s;%s;%d\n",c.nick,c.pass,c.id,c.nome,c.email,c.turma,c.saldo);
    }
    fclose(utxt);
    FILE * vtxt=fopen("./viaturas.txt","w");
    #define v viaturas[i]
    for (int i = 0; i < viaturassize ; ++i) {
        fprintf(vtxt,"%s;%s;%s;%s;%s;%d;%s\n",v.ID,v.cor,v.marca,v.modelo,v.tipo,v.mudancas,v.matricula);
    }
    fclose(vtxt);
}
int main(){

	createShmU();
	createShmV();

    int option=-1;
    while(option!=0){
        printf("Escolha uma opcao:\n1.Ler dados para a memoria\n2.Imprimir memoria\n3.Alterar utilizador\n"
                       "4.Alterar viatura\n5.Guardar dados\n0.Sair\n");
        scanf("%d",&option);
        switch (option){
            case 0:free(viaturas);free(clientes);
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
            default:
                printf("Opcao invalida");
                break;
        }
    }
    return 0;
}

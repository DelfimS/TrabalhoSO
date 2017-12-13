//
// Created by delfim on 10-11-2017.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#include "defines.h"
#define MaxSize 128

Tviatura* viaturas;
int viaturassize=0;
int viaturasSemId=0;
Tcliente* clientes;
int clientessize=0;
int clientesSemId=0;


void setupSems(){
    viaturasSemId=semget(shmKeyV,1,IPC_CREAT|0666);
    exit_on_error(viaturasSemId,"falha no semget");
    int status=semctl(viaturasSemId,0,SETVAL,1);
    exit_on_error(status,"falha no semctl");
    clientesSemId=semget(shmKeyU,1,IPC_CREAT|0666);
    exit_on_error(clientesSemId,"falha no semget");
    status=semctl(clientesSemId,0,SETVAL,1);
    exit_on_error(status,"falha no semctl");
};
void USemUp(){int status=semop(clientesSemId,&UP,1);exit_on_error(status,"erro no UP")};
void USemDown(){int status=semop(clientesSemId,&DOWN,1);exit_on_error(status,"erro no DOWN")};
void VSemUp(){int status=semop(viaturasSemId,&UP,1);exit_on_error(status,"erro no UP")};
void VSemDown(){int status=semop(viaturasSemId,&DOWN,1);exit_on_error(status,"erro no DOWN")};

void createShmU(){
    int id=shmget(shmKeyU,clientessize* sizeof(Tcliente),IPC_CREAT | 0666);
    exit_on_error(id,"falha no shmget");
    clientes=shmat(id,0,0);
    exit_on_null(clientes,"falha no shmat");
}
void getShmU(){
    int id=shmget(shmKeyU,clientessize* sizeof(Tcliente),0);
    exit_on_error(id,"falha no shmget");
    clientes=shmat(id,0,0);
    if(clientes==NULL)createShmU();

}
void createShmV(){
    int id=shmget(shmKeyV,viaturassize* sizeof(Tviatura),IPC_CREAT | 0666);
    exit_on_error(id,"falha no shmget");
    viaturas=shmat(id,0,0);
    exit_on_null(viaturas,"falha no shmat");
}
void getShmV(){
    int id=shmget(shmKeyV,viaturassize* sizeof(Tviatura),0);
    exit_on_error(id,"falha no shmget");
    viaturas=shmat(id,0,0);
    if(viaturas==NULL)createShmV();
}
void readMemory(){
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
        getShmU();
        fclose(fb);
        fb=fopen("./utilizadores.dat", "r");
        idx=0;
        USemDown();
        while(fread(&cdat, sizeof(cdat), 1,fb )>0) {
            clientes[idx++] = cdat;
        }
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
        getShmU();
        futxt=fopen("./utilizadores.txt","r");
        idx=0;
        USemDown();
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
        getShmV();
        fb=fopen("./viaturas.dat", "r");
        idx=0;
        VSemDown();
        while(fread(&vdat, sizeof(vdat), 1,fb )>0) {
            viaturas[idx++] = vdat;
        }
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
        getShmV();
        futxt=fopen("./viaturas.txt","r");
        idx=0;
        VSemDown();
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
            strcpy(v.matricula, tokenv);
            viaturas[idx++] = v;
        }
        VSemUp();
        fclose(futxt);
    }
}

void printMem(){
    Tcliente c;
    printf("Clientes: \n");
    USemDown();
    for (int i=0;i<clientessize;i++){
        c=clientes[i];
        printf("%s;%s;%d;%s;%s;%s;%d\n",c.nick,c.pass,c.id,c.nome,c.email,c.turma,c.saldo);
    }
    USemUp();
    printf("total: %d\n",clientessize);
    Tviatura v;
    printf("Viaturas: \n");
    VSemDown();
    for (int i=0;i<viaturassize;i++){
        v=viaturas[i];
        printf("%s;%s;%s;%s;%s;%d;%s\n",v.ID,v.cor,v.marca,v.modelo,v.tipo,v.mudancas,v.matricula);
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
    #define v viaturas[i]
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
        fwrite(&clientes[i],sizeof(clientes[i]),1,udat);
    }
    fclose(udat);

    FILE * utxt=fopen("./utilizadores.txt","w");
    #define c clientes[i]
    for (int i = 0; i < clientessize ; ++i) {
        fprintf(utxt,"%s;%s;%d;%s;%s;%s;%d\n",c.nick,c.pass,c.id,c.nome,c.email,c.turma,c.saldo);
    }
    USemUp();
    fclose(utxt);

    FILE * vdat=fopen("./viaturas.dat","w");
    VSemDown();
    for (int i = 0; i < viaturassize ; ++i) {
        fwrite(&viaturas[i],sizeof(viaturas[i]),1,vdat);
    }
    fclose(vdat);

    FILE * vtxt=fopen("./viaturas.txt","w");
    #define v viaturas[i]
    for (int i = 0; i < viaturassize ; ++i) {
        fprintf(vtxt,"%s;%s;%s;%s;%s;%d;%s\n",v.ID,v.cor,v.marca,v.modelo,v.tipo,v.mudancas,v.matricula);
    }
    VSemUp();
    fclose(vtxt);
}
int main(){
    int option=-1;
    setupSems();
    while(option!=0){
        printf("Escolha uma opcao:\n1.Ler dados para a memoria\n2.Imprimir memoria\n3.Alterar utilizador\n"
                       "4.Alterar viatura\n5.Guardar dados\n0.Sair\n");
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
            default:
                printf("Opcao invalida");
                break;
        }
    }
    return 0;
}

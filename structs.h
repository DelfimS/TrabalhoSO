#include <stdio.h>

typedef struct
{
    char
            nick[50];
    char
            pass[50];
    int
            id;
    char
            nome[50];
    char
            email[40];
    char
            turma[10];
    int
            saldo;
} Tcliente;

typedef struct
{
    char
            ID[20];
    char
            cor[20];
    char
            marca[50];
    char
            modelo[30];
    char
            tipo[20];
    int
            mudancas;
    char
            matricula[15];
    int
            disponivel;
} Tviatura;
Tcliente endUser={"empty","empty",-1,"empty","empty","empty",-1};
Tviatura endBike={"empty","empty","empty","empty","empty",-1,"empty",-1};
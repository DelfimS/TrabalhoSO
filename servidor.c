#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>
#define spid "servidor.pid"
#define slog "servidor.log"
FILE *serpid;
FILE *serlog;
int fiscal;

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
		exit(0);
	}
}

int main(){

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
		while(1){
			pause();
		}
	}
}

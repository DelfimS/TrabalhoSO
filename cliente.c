#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>

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

	printf("PID: %d\n", getpid());
	signal(SIGUSR1, tratar_sinal);
	signal(SIGUSR2, tratar_sinal);
	while(1){
		pause();
	}
	return 0;
}

#define exit_on_error(s,m) if (s<0) {perror(m);exit(1);}
#define exit_on_null(s,m)  if (s==NULL) {perror(m);exit(1);}
#define shmKeyV 0x1283
#define shmKeyU 0x2134
#define maxU 50
#define maxV 50
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
struct sembuf UP={0,1,0};
struct sembuf DOWN={0,-1,0};

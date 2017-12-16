#define exit_on_error(s,m) if (s<0) {perror(m);exit(1);}
#define exit_on_null(s,m)  if (s==NULL) {perror(m);exit(1);}
#define shmKeyV 23423
#define shmKeyU 34534
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
struct sembuf UP={0,1,0};
struct sembuf DOWN={0,-1,0};
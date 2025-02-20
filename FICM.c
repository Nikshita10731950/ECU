#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

struct FICM {
  int fuel;
  float time;
};

struct SDF {
  float temperature;
  float speed;
  float pressure;
  float fused;
};

int fuel;
float time = 12.00;

int calculate(int speed, int t_pos) {
  fuel = 0;
  if ((speed >= 0 && speed < 500) && (t_pos >= 0 && t_pos < 20)) {
    fuel = 12;
  } else if ((speed >= 500 && speed < 1000) && (t_pos >= 20 && t_pos < 40)) {
    fuel = 18;
  } else if ((speed >= 1000 && speed < 2000) && (t_pos >= 40 && t_pos < 60)) {
    fuel = 24;
  } else if ((speed >= 2000 && speed < 3000) && (t_pos >= 60 && t_pos < 80)) {
    fuel = 30;
  } else if ((speed >= 3000 && speed <= 4000) &&
             (t_pos >= 80 && t_pos <= 100)) {
    fuel = 36;
  } else {
    printf("\nTHROTTLE POSITON AND RPM IS INVALID\n");
    return 1;
  }
  printf("\n Fuel Injected:%dml\t time:%.2f PM\n", fuel, time);
  time += 0.01;
}

int main() {

  struct FICM *shared_memory;
  struct SDF *memory;
  int shmid, key_t = 23452, speed, t_pos, temp;

  shmid = shmget(key_t, 1024, 0666 | IPC_CREAT);
  // printf("KEY OF THE SHARED MEMORY IS %d\n",shmid);

  if (shmid == -1) {
    printf("SHARED MEMORY NOT ASSIGNED");
    exit(1);
  }

  shared_memory = (struct FICM *)shmat(shmid, NULL, 0);
  memory = (struct SDF *)shmat(shmid, NULL, 0);

  printf("%.2f", memory->temperature);
  if (memory->temperature > 220) {
    printf("\nENGINE TEMPERATURE IS TOO HIGH , FUEL INJECTION TUBE WILL NOT "
           "WORK UNTIL TEMPERATURE FALLS BELOW 220 CELSIUS\n");
    exit(1);
  }

  printf("\nSPEED \t \tTHROTTLE POSITION\n0-499 \t \t 00-19\n500-999 \t "
         "20-39\n1000-1999 \t 40-59\n2000-2999\t 60-79\n3000-4000 \t 80-100");

  for (int i = 1; i <= 5; i++) {
    printf("\nENTER SPEED (IN RPM)\n");
    scanf("%d", &speed);
    printf("\nENTER THE THROTTLE POSITION\n");
    scanf("%d", &t_pos);
    calculate(speed, t_pos);
  }

  shared_memory->fuel = fuel;
  shared_memory->time = time;
  printf("fuel\t:%d\ttime:%.2f\n", shared_memory->fuel, shared_memory->time);
}

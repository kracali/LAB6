/*
 ============================================================================
 Name        : Lab_6.c
 Author      : Katherine Rac
 ============================================================================
 */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/timerfd.h>
#include <time.h>
#include <pthread.h>



//------------------------------- Define ----------------------------------
#define MI_PRIORIDAD 10
#define MAX_LETRAS 100
#define MAX_CADENAS 60
#define PRIMERO "Lab6_primero.txt" //Impares
#define SEGUNDO "Lab6_segundo.txt" //Pares
#define Junto "LAB6_juntos.txt"
#define Periodo_12 20
#define Periodo_3 10
#define INIT_1 1
#define INIT_2 11
#define INIT_3 6
#define Periodo_nanosegundos 1000000
#define TiempoIni_nanosegundos 1000000


//-------------------- Prototipos de funciones ----------------------------
//int timer_config(int,int);
void wait_period(const int fd);
//void imprimir_y_guardar();

//----------------------- Variables globales ------------------------------
char StringArray[MAX_CADENAS][MAX_LETRAS];
int cont1 = 0;
int cont2 = 0;
int cont = 0;

void *First (void *ptr){
	char *Buff;
	Buff = (char *)ptr; //Recupera información
	FILE *fp_first = fopen(PRIMERO, "r"); //Abrir archivo
	//Prioridad
	struct sched_param param; // estructura que contiene la prioridad
	   // Asignar prioridad y la política de escalonamiento
	param.sched_priority = MI_PRIORIDAD; // Rango entre 1 y 99, si se usa SCHED_FIFO o SCHED_RR
	// Si se usan las otras políticas (incluyendo la por defecto),
	// este valor no se usa (se debe especificar como 0).
	if (sched_setscheduler(0, SCHED_FIFO, &param) == -1){ // esta función retorna algo
		perror("sched_setscheduler falló, thread1");
		exit(20);
	}
// Siempre es bueno chequear errores. Revisar la función sched_setscheduler()
// Si el primer argumento es cero, se configura el hilo que llama a la función.
// Para esta lab usaremos la política SCHED_FIFO.

//FILE *fp_first = fopen(PRIMERO, "r"); //Abrir archivo

//Se revisa que el archivo se abra correctamente
	if(fp_first == NULL){
		printf("El archivo %s no se abrió correctamente.\n", PRIMERO);
		exit(0);
	}

	// --- Configurar Timer (se podría crear una función que regrese el file descriptor) ---
	int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0); // retorna un file descriptor
	// Siempre es bueno chequear errores. Revisar la función timerfd_create()
	// Para este lab usaremos la opción CLOCK_MONOTONIC.

	struct itimerspec itval; // estructura que contiene el período y el tiempo inicial
	// El Timer se "dispara" cada ## segundos + ## nanosegundos
	itval.it_interval.tv_sec = 0;  // Revisar el tipo de la variable (¿int, long?)
	itval.it_interval.tv_nsec = (long)Periodo_12* Periodo_nanosegundos; // Revisar el tipo de la variable (¿int, long?)

	// El Timer empezará en ## segundos + ## nanosegundos desde el momento en que se inicie el Timer
	itval.it_value.tv_sec = 0;     // Revisar el tipo de la variable
	itval.it_value.tv_nsec = (long)INIT_1*TiempoIni_nanosegundos; // Revisar el tipo de la variable

	if(timerfd_settime(timer_fd, 0, &itval, NULL) == -1){
		perror("error del timer");
	exit(1);
	}

	wait_period(timer_fd);

	while(fgets(Buff, MAX_LETRAS, fp_first) !=  NULL){
		cont1++;
		//puts(Buff);
		wait_period(timer_fd);
	}

	fclose(fp_first); //Se cierra el archivo
	pthread_exit(0); //Se sale del hilo

}

void *Second (void *ptr){
	char *Buff;
	Buff = (char *)ptr; //Recupera información
	FILE *fp_second = fopen(SEGUNDO, "r"); //Abrir archivo
	//Prioridad
	struct sched_param param; // estructura que contiene la prioridad
	   // Asignar prioridad y la política de escalonamiento
	param.sched_priority = MI_PRIORIDAD; // Rango entre 1 y 99, si se usa SCHED_FIFO o SCHED_RR
	// Si se usan las otras políticas (incluyendo la por defecto),
	// este valor no se usa (se debe especificar como 0).

	if (sched_setscheduler(0, SCHED_FIFO, &param) == -1){ // esta función retorna algo
		perror("sched_setscheduler falló, thread2");
		exit(20);
	}
	// Siempre es bueno chequear errores. Revisar la función sched_setscheduler()
	// Si el primer argumento es cero, se configura el hilo que llama a la función.
	// Para esta lab usaremos la política SCHED_FIFO.

	//FILE *fp_second = fopen(SEGUNDO, "r"); //Abrir archivo

	//Se revisa que el archivo se abra correctamente
	if(fp_second == NULL){
		printf("El archivo %s no se abrió correctamente.\n", SEGUNDO);
		exit(0);
	}

	// --- Configurar Timer (se podría crear una función que regrese el file descriptor) ---
	int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0); // retorna un file descriptor
	// Siempre es bueno chequear errores. Revisar la función timerfd_create()
	// Para este lab usaremos la opción CLOCK_MONOTONIC.

	struct itimerspec itval; // estructura que contiene el período y el tiempo inicial
	// El Timer se "dispara" cada ## segundos + ## nanosegundos
	itval.it_interval.tv_sec = 0;  // Revisar el tipo de la variable (¿int, long?)
	itval.it_interval.tv_nsec = (long)Periodo_12* Periodo_nanosegundos; // Revisar el tipo de la variable (¿int, long?)

	// El Timer empezará en ## segundos + ## nanosegundos desde el momento en que se inicie el Timer
	itval.it_value.tv_sec = 0;     // Revisar el tipo de la variable
	itval.it_value.tv_nsec = (long)INIT_2*TiempoIni_nanosegundos; // Revisar el tipo de la variable

	if(timerfd_settime(timer_fd, 0, &itval, NULL) == -1){
		perror("error del timer");
		exit(1);
	}

	wait_period(timer_fd);

	while(fgets(Buff, MAX_LETRAS, fp_second) !=  NULL){
		cont2++;
		//puts(Buff);
		wait_period(timer_fd);
	}

	fp_second = fopen(SEGUNDO, "r");
	fclose(fp_second); //Se cierra el archivo
	pthread_exit(0); //Se sale del hilo

}

void *Third (void *ptr){

	int i = 0;
	char *Buff;
	Buff = (char *)ptr; //Recupera información

	//Prioridad
	struct sched_param param; // estructura que contiene la prioridad
	   // Asignar prioridad y la política de escalonamiento
	param.sched_priority = MI_PRIORIDAD; // Rango entre 1 y 99, si se usa SCHED_FIFO o SCHED_RR
	// Si se usan las otras políticas (incluyendo la por defecto),
	// este valor no se usa (se debe especificar como 0).
	if (sched_setscheduler(0, SCHED_FIFO, &param) == -1){ // esta función retorna algo
		perror("sched_setscheduler falló, thread2");
		exit(20);
	}
	// Siempre es bueno chequear errores. Revisar la función sched_setscheduler()
	// Si el primer argumento es cero, se configura el hilo que llama a la función.
	// Para esta lab usaremos la política SCHED_FIFO.

	// --- Configurar Timer (se podría crear una función que regrese el file descriptor) ---
	int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0); // retorna un file descriptor
	// Siempre es bueno chequear errores. Revisar la función timerfd_create()
	// Para este lab usaremos la opción CLOCK_MONOTONIC.

	struct itimerspec itval; // estructura que contiene el período y el tiempo inicial
	// El Timer se "dispara" cada ## segundos + ## nanosegundos
	itval.it_interval.tv_sec = 0;  // Revisar el tipo de la variable (¿int, long?)
	itval.it_interval.tv_nsec = (long)Periodo_3* Periodo_nanosegundos; // Revisar el tipo de la variable (¿int, long?)

	// El Timer empezará en ## segundos + ## nanosegundos desde el momento en que se inicie el Timer
	itval.it_value.tv_sec = 0;     // Revisar el tipo de la variable
	itval.it_value.tv_nsec = (long)INIT_3*TiempoIni_nanosegundos; // Revisar el tipo de la variable

	if(timerfd_settime(timer_fd, 0, &itval, NULL) == -1){
		perror("error del timer");
		exit(1);
	}



	wait_period(timer_fd);
	//FILE *fp_second = fopen(SEGUNDO, "r"); //Abrir archivo

	//Copia cadenas del string array
	for(i = 0; i < MAX_CADENAS; i++){
	strcpy(StringArray[i], Buff);
	wait_period(timer_fd);
	}

	//fclose(fp_second); //Se cierra el archivo
	pthread_exit(0); //Se sale del hilo

}

int main(void) {
	//Variables
	FILE *fp_original;
	int cont = 0;
	int i = 0;
    pthread_t thread1, thread2, thread3;
    char Buffer[MAX_LETRAS]; //Buffer común

    //Rango de prioridades para SCHED_FIFO:
    printf("Rangos SCHED_FIFO: min = %d, max = %d\n\n\n", sched_get_priority_min (SCHED_FIFO),
    sched_get_priority_max(SCHED_FIFO));

//-----------------------------------------------------------------------------------
    pthread_create(&thread1, NULL, First, (void*)Buffer);
    pthread_create(&thread2, NULL, Second, (void*)Buffer);
    pthread_create(&thread3, NULL, Third, (void*)Buffer);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    //imprimir_y_guardar();


    fp_original = fopen(Junto, "w");
    cont = cont1 + cont2;

    for(i = 0; i < cont; i++)
        {
    //if((i%2) == 0) // Líneas impares (corresponden a índices pares)
    fputs(StringArray[i], fp_original); // escribe una línea en el archivo
    puts(StringArray[i]);
    //else // Líneas pares (corresponden a índices impares)
    // escribe una línea en el archivo
    }

    fclose(fp_original); // cierra el archivo original.

    puts("\n\nFin del programa... \n");

return 0;
}

void wait_period(const int fd){
	uint64_t num_periods = 0;
	read(fd, &num_periods, sizeof(num_periods));
	if(num_periods > 1){
		puts("Se pasó de un periodo.");
		exit(1);
	}
}

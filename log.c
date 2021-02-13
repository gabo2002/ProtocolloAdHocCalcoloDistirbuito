#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
FILE* file;
char * nomeFile = NULL;

void inizializza_log(char* nome_file){
    pthread_mutex_init(&mutex,NULL);

    nomeFile = malloc(strlen(nome_file)+1);
    strcpy(nomeFile,nome_file);
}


void scrivi_su_log(char* testo){

    time_t timeAttuale = time(NULL);
    char* tempo_stringa = ctime(&timeAttuale);
    tempo_stringa[strlen(tempo_stringa)-1] = '\0';
    pthread_mutex_lock(&mutex);

    file = fopen(nomeFile,"a");
    fprintf(file,"%s: %s\n",  tempo_stringa ,testo);
    fclose(file);
    pthread_mutex_unlock(&mutex);
}

void chiudi_log(){
    pthread_mutex_destroy(&mutex);
    free(nomeFile);
}

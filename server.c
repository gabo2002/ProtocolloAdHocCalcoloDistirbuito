#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <stdbool.h>
#include <signal.h>

#include "log.h"

#define MAX_CLIENT 5
#define porta 6969


//VARIABILI GLOBALI
int sd;

struct blocco{
    bool calcolato;
    __uint64_t inizioBlocco;
    __uint64_t fineBlocco;
    __uint64_t numeroDaFattorizzare;
};


static void esci(int s);
int get_socket(int);

int main(int argc,char** argv){


    inizializza_log("prova.txt");

    scrivi_su_log("Ciao come va la vita?");
    scrivi_su_log("Tutto bene dai\n");

    chiudi_log();

    printf("Fine");


    if(argc != 2){
        printf("\nErrore! Devi inserire il numero da fattorizzare\n");
        return -1;
    }        

    signal(SIGINT,esci);
    signal(SIGTERM,esci);
    signal(SIGTSTP,esci);


    inizializza_log("log.test.txt");
    int client_sock;
    __uint64_t numero;
    __uint64_t lunghezza_divisori; 
    __uint64_t *divisori;
    struct sockaddr_in client;
    char client_message [2000];

    __uint64_t numeri[3];

    int read_size;

    sscanf(argv[1],"%lu",&numero);
    sd = get_socket(porta);
    socklen_t c = sizeof(struct sockaddr_in);

    while(1){

        client_sock = accept(sd,(struct sockaddr *)&client,(socklen_t* )&c);
        
        read_size = read(client_sock,lunghezza_divisori,sizeof(lunghezza_divisori));
        divisori= malloc(lunghezza_divisori);

        printf("Ho ricevuto: %lu divisori\n",lunghezza_divisori);
        int i=0;
        printf("[");
        for(;i<lunghezza_divisori-1;i++){
            printf("%lu,",divisori[i]);
        }
        printf("%lu]",divisori[lunghezza_divisori-1]);

        write(client_sock,"ciao",4);
        

    
    
    }




    
    return 0;
}


int get_socket(int port){

    int yes = 1;
    int sd = socket(AF_INET,SOCK_STREAM,0);

    if(sd == -1){
        fprintf(stderr,"Impossibile ottenere il file descriptor\nAbort!\n");
        exit(-1);
    }

    if(setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1){
        fprintf(stderr,"Errore!\n");
        exit(-1);
    }

    struct sockaddr_in server;

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if(bind(sd,(struct sockaddr*)&server,sizeof(server)) < 0 ){
        fprintf(stderr,"Errore! Impossibile inizializzare la porta richiesta\n");
        exit(-1);
    }

    if(listen(sd,MAX_CLIENT) != 0){
        fprintf(stderr,"Impossibile mettersi in ascolto\n");
        exit(-1);
    }

    printf("Socket in ascolto con successo...\n");
    return sd;
}


static void esci(int s){
    printf("CLose\n");
    printf("Exiting...\n");
    shutdown(sd,SHUT_RDWR);
    close(sd);
    chiudi_log();
    exit(-1);
}
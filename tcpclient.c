#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include "pedpack.h"

#define MAXLINE 4096
#define MAXSEND 10
#define SERV_PORT 3000      //numero di porta


int main(int argc, char **argv){
    char c[1];
    int sockfd;
    struct sockaddr_in servaddr;            //struttura per l'indirizzo del socket
    pac pacForward;
    char sendline[MAXSEND], recvline[MAXLINE];
    
    
    //pacForward
	char mov = ' ';
	int m;
	int n = 0;
	ped p  = {0,0};

    // Controllo l'indirizzo IP inserito da linea di comando
    if (argc != 2) {
            perror("Errore: Indirizzo specificato non valido.");
            exit(2);
    }

    // Creazione del socket client
    // Se sockfd < 0 c'è stato un errore nella creazione del socket
    if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
        perror("Errore nella creazione del socket!");
        exit(2);
    }

    /* Preparazione dell'indirizzo del socket*/
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr= inet_addr(argv[1]);
    servaddr.sin_port =  htons(SERV_PORT);          //Converte in network byte order (big-endian)

    // Connessione del client al server
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
        perror("Problem in connecting to the server");
            exit(3);
    }
    
    


        for(;;){
            printf("Di quanto vuoi spostare il pedone? Attualmente si trova in %d , %d . Scegli tra nord(n), sud(s) \n", p.x, p.y);
            
            mov = ' ';
            printf("Di quanti passi vuoi muoverti?\n");
            	scanf("%d", &m);
            	
            while(mov!= 'n' && mov!= 's' && mov!= 'e' && mov!= 'o'){
            	printf("Eh no, riprova\n");
            	scanf("%c", &mov);
             }

            
            
            
        	ped.riempi_pacchetto(&pacForward, mov, m);
        	
        	send(sockfd, &pacForward, sizeof(p),0);
        	
        	
        	n=0;
        	
        	while(n!=sizeof(p))
        	n = read(sockfd, &pacForward, sizeof(p));
           } 	
    return 0;
}

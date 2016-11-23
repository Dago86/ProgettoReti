#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include "pedina.h"


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
            printf("\n\n        In posizione:\n\n**************************************\n*(Asse ascisse: %d , Asse ordinate: %d)*\n**************************************\n\n", p.x, p.y);
            printf("Scegli la direzione del percorso! Tra nord, sud, est e ovest. \n Se vuoi uscire premi x.\n\n");

            mov = ' ';

			

            while(mov!= 'n' && mov!= 's' && mov!= 'e' && mov!= 'o' && mov!='x' && 
				mov!= 'N' && mov!= 'S' && mov!= 'E' && mov!= 'O' && mov!='X'){
            	scanf("%c", &mov);
			
             }

				if(mov== 'x'){
			    printf("Esco dal programma...\n"); break;
				}

            printf("Di quanti passi vuoi muoverti?\n");
            	scanf("%d", &m);






        	riempi_pacchetto(&pacForward, mov, m);

          // invio pacchetto
         if (mov != ' ')
         if ((send(sockfd, &pacForward, sizeof(pac),0) == -1))
         {
         printf("errore invio dati\n");
         exit(1);
         }

         usleep(2000);



        	n=0;

        	while(n!=sizeof(p))
        	n = read(sockfd, &p, sizeof(p));
          usleep(3000);
           }
    return 0;
}

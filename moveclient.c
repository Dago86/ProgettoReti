#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#define MAXLINE 4096
#define SERV_PORT 3000      //numero di porta


int main(int argc, char **argv){
    int sockfd;
    struct sockaddr_in servaddr;            //struttura per l'indirizzo del socket
    char sendline[MAXLINE], recvline[MAXLINE];

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

    while (fgets(sendline, MAXLINE, stdin) != NULL) {       // La funzione fgets() legge una linea dallo stream stdin
                                                            // immagazzinandola nel buffer puntato da sendline di dimensione MAXLINE.

        while(1){
            scanf("%s", sendline);
            send(sockfd, sendline, strlen(sendline), 0);

            if (recv(sockfd, recvline, MAXLINE,0) == 0){
                perror("Il server e' stato chiuso prematuramente");
                exit(4);
            }
            printf("%s", "Stringa ricevuta dal server: ");
            fputs(recvline, stdout);
        }
    puts(recvline);

    exit(0);
    }
    close(sockfd);
    return 0;
}

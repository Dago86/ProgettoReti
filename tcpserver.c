#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include "pedina.h"
#define MAXLINE 200         // Lunghezza massima di una linea comandi
#define SERV_PORT 3000      // Numero di porta
#define MAXQ 8              // Numero massimo di client in connesione




int main (int argc, char **argv){


  void err(const char* msg){
      perror (msg);
      exit(1);
  }

    int listenfd, connfd;
    pid_t childpid;         // Intero che rappresenta l'id del processo figlio.
    socklen_t clilen;
    char buf[MAXLINE];
    struct sockaddr_in cliaddr, servaddr;   // Strutture per l'indirizzo delle porte socket in IPv4.

    // Creazione di un socket con socket(int domain, int type, int protocol)
    // domain rappresenta la famiglia di protocollo (nel nostro caso IPv4), type specifica il tipo di socket
    // (nel nostro caso un socket stream per TCP), l'ultimo parametro intero definisce il protocollo usato ed è solitamente posto a 0
    // Se listenfd risulta < 0 c'è stato un errore nella creazione del socket.
    if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
        err("Errore nella creazione del socket!");
    }

    // Ripulisce la struttura dell'indirizzo del server.
    memset (&servaddr, 0, sizeof(servaddr));

    /* Preparazione dell'indirizzo del socket*/
    // Conversione in server byte order
    servaddr.sin_family = AF_INET;
    // Imposta automaticamente con l'indirizzo IP dell'host corrente.
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // Converte l'intero associato alla porta in network byte order.
    servaddr.sin_port = htons(SERV_PORT);

    // Collega il socket all' indirizzo IP corrente sulla porta SERV_PORT
    // bind() prende come argomenti il file descriptor per il socket, la struttura dell'indirizzo del server
    // e la lunghezza della struttura dell'indirizzo.
    if(bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        err("Errore nel collegamento del socket");

    // listen() dice al socket di rimanere in ascolto in attesa di eventuali richieste di connessione
    // e inserisce le connessioni in arrivo in una coda.
    // Nel nostro caso abbiamo settato la dimensione massima della coda a 8.
    listen (listenfd, MAXQ);

    printf("%s\n","Il server è attivo e in attesa di nuove connessioni.");

    for ( ; ; ) {

        clilen = sizeof(cliaddr);
        // accept() inserisce le informazioni dell'indirizzo del client in connessione
        // nella struttura cliaddr la cui dimensione è data da clilen.
        // accept() ritorna un nuovo file descriptor per il socket collegato alla connessione in entrata.
        // In questo modo il file descriptor originale continua ad essere utilizzato per
        // rimanere in ascolto in caso di arrivo di altre connessioni.
        connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);
        printf("%s\n","Richiesta in arrivo!");

        if ( (childpid = fork ()) != 0 ) {    // Se fork() restituisce 0 significa che ha avuto successo e ha creato un processo figlio altrimenti c'e' stato un errore.
            printf("%s\n", "Disconnessione del processo figlio.");
            close(connfd);
        }else{
            printf ("%s\n","Processo figlio creato!");

            int n;      // Variabile per controllare il numero di byte ricevuti dal client.

            // Chiude il socket in ascolto.
            close (listenfd);
            ped p = {0, 0};
            pac ricevuto = {' ', 0};


            while (n = read(connfd, &ricevuto, sizeof(pac)) > 0) {      // read() ritorna la lunghezza del messaggio ricevuto dal client.
                movimento(&ricevuto, &p);		//la pedina viene spostata in base ai dati del pacchetto ricevuto dal client.
                printf("La pedina e' stata spostata di %d passi in direzione %c. La pedina ha ora coordinate (%d, %d)", ricevuto.passi, ricevuto.dir, p.x, p.y);
                write(connfd, &p, sizeof(ped));
				fflush(stdout);
				
            }

            if (n < 0) {
                printf("%s\n", "Errore nella lettura.");
                exit(0);
            }
            close(connfd);      // Chiusura della connessione.
            printf("%s\n", "Connessione terminata correttamente.");
            exit(0);
        }
    }
}




    // e la lunghezza della struttura dell'indirizzo.
    if(bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        err("Errore nel collegamento del socket");

    // listen() dice al socket di rimanere in ascolto in attesa di eventuali richieste di connessione
    // e inserisce le connessioni in arrivo in una coda.
    // Nel nostro caso abbiamo settato la dimensione massima della coda a 8.
    listen (listenfd, MAXQ);

    printf("%s\n","Il server è attivo e in attesa di nuove connessioni.");

    for ( ; ; ) {

        clilen = sizeof(cliaddr);
        // accept() inserisce le informazioni dell'indirizzo del client in connessione
        // nella struttura cliaddr la cui dimensione è data da clilen.
        // accept() ritorna un nuovo file descriptor per il socket collegato alla connessione in entrata.
        // In questo modo il file descriptor originale continua ad essere utilizzato per
        // rimanere in ascolto in caso di arrivo di altre connessioni.
        connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);
        printf("%s\n","Richiesta in arrivo!");

        if ( (childpid = fork ()) != 0 ) {    // Se fork() restituisce 0 significa che ha avuto successo e ha creato un processo figlio altrimenti c'e' stato un errore.
            printf("%s\n", "Disconnessione del processo figlio.");
            close(connfd);
        }else{
            printf ("%s\n","Processo figlio creato!");

            int n;      // Variabile per controllare il numero di byte ricevuti dal client.

            // Chiude il socket in ascolto.
            close (listenfd);
            ped p = {0, 0};
            pac ricevuto = {' ', 0};


            while (n = read(connfd, &ricevuto, sizeof(pac)) > 0) {      // read() ritorna la lunghezza del messaggio ricevuto dal client.
                movimento(&ricevuto, &p);		//la pedina viene spostata in base ai dati del pacchetto ricevuto dal client.
                printf("La pedina e' stata spostata di %d passi in direzione %c. La pedina ha ora coordinate (%d, %d)", ricevuto.passi, ricevuto.dir, p.x, p.y);
                write(connfd, &p, sizeof(ped));
            }

            if (n < 0) {
                printf("%s\n", "Errore nella lettura.");
                exit(0);
            }
            close(connfd);      // Chiusura della connessione.
            printf("%s\n", "Connessione terminata correttamente.");
            exit(0);
        }
    }
}



/*
            if (x == 'x')break;
            else if(x =='N') {
                char frase[MAXLINE]="*********************\n*Andando verso Nord!*\n*********************\n";
                write(connfd, frase, strlen(frase));
            }else if(x =='S'){
                char frase[MAXLINE]="*********************\n*Andando verso Sud!*\n*********************\n";
                write(connfd, frase, strlen(frase));
            }else if(x =='E'){
                char frase[MAXLINE]="*********************\n*Andando verso Est!*\n*********************\n";
                write(connfd, frase, strlen(frase));
            }else if(x =='O'){
                char frase[MAXLINE]="*********************\n*Andando verso Ovest!*\n*********************\n";
                write(connfd, frase, strlen(frase));
            }else{
                char frase[MAXLINE]="******************************************\n*Hai sbagliato opzione. Scegli di nuovo*\n******************************************\n";
                write(connfd, frase, strlen(frase));
            }
        }
            if (n < 0)
                printf("%s\n", "Read error");
            exit(0);
        }
        // Chiude il socket del server
        close(connfd);
    }
}
*/

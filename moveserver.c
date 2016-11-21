#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define MAXLINE 4096        //lunghezza massima di una linea comandi
#define SERV_PORT 3000      //numero di porta
#define MAXQ 8              //numero massimo di client in connesione


void err(const char* msg){
    perror (msg);
    exit(1);
}

int main (int argc, char **argv){
	int listenfd, connfd, n;
	pid_t childpid;         // Intero che rappresenta l'id del processo figlio.
	socklen_t clilen;
	char buf[MAXLINE];
	struct sockaddr_in cliaddr, servaddr;   // Strutture per l'indirizzo delle porte socket in IPv4.

    // Creazione di un socket con socket(int domain, int type, int protocol)
    // domain rappresenta la famiglia di protocollo (nel nostro caso IPv4), type specifica il tipo di socket
    // (nel nostro caso un socket stream per TCP), l'ultimo parametro intero definisce il protocollo usato ed è solitamente posto a 0
    // Se listenfd risulta < 0 c'è stato un errore nella creazione del socket.
    if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
        err("Errore nella creazione del socket!");
    }

    // Ripulisce la struttura dell'indirizzo del server.
    memset (&servaddr, 0, sizeof(servaddr));

    /* Preparazione dell'indirizzo del socket*/
    //Server byte order
    servaddr.sin_family = AF_INET;
    // Imposta automaticamente con l'indirizzo IP dell'host corrente.
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // Converte l'intero associato alla porta in network byte order.
    servaddr.sin_port = htons(SERV_PORT);

    // Collega il socket all' indirizzo IP corrente sulla porta SERV_PORT
    // bind() prende come argomenti il file descriptor per il socket, la struttura dell'indirizzo del server
    // e la lunghezza della struttura dell'indirizzo.
    bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

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

        if ( (childpid = fork ()) == 0 ) {    // Se fork() restituisce 0 significa che ha avuto successo e ha creato un processo figlio.

            printf ("%s\n","Processo figlio creato!");

            // Chiude il socket in ascolto.
            close (listenfd);

            while ( (n = recv(connfd, buf, MAXLINE,0)) > 0)  {      // recv() ritorna la lunghezza del messaggio ricevuto dal client.
                char begin[MAXLINE] = "Di quanto vuoi spostare il pedone? Premi N per una magia\n";
                write(connfd, begin, strlen(begin) );
                char x = buf[0];
                char *str = "Inserisci il nome dell'oggetto che vuoi muovere";

                send(connfd, str, n, 0);
                puts(buf);
                //Ovviamente da implementare un bel sistemino di else if. Pare che uno debba inserire due volte x per uscire...


                if (x == 'x')break;
                if(x =='N') {
                    char frase[MAXLINE]="Andando verso Nord!";
                write(connfd, frase, strlen(frase));
                }
                //Sta roba dovrebbe pulire il Buffer in modo tale da riazzerarlo ad ogni input da client
                buf[n]='\0';
                fflush(stdout);    // o stdin?
                buf[0]='\0';
                //printf("%s","String received from and resent to the client:");
                //puts(buf);
                //send(connfd, buf, n, 0);

            }

            if (n < 0)
                printf("%s\n", "Read error");
            exit(0);
        }
        // Chiude il socket del server
        close(connfd);
    }
}

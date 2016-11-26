 #include <unistd.h>
 #include <stdlib.h>
 #include <stdio.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <string.h>
 #include <arpa/inet.h>
 #include "pedina.h"
 #define MAXCOMMANDS 3		// Numero massimo di spostamenti impartibili alla volta.
 #define MAXLINE 4096		// Numero massimo di caratteri ricevuti dal server.
 #define MAXSEND 10
 #define SERV_PORT 3000      // Numero di porta.
 
 // Stampa un messaggio di errore ed esce.
 void err(const char* msg){
   perror (msg);
   exit(1);
 }
 
 
 
 int main(int argc, char **argv){
   
   int sockfd;			// File descriptor per il socket.
   struct sockaddr_in servaddr;       // Struttura per l'indirizzo del socket.  
   char dir;
   int mov;
   char compass[MAXCOMMANDS];		// Array per memorizzare le direzioni in cui muovere la pedina.
   char steps[MAXCOMMANDS];		// Array per memorizzare i passi da fare.
   int validInput = 0;		// Validita' dell'input da linea di comando.
   
 
   if(argc != 2 && argc != 3) err("Error: incorrect number of arguments!");
   if(argc == 3){
     int iLen = strlen(argv[2]);
     char *sInput = (char *)malloc((iLen+1) * sizeof(char));
     char *sSeparator = ",";
     strcpy(sInput, argv[2]);
     char *str;
     str = strtok(sInput, sSeparator);
     int i;
     for(i = 0; i < MAXSEND; i++){
       if(str == NULL) break;
       if(sscanf(str, "%c %d", &dir, &mov) == EOF)
           err("Movement input error!");
       else{
         if((dir == 'n' || dir == 's' || dir == 'e' || dir == 'o' || dir == 'N' || dir == 'S' || dir == 'E' || dir == 'O') && mov > 0){
           validInput = 1;
           compass[i] = dir;
           steps[i] = mov;
           printf("Requested movement: direction %c, %d steps\n");
         }else err("Error: invalid movement input!");
         }
         str = strtok(NULL, sSeparator);
     }
     if(str != NULL) printf("Max movement request exceeded, first %d will be processed.\n", MAXCOMMANDS);
   }
   
    
 
 
   // Creazione del socket client
   // Se sockfd < 0 c'è stato un errore nella creazione del socket
   if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Error creating socket!");
    exit(2);
   }
   /* Preparazione dell'indirizzo del socket*/
   memset(&servaddr, 0, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr= inet_addr(argv[1]);
   servaddr.sin_port =  htons(SERV_PORT);          // Converte in network byte order (big-endian)
   // Connessione del client al server
   if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
     perror("Problem in connecting to the server!");
     exit(3);
   }
 
   int n;                 // Numero di 
   pac pacForward;        // Pacchetto da inviare.
   ped p  = {0,0};
   
   if(validInput){
     int j;
     for(j = 0; steps[j] > 0; j++){
       riempi_pacchetto(&pacForward, compass[j], steps[j]);
       if(compass[j] != ' ')
         if ((send(sockfd, &pacForward, sizeof(pac),0) == -1)){
           err("Error sending packet.");
         }
       
       usleep(2000);
       compass[j] = ' ';        // Resetta l'array preparandolo per successivi movimenti
       n = 0;
       while(n!=sizeof(p)){
         n = read(sockfd, &p, sizeof(ped));
       }
     }
     printf("\n\n        In posizione:\n\n**************************************\n*(Asse ascisse: %d , Asse ordinate: %d)*\n**************************************\n\n", p.x, p.y);
     
   }
   
   
   for(;;){
     printf("\n\n        In posizione:\n\n**************************************\n*(Asse ascisse: %d , Asse ordinate: %d)*\n**************************************\n\n", p.x, p.y);
     printf("Scegli la direzione del percorso! Tra nord, sud, est e ovest. \n Se vuoi uscire premi x.\n\n");
     scanf("%c", &dir);
     while(dir!= 'n' && dir!= 's' && dir!= 'e' && dir!= 'o' && dir!='x' && dir!= 'N' && dir!= 'S' && dir!= 'E' && dir!= 'O' && dir!='X'){
       fflush(stdout);
       printf("EH NO!\n");
       fflush(stdout);
       scanf("%c", &dir);
     }
     if(dir == 'x' || dir == 'X'){
       printf("Ucita dal programma...\n"); break;
     }
     printf("Di quanti passi vuoi muoverti?\n");
     scanf("%d", &mov);
     riempi_pacchetto(&pacForward, dir, mov);
 
     // invio pacchetto
     if (dir != ' '){
       if ((send(sockfd, &pacForward, sizeof(pac),0) == -1)){
         printf("errore invio dati\n");
         exit(1);
       }
     }
     n=0;
     while(n!=sizeof(p)){
       n = read(sockfd, &p, sizeof(p));
     }
   }
 return 0;
 }
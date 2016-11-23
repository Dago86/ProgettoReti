
#ifndef PEDINA_H
#define PEDINA_H


// Il pedone vero e proprio
typedef struct pedina{
	int x;
	int y;
} ped;
typedef struct pacchetto{
	char dir;
	int passi;
} pac;
void movimento(struct pacchetto* pac,struct pedina* ped);



void riempi_pacchetto(struct pacchetto* pac, char d, int p);

#endif // PEDINA_H_INCLUDED

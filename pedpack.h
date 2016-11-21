#ifndef PEDPACK_H_
#define PEDPACK_H_

// Il pedone vero e proprio
typedef struct pedina{
	int x;
	int y;
} ped;

//Pacchetto da inviare al server
typedef struct pacchetto{
	char dir;
	int passi;
} pac;

//inserisce d e p nei rispettivi campi del pacchetto
void riempi_pacchetto(struct pacchetto* pa, char d, int p);

//muove il pedone
void movimento(struct pacchetto* pa, struct pedina* pe);

#endif

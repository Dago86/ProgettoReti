#ifndef PACCHETTO_HPP_INCLUDED
#define PACCHETTO_HPP_INCLUDED

//Pacchetto da inviare al server
typedef struct pacchetto{
	char dir;
	int passi;
} pac;

void riempi_pacchetto(struct pacchetto* pa, char d, int p);

#endif // PACCHETTO_HPP_INCLUDED

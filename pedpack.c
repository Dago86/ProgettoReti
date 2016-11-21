#include "pedpack.h"

void riempi_pacchetto(struct pacchetto* pa, char d, int p){
	pa->dir = d;
	pa->passi = p;
}

void movimento(pac* pa, ped* pe){
	if(pa->dir == 'a' || pa->dir == 'A')
		pe->y += pa->passi;
	else if(pa->dir == 'b' || pa->dir == 'B')
		pe->y -= pa->passi;
	else if(pa->dir == 'd' || pa->dir == 'D')
		pe->x += pa->passi;
	else if(pa->dir == 's' || pa->dir == 'S')
		pe->x -= pa->passi;
}

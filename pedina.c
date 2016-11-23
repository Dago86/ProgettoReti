#include "pedina.h"

void movimento(struct pacchetto* pac,struct pedina* ped){
    if(pac->dir == 'n' || pac->dir == 'N')
        ped->y += pac->passi;
    else if(pac->dir == 'o' || pac->dir == 'O')
        ped->y -= pac->passi;
    else if(pac->dir == 'e' || pac->dir == 'E')
        ped->x += pac->passi;
    else if(pac->dir == 's' || pac->dir == 'S')
        ped->x -= pac->passi;
}

void riempi_pacchetto(struct pacchetto* pac, char d, int p){
    pac->dir = d;
    pac->passi = p;
}

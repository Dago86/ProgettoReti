#include "pedina.hpp"

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

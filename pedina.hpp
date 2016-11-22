#ifndef PEDINA_HPP_INCLUDED
#define PEDINA_HPP_INCLUDED
#include "pacchetto.hpp"

// Il pedone vero e proprio
typedef struct pedina{
	int x;
	int y;
} ped;

void movimento(pac* pa, ped* pe);

#endif // PEDINA_HPP_INCLUDED

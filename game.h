#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <stdbool.h>
#include "structure.h"

#define MAX 60

void initBike();
int randomizer(int _min, int _max);
void collision();
void check_lives();

#endif
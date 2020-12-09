#ifndef STRUCTURE_H_INCLUDED
#define STRUCTURE_H_INCLUDED
#include <stdbool.h>

// structure du vélo du joueur
struct bike
{
    int x;                            // posistion x du vélo
    int y;                            // position y du vélo
    int score;                        // score du joueur
    int lives;                        // nombre de vies du joeur
    char* name;                       // pseudo du joueur
};
typedef struct bike bike;
bike b;

// structure des voitures ennemies
struct car
{
    int x;                            // position x de la voiture
    int y;                            // position y de la voiture
    int stuck;                        // état de la voiture, si != 0, la voiture est gélée sur place
    struct car *next;                 // pointeur vers la voiture suivante
};
typedef struct car car;

// structure des tirs du joueur
struct weapon
{
    int x;                            // position x du tir
    int y;                            // position y du tir
    bool bubble;                      // type de tir, si true le tire est une bulle qui gèle l'adversaire
    struct weapon *next;              // pointeur vers le tir suivant
};
typedef struct weapon weapon;

// structure des bandes de circulation
struct road
{
    int x;                            // position x de la bande de circulation
    int y;                            // position y de la bande de circulation
    struct road* next;                // pointeur vers la bande de circulation suivante
};
typedef struct road road;
road r[100];

weapon* add_weapon();
void delete_weapon(weapon* w_delete);
car* add_car();
void delete_car(car* c_delete);

#endif
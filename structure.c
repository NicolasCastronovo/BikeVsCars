#ifdef __APPLE_CC__
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"
#include "draw.h"
#include "structure.h"
#include "display.h"

extern int gridX, gridY;
extern bool bubble;
extern weapon* w;
extern car* c;
extern bool isCar;
extern int car_turn;
extern bool first_shot;

// fonction permettant d'ajouter un "tir" dans la liste chainée de type "weapon"
weapon* add_weapon()
{
    weapon* new_w = malloc(sizeof(weapon));
    new_w->x = b.x;
    new_w->y = b.y;
    new_w->bubble = bubble;
    new_w->next = NULL;
    return new_w;
}

// Fonction permettant de supprimer un élément de la liste chainée
void delete_weapon(weapon* w_delete)
{
    weapon* w_temp = w;
    weapon* w_prev;

    if(w_temp != NULL && w_temp == w_delete)
    {
        w = w_temp->next;
        w_temp = NULL;
        free(w_temp);
        return;
    }

    while(w_temp != NULL && w_temp != w_delete)
    {
        w_prev = w_temp;
        w_temp = w_temp->next;
    }

    if(w_temp == NULL) return;

    w_prev->next = w_temp->next;

    free(w_temp);
}

// fonction permettant d'ajouter une voiture dans la liste chainée de type "car"
car* add_car()
{
    car* new_c = malloc(sizeof(car));
    new_c->x = randomizer(2, gridX - 4);
    new_c->y = gridY - 1;
    new_c->next = NULL;
    new_c->stuck = 0;
    return new_c;
}

// fonction permettant de supprimer un élément de la liste chainée
void delete_car(car* c_delete)
{
   car* c_temp = c;
   car* c_prev;

    if(c_temp != NULL && c_temp == c_delete)
    {
        c = c_temp->next;
        c_temp = NULL;
        free(c_temp);
        return;
    }

    while(c_temp != NULL && c_temp != c_delete)
    {
        c_prev = c_temp;
        c_temp = c_temp->next;
    }

    if(c_temp == NULL) return;

    c_prev->next = c_temp->next;

    free(c_temp);
}


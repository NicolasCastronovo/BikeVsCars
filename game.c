#ifdef __APPLE_CC__
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "game.h"
#include "draw.h"
#include "structure.h"
#include "display.h"

// Coordonnées des bordures du jeux
extern int gridX, gridY;

// vrai si une voiture est en jeux
extern bool isCar;

// vrai si un rocher est en jeu
extern bool isRock;
extern bool isLife;
extern bool fire;
extern bool gameOver;

// coordonnées du rocher
extern int rockX, rockY;
extern int lifeX, lifeY;

// header et temporaire de la liste chainée des tires
extern weapon *w;
extern weapon *w_tmp;

// header et temporaire de la liste chainée des voitures
extern car *c;
extern car *c_tmp;
car *c_tmp_collision;

// Initialise le vélo (joueur)
void initBike()
{
    b.x = 15;
    b.y = 5;
    b.score = 0;
    b.lives = 3;
}

// fonction qui donne un entier au hazard entre deux chiffres
int randomizer(int _min, int _max)
{
    int x;
    srandom(time(NULL));
    do
    {
        x = random() % _max;
    }
    while(x <= _min);

    return x;
}

// fonction qui gère les collision et les sortie de terrain de toute les construction dans le jeu
void collision()
{
    // Rocher hors-map
    if(rockY < 1)
    {
        isRock = false;
    }

    // Collision vélo / rocher
    if(b.x >= rockX && b.x <= rockX + 4 && b.y == rockY)
    {
        isRock = false;
        b.lives--;
    }

    // Voiture hors-map
    if(c != NULL)
    {
        c_tmp_collision = c;
        do
        {
            if(c_tmp_collision->y == 0)
            {
                b.lives--;
            }

            c_tmp_collision = c_tmp_collision->next;

        } while(c_tmp_collision != NULL);

        // Voiture détruite
        if(w != NULL)
        {
            c_tmp_collision = c;
            while(c_tmp_collision != NULL)
            {
                w_tmp = w;
                while(w_tmp != NULL)
                {
                    // Supprimer les "balles" hors-map
                    if(w_tmp->y == gridY - 3)
                    {
                        delete_weapon(w_tmp);
                    }

                    if(c_tmp_collision->x == w_tmp->x && (c_tmp_collision->y == w_tmp->y || c_tmp_collision->y == w_tmp->y -1) && !w_tmp->bubble)
                    {
                        delete_car(c_tmp_collision);
                        delete_weapon(w_tmp);
                        isCar = false;
                        b.score += 100;
                    }
                    else if(c_tmp_collision->x == w_tmp->x && (c_tmp_collision->y == w_tmp->y || c_tmp_collision->y == w_tmp->y -1) && w_tmp->bubble)
                    {
                        c_tmp_collision->stuck = 30;
                        b.score += 200;
                    }
                    w_tmp = w_tmp->next;
                }
                c_tmp_collision = c_tmp_collision->next;
            }
        }

        // Collision voiture / vélo
        c_tmp_collision = c;
        if(c_tmp_collision != NULL)
        {
            do
            {
                if(c_tmp_collision->x == b.x && c_tmp_collision->y == b.y)
                {
                    delete_car(c_tmp_collision);
                    isCar = false;
                    b.lives--;
                }

                c_tmp_collision = c_tmp_collision->next;
            }
            while(c_tmp_collision != NULL);
        }

        while(b.x <= 1)
        {
            b.x++;
        }

        while(b.x >= gridX - 2)
        {
            b.x--;
        }
    }

    if(b.x == lifeX && b.y == lifeY)
    {
        isLife = false;
        b.score += 500;

        if(b.lives < 3)
        {
            b.lives++;
        }
    }

    if(lifeX < 0)
    {
        isLife = false;
    }
}

// vérifie le nombre des vies du joueur
void check_lives()
{
    if(b.lives <= 0)
    {
        gameOver = true;
    }
}
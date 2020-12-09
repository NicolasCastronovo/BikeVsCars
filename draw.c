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

int gridX, gridY;
bool isCar = false;
bool isRock = false;
bool isRoad = false;
bool isLife = false;

// vrai si le premier tire à été tiré
bool first_shot = true;

// coordonnées du rocher et booléen qui décide du nombre de tours restant avant le prochain rocher
int rockX, rockY, rock_time = 0;
int lifeX, lifeY, life_time = 50;
extern bool fire;
extern bool gameOver;

// Enregistre le nombre de tours et de bandes de circulation sur le terrain
int road_i = 0;
int road_turn = 0;
extern bool new_shot;

// Enregistre le nombre de tour pour décider d'envoyer une nouvelle voiture
int car_turn = 0;

weapon *w;
weapon *w_tmp;
weapon *w_i;

car *c;
car *c_tmp;

int high[10];

FILE *scores;

extern int tmp;

extern bool bubble;

// initialisation des bordures de terrain
void initGrid(int x, int y)
{
    gridX = x;
    gridY = y;
}

// dessin des bordures de terrain
void drawGrid()
{
    for(int x = 0; x < gridX; x++)
    {
        for(int y = 0; y < gridY; y++)
        {
            square(x, y);
        }
    }
}

// dessin du plateau de jeu
void square(int x, int y)
{
    if((x == 0 || x == gridX - 1) && (y != gridY - 1) && (y != gridY - 2))
    {
        glColor3f(1.0, 1.0, 1.0);
        glRectd(x, y, x+1, y+1);
    }
    else if(y == gridY - 1 || y == gridY - 2)
    {
        glColor3f(0.0, 0.0, 0.0);
        glRectd(x, y, x+1, y+1);
    }
    else
    {
        glLineWidth(1.0);
        glColor3f(0.4, 0.4, 0.4);
    }

    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x+1, y);
        glVertex2f(x+1, y+1);
        glVertex2f(x, y+1);
    
    glEnd();
}

// dessin des voitures ennemies
void drawCar()
{
    if(car_turn % 30 == 0 || (car_turn > 300 && car_turn % 15 == 0))
    {
        c_tmp = add_car();

        if(!isCar)
        {
            c = c_tmp;
            isCar = true;
        }
        else if(isCar)
        {
            car *t = c;
            while(t->next != NULL)
            {
                t = t->next;
            }
            t->next = c_tmp;
        }
    }

    c_tmp = c;
    if(c_tmp != 0)
    {
        do
        {
            glColor3f(0.0, 0.0, 1.0);
            glRectd(c_tmp->x, c_tmp->y, c_tmp->x + 1 ,c_tmp->y + 1);
            if(c_tmp->stuck != 0)
            {
                c_tmp->stuck--;
            }
            else
            {
                c_tmp->y--;
            }

            c_tmp = c_tmp->next;
        }
        while(c_tmp != NULL);
    }
    car_turn++;
}

// dessin du vélo du joueur
void drawBike()
{
    glColor3f(1.0, 1.0, 0.0);
    glRectd(b.x, b.y, b.x+1, b.y+1);
}

// dessin des tirs du vélo
void drawWeapon()
{
    if(fire)
    {
        if(new_shot)
        {
            w_tmp = add_weapon();

            if(first_shot || w == NULL)
            {
                w = w_tmp;
                first_shot = false;
            }
            else
            {
                weapon *k = w;
                while(k->next != NULL)
                {
                    k = k->next;
                }
                k->next = w_tmp;
            }
            new_shot = false;
        }

        w_i = w;
        while(w_i != NULL)
        {
            if(!w_i->bubble)
            {
                glColor3f(1.0, 0.0, 0.0);
            }
            else
            {
                glColor3f(0.0, 0.5, 1.0);
            }
            
            glRectd(w_i->x, w_i->y, w_i->x + 1, w_i->y + 1);

            w_i->y++;

            w_i = w_i->next;
        }
    }
}

// dessin des rochers sur la route
void drawRock()
{
    if(rock_time % 100 == 0 && rock_time != 0)    
    {
        if(isRock == false)
        {
            rockY = gridY - 2;
            rockX = randomizer(2, gridX);
            glColor3f(0.0, 0.0, 0.0);
            glRectd(rockX, rockY, rockX+5, rockY+2);
            isRock = true;
        }
    }
    
    if(isRock)
    {
        rockY -= 3;
        glColor3f(0.0, 0.0, 0.0);
        glRectd(rockX, rockY, rockX+5, rockY+2);
    }
    rock_time++;
}

// dessin du "coeur" qui rend une vie
void drawLife()
{
    if(life_time % 220 == 0 && life_time != 0)    
    {
        if(isLife == false)
        {
            lifeY = gridY - 2;
            lifeX = randomizer(2, gridX);
            glColor3f(1.0, 0.7, 0.8);
            glRectd(lifeX, lifeY, lifeX+1, lifeY+1);
            isLife = true;
        }
    }
    
    if(isLife)
    {
        lifeY -= 3;
        glColor3f(1.0, 0.7, 0.8);
        glRectd(lifeX, lifeY, lifeX+1, lifeY+1);
    }
    life_time++;
}

// dessin des bandes de circulation
void drawRoad()
{
    if(road_turn == 5 || road_turn == 0)
    {
        r[road_i].x = gridX / 2;
        r[road_i].y = gridY - 7;
        road_i++;
        road_turn = 0;
    }

    for(int j = 0; j < road_i; j++)
    {
        glColor3f(1.0, 1.0, 1.0);
        glRectd(r[j].x, r[j].y, r[j].x + 1, r[j].y + 5);
        r[j].y -= 3;
    }

    if(road_i == 99)
    {
        road_i = 0;
    }

    road_turn++;
}

// dessin du menu principal
void drawMenu()
{
    char *a = "----- BIKE VS CARS -----";
    glRasterPos3f(9.0f, 35.0f, 0.0f);
    glColor3f(1.0, 1.0, 1.0);
    for(int i = 0; a[i] != '\0'; i++)
    {
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , a[i]);
    }

    char *b = "BARRE D'ESPACE POUR JOUER";
    glRasterPos3f(2.0f, 30.0f, 0.0f);
    glColor3f(1.0, 1.0, 1.0);
    for(int i = 0; b[i] != '\0'; i++)
    {
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , b[i]);
    }

    char *z = "'c' POUR LE MENU DES COMMANDES";
    glRasterPos3f(2.0f, 25.0f, 0.0f);
    glColor3f(1.0, 1.0, 1.0);
    for(int i = 0; z[i] != '\0'; i++)
    {
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , z[i]);
    }

    char *y = "'h' POUR LE MENU DES HIGHSCORES";
    glRasterPos3f(2.0f, 20.0f, 0.0f);
    glColor3f(1.0, 1.0, 1.0);
    for(int i = 0; y[i] != '\0'; i++)
    {
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , y[i]);
    }

    char *c = "'x' POUR QUITTER";
    glRasterPos3f(2.0f, 15.0f, 0.0f);
    glColor3f(1.0, 1.0, 1.0);
    for(int i = 0; c[i] != '\0'; i++)
    {
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , c[i]);
    }
}

// dessin des stats du joueur au dessus de l'écran
void drawStats()
{
    if(b.lives == 3)
    {
        char *d = "LIVES: <3 <3 <3";
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos3f(1.0f, 39.0f, 0.0f);
        for(int i = 0; d[i] != '\0'; i++)
        {
            glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , d[i]);
        }
    }
    else if(b.lives == 2)
    {
        char *d = "LIVES: <3 <3";
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos3f(1.0f, 39.0f, 0.0f);
        for(int i = 0; d[i] != '\0'; i++)
        {
            glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , d[i]);
        }
    }
    else
    {
        char *d = "LIVES: <3";
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos3f(1.0f, 39.0f, 0.0f);
        for(int i = 0; d[i] != '\0'; i++)
        {
            glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , d[i]);
        }
    }

    char score[50];
    sprintf(score, "SCORE: %i", b.score);
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos3f(19.0f, 39.0f, 0.0f);
    for(int i = 0; score[i] != '\0'; i++)
    {
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , score[i]);
    }
}

// dessin du menu des commandes
void drawCommands()
{
    char *e = "z : deplacement haut";
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos3f(5.0f, 35.0f, 0.0f);
    for(int i = 0; e[i] != '\0'; i++)
    {
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , e[i]);
    }

    char *f = "s : deplacement bas";
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos3f(5.0f, 33.0f, 0.0f);
    for(int i = 0; f[i] != '\0'; i++)
    {
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , f[i]);
    }

    char *g = "d : deplacement droite";
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos3f(5.0f, 31.0f, 0.0f);
    for(int i = 0; g[i] != '\0'; i++)
    {
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , g[i]);
    }

    char *h = "q : deplacement gauche";
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos3f(5.0f, 29.0f, 0.0f);
    for(int i = 0; h[i] != '\0'; i++)
    {
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , h[i]);
    }

    char *k = "o : tir classique";
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos3f(5.0f, 27.0f, 0.0f);
    for(int i = 0; k[i] != '\0'; i++)
    {
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , k[i]);
    }

    char *l = "p : tir immobilisant";
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos3f(5.0f, 25.0f, 0.0f);
    for(int i = 0; l[i] != '\0'; i++)
    {
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , l[i]);
    }

    char *m = "x : retour au menu principal";
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos3f(5.0f, 23.0f, 0.0f);
    for(int i = 0; m[i] != '\0'; i++)
    {
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , m[i]);
    }
}

// dessin du menu des highscores
void drawHighscores()
{
    char top[100];
    for(int i = 0; i < 10; i++)
    {
        sprintf(top, "%i\n", high[i]);
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos3f(15.0f, 35.0f-i+2, 0.0f);
        for(int j = 0; top[j] != '\0'; j++)
        {
            glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , top[j]);
        }
    }
}

// fonction qui lit le fichier score.txt et scan son contenu dans high[]
void readHighscores()
{
    scores = fopen("scores.txt", "r");
    for(int i = 0; i < 10; i++)
    {
        fscanf(scores, "%i", &high[i]);
    }
    fclose(scores);
}

// fonction qui écrit le contenu de high[] dans le fichier texte en fin de partie
void writeHighscores()
{
    scores = fopen("scores.txt", "w");
    for(int i = 0; i < 10; i++)
    {
	    fprintf(scores,"%i\n",high[i]);     
    }
    fclose(scores);

}

// fonction qui vérifié si un nouveau score est dans le top 10
void newHighscore()
{
    sortHighscores();

    for(int i = 0; i < 10; i++)
    {
        if(b.score > high[i])
        {
            high[i] = b.score;
            for(int j = i + 1; j < 9 - i; j++)
            {
                high[j] = high[j + 1];
            }
            break;
        }
    }
}

// fonction qui trie les scores par ordre décroissant
void sortHighscores()
{
    for(int i = 0; i < 10; i++)
    {
        for(int j = i + 1; j < 10; j++)
        {
            if(high[i] < high[j])
            {
                tmp = high[i];
                high[i] = high[j];
                high[j] = tmp;
            }
        }
    }
}
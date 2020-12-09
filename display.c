#ifdef __APPLE_CC__
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdbool.h>
#include <stdio.h>
#include "game.h"
#include "draw.h"
#include "structure.h"
#include "display.h"

// permet de changer facilement le nombre de colonnes, de rangées et la vitesse du jeu
#define COLUMNS 30
#define ROWS 40
#define FPS 10

// est vrai si le joueur a perdu la partie
bool gameOver = false;

// enregistre le premier tir du joueur 
bool fire = false;

// enregistre si le tir est un tire bulle ou non
bool bubble;

// enregistre chaque nouveau tir (appui sur touche)
bool new_shot = false;

// fichier dans lequel vont être sauvegarder les scores
FILE *scores;

extern bool isCar;
extern bool car_turn;
extern bool rock_time;

// variable temporaire utiliée pour les highscores
int tmp;

// tableau qui contient les highscore
extern int high[10];

// Initialisation de la fenêtre
void init()
{
    glClearColor(0.4, 0.4, 0.4, 1.0);
    initGrid(COLUMNS, ROWS);
}

// Ce que le programme doit dessiner à chaque nouvelle frame
void display_game()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();
    drawRoad();
    drawWeapon();
    drawBike();
    drawRock();
    drawLife();
    drawCar();
    drawStats();
    collision();
    check_lives();
    glutSwapBuffers();
    if(gameOver == true)
    {
        newHighscore();
        writeHighscores();
        glutDisplayFunc(display_menu);
        glutKeyboardFunc(keyboard_menu);
    }
}

// loop qui display le menu principal
void display_menu()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawMenu();
    glutSwapBuffers();
}

// loop qui displau le menu des highscore
void display_highscores()
{
    glClear(GL_COLOR_BUFFER_BIT);
    readHighscores();
    drawHighscores();
    glutSwapBuffers();
}

// loop qui display le menu des commandes
void display_commands()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawCommands();
    glutSwapBuffers();
}

// fonction opengl
void reshape_callback(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, COLUMNS, 0.0, ROWS, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

// fonction opengl
void timer_callback(int x)
{
    glutPostRedisplay();
    glutTimerFunc(1000/FPS, timer_callback, 0);
}

// fonction keyboard qui s'occupe des touches utilisées par le joueur dans le menu principal
void keyboard_menu(unsigned char key, int x, int y)
{
    switch(key)
    {
        case ' ':
            gameOver = false;
            isCar = false;
            fire = false;
            new_shot = false;
            car_turn = 0;
            rock_time = 0;
            initBike();
            glutDisplayFunc(display_game);
            glutKeyboardFunc(keyboard_game);
            break;
        
        case 'c':
            glutDisplayFunc(display_commands);
            glutKeyboardFunc(keyboard_submenu);
            break;

        case 'h':
            glutDisplayFunc(display_highscores);
            glutKeyboardFunc(keyboard_submenu);
            break;
        
        case 'x':
            exit(0);
            break;
    } 
}

// Fonction qui enregistre et fait des actions en fonction des touches appuyées à n'importe quel moment.
void keyboard_game(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'z':
            b.y++;
            break;

        case 's':
            b.y--;
            break;
        
        case 'd':
            b.x++;
            break;

        case 'q':
            b.x--;
            break;

        case'o':
            fire = true;
            new_shot = true;
            bubble = false;
            break;

        case 'p':
            fire = true;
            new_shot = true;
            bubble = true;
            break;
        
        case'x':
            gameOver = true;
            break;
    }    
}

// fonction keyboard qui s'occupe des touches utilisées par le joueur dans les sous menus
void keyboard_submenu(unsigned char key, int x, int y)
{
    switch(key)
    {
        case'x':
            glutDisplayFunc(display_menu);
            glutKeyboardFunc(keyboard_menu);
            break;
    }    
}
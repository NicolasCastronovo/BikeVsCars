/* 
 * --- Bike vs Cars ---
 *
 * --- Nicolas Castronovo ---
 * 
 */

#ifdef __APPLE_CC__
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdbool.h>
#include "game.h"
#include "draw.h"
#include "structure.h"
#include "display.h"

// Fonction main
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(500, 500);
    glutInitWindowSize(600, 800);
    glutCreateWindow("BikevsCars");
    glutDisplayFunc(display_menu);
    glutReshapeFunc(reshape_callback);
    glutTimerFunc(0, timer_callback, 0);
    glutKeyboardFunc(keyboard_menu);
    initBike();
    init();
    glutMainLoop();
    return 0;
}

#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

void init();
void display_game();
void display_menu();
void display_highscores();
void display_commands();
void reshape_callback(int w, int x);
void timer_callback(int x);
void keyboard_game(unsigned char key, int x, int y);
void keyboard_menu(unsigned char key, int x, int y);
void keyboard_submenu(unsigned char key, int x, int y);

#endif
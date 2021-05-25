#ifndef __SNAKES_H__
#define __SNAKES_H__
/*******************************************************************
                            ||| SNAKES |||
                      Petr Stádník & Josef Rastočný 
                                2021
 *******************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include "utils.h"

typedef struct
{
   int x;
   int y;
} spot;

typedef struct
{
   bool is_alive;
   int len;
   char direction; 
   spot *body;
} snake;


unsigned short *snakes_init(int w, int h, snake *A, snake *B);
void snakes_cleanup(snake *A, snake *B);

void draw_walls(unsigned short color);
void clean_grid();

int snakes_to_grid(snake *A, snake *B, unsigned short A_color, unsigned short B_color);
void add_chunk(int x, int y, snake *snake);
int move(char direction, snake *snake, bool grow);
void snakes_restart(int w, int h, snake *A, snake *B);

#endif
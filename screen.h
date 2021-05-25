#ifndef __SCREEN_H__
#define __SCREEN_H__

#define M_PI 3.1415
#define WAIT 0.2

#define GRID_SIZE 320 * 480
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>


#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "serialize_lock.h"
#include "font_types.h"




void init_screen(unsigned short *snakes_grid);
unsigned int hsv2rgb_lcd(int hue, int saturation, int value);
void draw_pixel_big(int x, int y, unsigned short color);
void draw_pixel(int x, int y, unsigned short color);
void draw_char(int x, int y, char ch, unsigned short color);

void refresh_screen();
void clean_screen();
#endif
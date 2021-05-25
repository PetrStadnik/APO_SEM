#ifndef __LED_LINE_H__
#define __LED_LINE_H__
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


void led_init(void);

void line_add();
void line_off();

void line(void);
void led_green(char led);
void led_red(char led);
void led_blue(char led);
void led_off(char led);

unsigned int hsv2rgb(int hue, int saturation, int value);
uint32_t my_pow(uint32_t z, uint32_t m);
#endif
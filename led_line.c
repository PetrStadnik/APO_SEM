#include "led_line.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

unsigned char *mem_base;
uint32_t line_stat = 1;
uint32_t val_line = 0;

void led_init()
{
    /*
   * Setup memory mapping which provides access to the peripheral
   * registers region of RGB LEDs, knobs and line of yellow LEDs.
   */
    mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);

    /* If mapping fails exit with error code */
    if (mem_base == NULL)
        exit(1);
}

void line_add()
{
    val_line += my_pow(2, line_stat);
    *(volatile uint32_t *)(mem_base + SPILED_REG_LED_LINE_o) = val_line;
    line_stat++;
}

void line_off()
{
    val_line = 0;
    line_stat = 1;
    *(volatile uint32_t *)(mem_base + SPILED_REG_LED_LINE_o) = val_line;
}

void led_green(char led)
{
    uint32_t col = hsv2rgb(114, 255, 200);
    if (led == 0)
    {
        *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = col;
    }
    else if (led == 1)
    {
        *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = col;
    }
}
void led_red(char led)
{
    uint32_t col = hsv2rgb(360, 250, 255);
    if (led == 0)
    {
        *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = col;
    }
    else if (led == 1)
    {
        *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = col;
    }
}
void led_blue(char led)
{
    uint32_t col = hsv2rgb(230, 255, 255);
    if (led == 0)
    {
        *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = col;
    }
    else if (led == 1)
    {
        *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = col;
    }
}
void led_off(char led)
{
    if (led == 0)
    {
        *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = 0;
    }
    else if (led == 1)
    {
        *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = 0;
    }
}

unsigned int hsv2rgb(int hue, int saturation, int value)
{
    hue = (hue % 360);
    float f = ((hue % 60) / 60.0);
    int p = (value * (255 - saturation)) / 255;
    int q = (value * (255 - (saturation * f))) / 255;
    int t = (value * (255 - (saturation * (1.0 - f)))) / 255;
    unsigned int r, g, b;
    if (hue < 60)
    {
        r = value;
        g = t;
        b = p;
    }
    else if (hue < 120)
    {
        r = q;
        g = value;
        b = p;
    }
    else if (hue < 180)
    {
        r = p;
        g = value;
        b = t;
    }
    else if (hue < 240)
    {
        r = p;
        g = q;
        b = value;
    }
    else if (hue < 300)
    {
        r = t;
        g = p;
        b = value;
    }
    else
    {
        r = value;
        g = p;
        b = q;
    }
    return ((r << 16) | (g << 8) | b);
}

uint32_t my_pow(uint32_t z, uint32_t m)
{
    uint32_t ret = 1;
    for (int i = 0; i < m; i++)
    {
        ret = ret * z;
    }
    return ret;
}

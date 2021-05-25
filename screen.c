#include "screen.h"

unsigned char *parlcd_mem_base;
font_descriptor_t *fdes;
int scale = 4;
unsigned short *screen_grid;
void init_screen(unsigned short *snakes_grid)
{
    parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    if (parlcd_mem_base == NULL)
    {
        exit(1);
    }

    parlcd_hx8357_init(parlcd_mem_base);

    fdes = &font_winFreeSystem14x16;
    screen_grid = snakes_grid;
}

void draw_pixel(int x, int y, unsigned short color)
{
    if (x >= 0 && x < 480 && y >= 0 && y < 320)
    {
        screen_grid[x + 480 * y] = color;
    }
}

void draw_pixel_big(int x, int y, unsigned short color)
{
    int i, j;
    for (i = 0; i < scale; i++)
    {
        for (j = 0; j < scale; j++)
        {
            draw_pixel(x + i, y + j, color);
        }
    }
}

int char_width(int ch)
{
    int width;
    if (!fdes->width)
    {
        width = fdes->maxwidth;
    }
    else
    {
        width = fdes->width[ch - fdes->firstchar];
    }
    return width;
}

void draw_char(int x, int y, char ch, unsigned short color)
{
    int w = char_width(ch);
    const font_bits_t *ptr;
    if ((ch >= fdes->firstchar) && (ch - fdes->firstchar < fdes->size))
    {
        if (fdes->offset)
        {
            ptr = &fdes->bits[fdes->offset[ch - fdes->firstchar]];
        }
        else
        {
            int bw = (fdes->maxwidth + 15) / 16;
            ptr = &fdes->bits[(ch - fdes->firstchar) * bw * fdes->height];
        }
        int i, j;
        for (i = 0; i < fdes->height; i++)
        {
            font_bits_t val = *ptr;
            for (j = 0; j < w; j++)
            {
                if ((val & 0x8000) != 0)
                {
                    draw_pixel_big(x + scale * j, y + scale * i, color);
                }
                val <<= 1;
            }
            ptr++;
        }
    }
}

unsigned int hsv2rgb_lcd(int hue, int saturation, int value)
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
    r >>= 3;
    g >>= 2;
    b >>= 3;
    return (((r & 0x1f) << 11) | ((g & 0x3f) << 5) | (b & 0x1f));
}

void refresh_screen()
{
    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    for (int i = 0; i < GRID_SIZE; i++)
    {
        parlcd_write_data(parlcd_mem_base, screen_grid[i]);
        /*  if (screen_grid[i] > 0)
    {
      printf("x");
    }
    else
    {
      printf(" ");
    }
    if (i % SCREEN_WIDTH == 0)
    {
      printf("\n");
    } */
    }
}

void clean_screen()
{
    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    for (int i = 0; i < GRID_SIZE; i++)
    {
        screen_grid[i] = 0;
        parlcd_write_data(parlcd_mem_base, 0);
    }
}
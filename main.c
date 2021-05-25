#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <termios.h>
#include <unistd.h>

#include <pthread.h>

#include "utils.h"
#include "snakes.h"
#include "screen.h"
#include "led_line.h"

#define GRID_SIZE 320 * 480
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

typedef struct
{
    char c;
    bool quit;
} data;

data shared_data = {.c = 0, .quit = false};

pthread_mutex_t mtx;
pthread_cond_t cond;

void *input_thread(void *);
void write_menu(unsigned short color_1, unsigned short color_2);

// - main ---------------------------------------------------------------------
int main(int argc, char *argv[])
{
    pthread_t thread_id;
    pthread_mutex_init(&mtx, NULL); // initialize mutex with default attributes
    pthread_cond_init(&cond, NULL); // initialize mutex with default attributes

    int r = pthread_create(&thread_id, NULL, input_thread, NULL);
    fprintf(stderr, "INFO: Create input thread %s\n", (r == 0 ? "OK" : "FAIL"));

    snake *A = my_alloc(sizeof(snake));
    snake *B = my_alloc(sizeof(snake));
    call_termios(0);
    unsigned short *snakes_grid = snakes_init(SCREEN_WIDTH, SCREEN_HEIGHT, A, B);
    if (snakes_grid != NULL)
    {
        init_screen(snakes_grid);
    }
    else
    {
        error("Init grid failed");
    }
    led_init();

    //define colours-----------------------------------------
    unsigned short red_color = hsv2rgb_lcd(360, 250, 255);
    unsigned short blue_color = hsv2rgb_lcd(230, 255, 255);
    unsigned short green_color = hsv2rgb_lcd(114, 255, 200);
    unsigned short yellow_color = hsv2rgb_lcd(55, 255, 240);
    unsigned short white_color = hsv2rgb_lcd(0, 0, 255);
    //--------------------------------------------------------

    clean_screen();
    int y = 80;
    draw_char(120, y, 'S', green_color);
    draw_char(160, y, 'N', blue_color);
    draw_char(200, y, 'A', red_color);
    draw_char(240, y, 'K', yellow_color);
    draw_char(280, y, 'E', white_color);
    draw_char(320, y, 'S', green_color);
    led_green(0);
    led_green(1);
    refresh_screen();
    sleep(3);
    bool exit_game = false;
    int winner;
    char c;
    bool end = false;
    int add_line_counter;
    int sleep_time_ms = 400;
    while (!exit_game)
    {
        line_off();
        add_line_counter = 1;
        bool exit_menu = false;
        end = false;
        clean_screen();
        write_menu(green_color, white_color);
        refresh_screen();
        while (!exit_menu)
        {
            led_off(0);
            led_off(1);
            pthread_mutex_lock(&mtx);
            c = shared_data.c;
            pthread_mutex_unlock(&mtx);
            switch (c)
            {
            case 'n':
                sleep_time_ms = 400;
                exit_menu = true;
                led_red(0);
                led_blue(1);
                break;
            case 'f':
                sleep_time_ms = 200;
                exit_menu = true;
                led_red(0);
                led_blue(1);
                break;
            case 'e':
                exit_game = true;
                exit_menu = true;
                end = true;
                pthread_mutex_lock(&mtx);
                shared_data.quit = true;
                pthread_mutex_unlock(&mtx);
            default:
                break;
            }
        }

        clean_screen();
        draw_walls(yellow_color);

        while (!end)
        {
            pthread_mutex_lock(&mtx);
            c = shared_data.c;
            pthread_mutex_unlock(&mtx);

            switch (c)
            {
            case 'a':
                if (A->direction < 3)
                {
                    A->direction++;
                }
                else
                {
                    A->direction = 0;
                }
                break;
            case 's':
                if (A->direction > 0)
                {
                    A->direction--;
                }
                else
                {
                    A->direction = 3;
                }
                break;
            case 'k':
                if (B->direction < 3)
                {
                    B->direction++;
                }
                else
                {
                    B->direction = 0;
                }
                break;
            case 'l':
                if (B->direction > 0)
                {
                    B->direction--;
                }
                else
                {
                    B->direction = 3;
                }
                break;
            case 'q':
                end = true;
                break;
            case 'p': // pause game
                draw_char(140, y, 'P', yellow_color);
                draw_char(180, y, 'A', yellow_color);
                draw_char(220, y, 'U', yellow_color);
                draw_char(260, y, 'S', yellow_color);
                draw_char(300, y, 'E', yellow_color);
                refresh_screen();
                pthread_mutex_lock(&mtx);
                shared_data.c = 0;
                pthread_mutex_unlock(&mtx);
                bool end_pause = false;
                while (!end_pause)
                {
                    pthread_mutex_lock(&mtx);
                    if (shared_data.c == 'p')
                    {
                        end_pause = true;
                    }
                    pthread_mutex_unlock(&mtx);
                    usleep(500 * 1000);
                }
                clean_screen();
                draw_walls(yellow_color);
                break;

            default:

                break;
            }
            pthread_mutex_lock(&mtx);
            shared_data.c = 0;
            pthread_mutex_unlock(&mtx);

            move(A->direction, A, true);
            move(B->direction, B, true);
            add_line_counter++;
            if(add_line_counter == 20) 
            {
                line_add();
                add_line_counter = 1;
            }
            winner = snakes_to_grid(A, B, red_color, blue_color);
            if (winner != 0)
            {
                end = true;
            }
            refresh_screen();
            usleep(sleep_time_ms * 1000);
        }
        clean_screen();
        if (!exit_game)
        {
            if (winner == 1)
            {
                led_blue(0);
                draw_char(80, y, 'B', blue_color);
                draw_char(120, y, 'L', blue_color);
                draw_char(160, y, 'U', blue_color);
                draw_char(200, y, 'E', blue_color);

                draw_char(280, y, 'W', blue_color);
                draw_char(330, y, 'O', blue_color);
                draw_char(370, y, 'N', blue_color);
            }
            else
            {
                led_red(1);
                draw_char(100, y, 'R', red_color);
                draw_char(140, y, 'E', red_color);
                draw_char(180, y, 'D', red_color);

                draw_char(240, y, 'W', red_color);
                draw_char(300, y, 'O', red_color);
                draw_char(340, y, 'N', red_color);
            }
            refresh_screen();
            snakes_restart(SCREEN_WIDTH, SCREEN_HEIGHT, A, B);
            sleep(3);
            winner = 0;
            end = false;
        }
    }
    r = pthread_join(thread_id, NULL);
    fprintf(stderr, "INFO: Joining the game thread has been %s\n", (r == 0 ? "OK" : "FAIL"));
    snakes_cleanup(A, B);
    call_termios(1);
    free(A);
    free(B);

    return EXIT_SUCCESS;
}

int ex;
void *input_thread(void *arg)
{
    char c;
    bool end = false;
    while (!end)
    {
        c = getchar();
        pthread_mutex_lock(&mtx);
        shared_data.c = c;
        end = shared_data.quit;
        pthread_mutex_unlock(&mtx);
        if (c == 'e')
        {
            end = true;
        }
    }

    return NULL;
}

void write_menu(unsigned short color_1, unsigned short color_2)
{
    draw_char(160, 10, 'M', color_1);
    draw_char(200, 10, 'E', color_1);
    draw_char(240, 10, 'N', color_1);
    draw_char(280, 10, 'U', color_1);

    draw_char(60, 80, 'N', color_2);
    draw_char(100, 80, '-', color_2);
    draw_char(140, 80, 'P', color_2);
    draw_char(180, 80, 'L', color_2);
    draw_char(220, 80, 'A', color_2);
    draw_char(260, 80, 'Y', color_2);

    draw_char(60, 130, 'F', color_2);
    draw_char(100, 130, '-', color_2);
    draw_char(140, 130, 'F', color_2);
    draw_char(180, 130, 'A', color_2);
    draw_char(220, 130, 'S', color_2);
    draw_char(260, 130, 'T', color_2);

    draw_char(60, 180, 'E', color_2);
    draw_char(100, 180, '-', color_2);
    draw_char(140, 180, 'E', color_2);
    draw_char(180, 180, 'X', color_2);
    draw_char(220, 180, 'I', color_2);
    draw_char(260, 180, 'T', color_2);
}

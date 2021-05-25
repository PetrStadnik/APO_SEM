#include "snakes.h"

#define MAX_SNAKE_LEN 320 * 480

#define GRID_SIZE 320 * 480
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

unsigned short *grid;

unsigned short *snakes_init(int w, int h, snake *A, snake *B)
{
  grid = my_alloc(w * h * 2);
  A->len = B->len = 0;

  A->body = my_alloc(sizeof(spot) * MAX_SNAKE_LEN);
  add_chunk(9, h - 20, A);
  A->direction = 2;

  B->body = my_alloc(sizeof(spot) * MAX_SNAKE_LEN);
  add_chunk(w - 9, h - 20, B);
  B->direction = 2;

  return grid;
}

void snakes_cleanup(snake *A, snake *B)
{
  free(grid);
  grid = NULL;
  free(A->body);
  A->body = NULL;
  free(B->body);
  B->body = NULL;
}

void draw_walls(unsigned short color)
{
  for (int i = 0; i < GRID_SIZE; i++)
  {
    if (i < SCREEN_WIDTH || i > GRID_SIZE - SCREEN_WIDTH || i % SCREEN_WIDTH == 0 || (i + 2) % SCREEN_WIDTH == 0)
    {
      grid[i] = color;
    }
  }
}

void add_chunk(int x, int y, snake *snake)
{
  spot s = {x - 5, y - 5};
  for (int i = 0; i < 121; i++)
  {
    snake->body[121 * snake->len + i] = s;
    s.x++;
    if ((i + 1) % 11 == 0)
    {
      s.x = x - 5;
      s.y++;
    }
  }
  snake->len++;
}

int move(char direction, snake *snake, bool grow)
{
  if (grow)
  {
    snake->len += 1;
  }

  for (int i = snake->len - 1; i > 0; i--)
  {
    for (int k = 0; k < 121; k++)
    {
      snake->body[i * 121 + k] = snake->body[(i - 1) * 121 + k];
    }
  }
  switch (direction)
  {
  case 0:
    for (int k = 0; k < 121; k++)
    {
      snake->body[k].y += 11;
    }
    break;
  case 1:
    for (int k = 0; k < 121; k++)
    {
      snake->body[k].x += 11;
    }
    break;
  case 2:
    for (int k = 0; k < 121; k++)
    {
      snake->body[k].y -= 11;
    }
    break;
  case 3:
    for (int k = 0; k < 121; k++)
    {
      snake->body[k].x -= 11;
    }
    break;
  default:
    break;
  }
  return 0;
}

void clean_grid()
{
  for (int i = 0; i < GRID_SIZE; i++)
  {
    grid[i] = 0;
  }
}

int snakes_to_grid(snake *A, snake *B, unsigned short A_color, unsigned short B_color)
{
  int ret = 0;
  for (int i = 0; i < A->len * 121; i++)
  {
    if ((grid[SCREEN_WIDTH * A->body[i].y + A->body[i].x] != 0 && i < 121) || i >= GRID_SIZE || i < 0)
    {
      ret = 1;
      break;
    }
    else
    {
      grid[SCREEN_WIDTH * A->body[i].y + A->body[i].x] = A_color;
    }
  }
  for (int i = 0; i < B->len * 121; i++)
  {
    if ((grid[SCREEN_WIDTH * B->body[i].y + B->body[i].x] != 0 && i < 121) || i >= GRID_SIZE || i < 0)
    {
      ret = 2;
      break;
    }
    else
    {
      grid[SCREEN_WIDTH * B->body[i].y + B->body[i].x] = B_color;
    }
  }
  return ret;
}

void snakes_restart(int w, int h, snake *A, snake *B)
{

  free(A->body);
  A->body = NULL;
  free(B->body);
  B->body = NULL;

  A->len = B->len = 0;

  A->body = my_alloc(sizeof(spot) * MAX_SNAKE_LEN);
  add_chunk(9, h - 20, A);
  A->direction = 2;

  B->body = my_alloc(sizeof(spot) * MAX_SNAKE_LEN);
  add_chunk(w - 9, h - 20, B);
  B->direction = 2;
}

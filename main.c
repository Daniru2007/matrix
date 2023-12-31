#include <curses.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct drops {
  int x;
  int y;
  int amount;
} drops;

struct drops *droplets;
int num_drops = 2000;
int start_x = 0, start_y = 0;
char *drop_styles[12] = {
    "#", "@", "%", "$", "&", "*", "+", "=", "-", "!", "|", ".",
};

void print_drops() {
  erase();
  srandom(time(NULL));
  int max_x;
  int max_y;
  getmaxyx(stdscr, max_y, max_x);
  for (int i = 0; i < num_drops; i++) {
    for (int l = 0; l < droplets[i].amount; l++) {
      mvprintw(droplets[i].y - l, droplets[i].x, "%s", drop_styles[l]);
    }
    int rand_x = random() % max_x + 1;
    int rand_y = -(random() % 500 + 1);
    int rand_amount = random() % 10 + 1;
    // droplets[i].x += rand_x;
    droplets[i].y += 1;
    if (droplets[i].y - droplets[i].amount >= max_y) {
      droplets[i].y = rand_y;
      droplets[i].x = rand_x;
      droplets[i].amount = rand_amount;
    }
  }
  refresh();
}

void init() {
  srandom(time(NULL));
  initscr();
  curs_set(0);
  noecho();
  keypad(stdscr, TRUE);
  raw();
  nodelay(stdscr, TRUE);
  mousemask(ALL_MOUSE_EVENTS, NULL);
  int max_x;
  int max_y;
  getmaxyx(stdscr, max_y, max_x);
  num_drops = max_x * 25;
  droplets = calloc(num_drops, sizeof(drops));
  for (int i = 0; i < num_drops; i++) {
    int max_x = getmaxx(stdscr);
    int rand_x = random() % max_x + 1;
    int rand_amount = random() % 12 + 1;
    int rand_y = -(random() % 500 + 1);
    droplets[i].x = rand_x;
    droplets[i].y = rand_y;
    droplets[i].amount = rand_amount;
  }
}

int main(int argc, char *argv[]) {
  init();
  srandom(time(NULL));
  MEVENT event;
  int input;

  start_color();
  use_default_colors();
  init_pair(1, COLOR_GREEN, -1);
  init_pair(2, COLOR_BLUE, -1);
  init_pair(3, COLOR_CYAN, -1);
  init_pair(4, COLOR_RED, -1);
  init_pair(5, COLOR_WHITE, -1);
  init_pair(6, COLOR_MAGENTA, -1);
  init_pair(7, COLOR_YELLOW, -1);
  int color = 2;
  int delay = 60000;
  while (true) {
    attron(COLOR_PAIR(color));
    input = getch();
    if (input == 113 || input == (int)'q') {
      break;
    } else if (input == (int)'f') {
      if (delay > 10000) {
        delay -= 1000;
      }
    } else if (input == (int)'F') {
      delay += 1000;
    }
    switch (input) {
    case (int)'g':
      color = 1;
      break;
    case (int)'b':
      color = 2;
      break;
    case (int)'c':
      color = 3;
      break;
    case (int)'r':
      color = 4;
      break;
    case (int)'w':
      color = 5;
      break;
    case (int)'m':
      color = 6;
      break;
    case (int)'y':
      color = 7;
      break;
    }

    int max_x;
    int max_y;
    getmaxyx(stdscr, max_y, max_x);
    if (start_x != max_x || start_y != max_y) {
      start_x = max_x;
      start_y = max_y;
      free(droplets);
      erase();
      refresh();
      init();
    }

    print_drops();
    usleep(delay);
    attroff(COLOR_PAIR(color));
  }

  refresh();
  endwin();

  return 0;
}

#include <curses.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

struct drops {
  int x;
  int y;
  int amount;
};

struct drops droplets[2000];

void print_drops() {
  erase();
  srandom(time(NULL));
  int max_x;
  int max_y;
  getmaxyx(stdscr, max_y, max_x);
  for (int i = 0; i < 2000; i++) {
    for (int l = 0; l < droplets[i].amount; l++) {
      mvprintw(droplets[i].y - l, droplets[i].x, "$");
    }
    int rand_x = random() % 5 + 1;
    int rand_y = -(random() % 500 + 1);
    // droplets[i].x += rand_x;
    droplets[i].y += 1;
    if (droplets[i].y - droplets[i].amount >= max_y) {
      droplets[i].y = rand_y;
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
  for (int i = 0; i < 2000; i++) {
    int max_x = getmaxx(stdscr);
    int rand_x = random() % max_x + 1;
    int rand_amount = random() % 10 + 1;
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
  init_pair(2, COLOR_RED, -1);
  attron(COLOR_PAIR(1));
  while (true) {
    input = getch();
    if (input == 113 || input == 3) {
      break;
    }
    print_drops();
    usleep(80000);
  }

  attroff(COLOR_PAIR(1));

  refresh();
  endwin();

  return 0;
}

//
// Created by rogue on 07/11/24.
//

#include "delay.h"

#include <time.h>

void delay() {
  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = 0;
  nanosleep(&ts, NULL);
}
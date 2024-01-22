#ifndef PONG_H
#define PONG_H

#define LOAD_NUMBERS
#define LOAD_CHARS

#include "sprites.h"
#include "graphics.h" // Library created by Lucas Raniere to draw graphics in a low memory device
#include <stdint.h>

#ifndef BUTTON_UP
#define BUTTON_UP 2
#endif

#ifndef BUTTON_DOWN
#define BUTTON_DOWN 3
#endif

void bar(uint8_t x, uint8_t initialY);

void clearBar(uint8_t x, uint8_t yInitial);

void initializeObjects();

void initializePlayer();

void checkEvents();

int checkButtons();

int checkCollisions();

void initializeButtons();

void drawFrame();

void score(uint8_t playerPoints, uint8_t machinePoints);

void startScreen();

#endif

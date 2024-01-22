#include "pong.h"


enum {
  UP_PRESSED = -1,
  DEFAULT_PRESSED = 0,
  DOWN_PRESSED = 1,
  PLAYER_POINT,
  MACHINE_POINT,
  POINT_DEFAULT,
  PLAYER_WIN,
  MACHINE_WIN
};

typedef struct {
  float x;
  float y;
} Vector2d;

const uint8_t *points[10] = {zero, one, two, three, four, five, six, seven, eight, nine};

Vector2d posPlayer;

Vector2d posPlayerBefore;

Vector2d posBall;

Vector2d posBallBefore;

Vector2d ballVelocity;

Vector2d machinePos;

Vector2d machinePosBefore;

uint8_t playerPoints;

uint8_t machinePoints;

void score(uint8_t playerPoints, uint8_t machinePoints) {
  setColumnAddress(0, 127);
  setPageAddress(0,0);
  for(uint8_t i = 0; i < 55; i++) {
    sendSprite(0b10000000,0);
  }
  sendSprite8x8(points[playerPoints],0);
  sendSprite(0b10011000, 0);
  sendSprite(0b10011000, 0);
  sendSprite8x8(points[machinePoints],0);
  for(uint8_t i = 0; i < 127-73; i++) {
    sendSprite(0b10000000,0);
  }
}

void ball(uint8_t x, uint8_t y) {
  setColumnAddress(x,x);
  setPageAddress(y/N_PAGES + 1, 7);
  uint8_t byte = 0x01 << y%N_PAGES;
  sendSprite(byte, 0);
}

void clearBola(uint8_t x, uint8_t y) {
  setColumnAddress(x,x);
  setPageAddress(y/N_PAGES + 1, 7);
  sendSprite(0x00, 0);
}

void bar(uint8_t x, uint8_t yInitial) {
  if (yInitial % 8 != 0) {
    uint8_t byte1 = 0xff << yInitial % 8;
    uint8_t byte2 = 0xff >> (8 - (yInitial % 8));
    setColumnAddress(x,x);
    setPageAddress( yInitial/N_PAGES + 1, 7);
    sendSprite(byte1, 0);
    sendSprite(byte2, 0);
  } else {
    setColumnAddress(x, x);
    setPageAddress( yInitial/N_PAGES + 1,7);
    sendSprite(0xFF, 0);
  }
}

void clearBar(uint8_t x, uint8_t yInitial) {
  if (yInitial % 8 != 0) {
    setColumnAddress(x,x);
    setPageAddress( yInitial/N_PAGES + 1, 7);
    sendSprite(0x00,0);
    sendSprite(0x00,0);
  } else {
    setColumnAddress(x, x);
    setPageAddress( yInitial/N_PAGES + 1, 7);
    sendSprite(0xFF, 0);
  }
}

void setVector2d(Vector2d *toSet, float x, float y) {
  toSet->x = x;
  toSet->y = y;
}

void initializePlayer() {
  setVector2d(&posPlayer, 0, 20);
  setVector2d(&posPlayerBefore, 0, 20);
}

void initializeBall() {
  setVector2d(&posBall, 64, 32);
  setVector2d(&posBallBefore, 64, 32);
  setVector2d(&ballVelocity, 2, 1.5);
}

void initializeMachine() {
  setVector2d(&machinePos, 127, 20);
  setVector2d(&machinePosBefore, 127, 20);
}

void initializeObjects() {
  initializePlayer();
  initializeBall();
  playerPoints = 0;
  machinePoints = 0;
  score(playerPoints,machinePoints);
  initializeMachine();
}

void initializeButtons() {
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_UP, INPUT_PULLUP);
}

int checkButtons() {
  uint8_t buttonDown = digitalRead(BUTTON_DOWN);
  if (buttonDown == LOW) {
    return DOWN_PRESSED;
  }
  uint8_t buttonUp = digitalRead(BUTTON_UP);
  if (buttonUp == LOW) {
    return UP_PRESSED;
  }
  return 0;
}

void movePlayer(int buttonPressed) {
  posPlayerBefore.y = posPlayer.y;
  posPlayer.y += buttonPressed;
  if(posPlayer.y >= 48) {
    posPlayer.y = 48;
    posPlayerBefore.y = posPlayer.y;
  }

  if (posPlayer.y <= 8) posPlayer.y = 8;
}

bool verifyBarCollision() {
    bool colisaoComBarraJogador = posBall.x <= 2 && posBall.y >= posPlayer.y && posBall.y <= posPlayer.y + 8;
    bool colisaoComBarraMaquina = posBall.x >= 125 &&  posBall.y >= machinePos.y && posBall.y <= machinePos.y + 8;  
    return colisaoComBarraJogador || colisaoComBarraMaquina;
}

int verifyCollisions() {
  if (verifyBarCollision()) {
    if (ballVelocity.x < 0) ballVelocity.x -= 0.3;
    else ballVelocity.x += 0.3;
    ballVelocity.x *= -1;
    return POINT_DEFAULT;
  } else if (posBall.x <= 0) {
    return MACHINE_POINT;
  } else if (posBall.x >= 127) {
    return PLAYER_POINT;
  }
  if (posBall.y <= 8 || posBall.y >= 54) {
    ballVelocity.y *= -1;
  }
  return 0;
}

void moveBall() {
  setVector2d(&posBallBefore, posBall.x, posBall.y);
  posBall.x += ballVelocity.x;
  posBall.y += ballVelocity.y;
}

void moveMachine() {
  machinePosBefore.y = machinePos.y;
  if (machinePos.y < posBall.y) {
    machinePos.y += 2;
  } else if (machinePos.y > posBall.y) {
    machinePos.y -= 2;
  }
  if(machinePos.y >= 48) {
    machinePos.y = 48;
    machinePosBefore.y = machinePosBefore.y;
  }
  if (machinePos.y <= 8) machinePos.y = 8;
}


void startScreen() {
  setColumnAddress(10, 127);
  setPageAddress(3,7);
  sendSprite8x8(a,0);
  sendSprite8x8(p,0);
  sendSprite8x8(e,0);
  sendSprite8x8(r,0);
  sendSprite8x8(t,0);
  sendSprite8x8(e,0);
  sendSprite8x8(vazio,0);
  sendSprite8x8(p,0);
  sendSprite8x8(a,0);
  sendSprite8x8(r,0);
  sendSprite8x8(a,0);
  setColumnAddress(10, 127);
  setPageAddress(4,7);
  sendSprite8x8(j,0);
  sendSprite8x8(o,0);
  sendSprite8x8(g,0);
  sendSprite8x8(a,0);
  sendSprite8x8(r,0);
}

void restartGame() {
  clearBar(posPlayerBefore.x, posPlayerBefore.y);
  clearBar(machinePosBefore.x, machinePosBefore.y);
  clearBola(posBallBefore.x, posBallBefore.y);
  initializePlayer();
  initializeBall();
  score(playerPoints,machinePoints);
  initializeMachine();
}

void endGameScreen(uint8_t vencedor) {
  clearDisplay();
  setColumnAddress(10, 127);
  setPageAddress(3,7);
  if (vencedor == PLAYER_WIN) {
    sendSprite8x8(j,0);
    sendSprite8x8(o,0);
    sendSprite8x8(g,0);
    sendSprite8x8(a,0);
    sendSprite8x8(d,0);
    sendSprite8x8(o,0);
    sendSprite8x8(r,0);
  } else {
    sendSprite8x8(m,0);
    sendSprite8x8(a,0);
    sendSprite8x8(q,0);
    sendSprite8x8(u,0);
    sendSprite8x8(i,0);
    sendSprite8x8(n,0);
    sendSprite8x8(a,0);
  }
  setColumnAddress(10, 127);
  setPageAddress(4,7);
  sendSprite8x8(v,0);
  sendSprite8x8(e,0);
  sendSprite8x8(n,0);
  sendSprite8x8(c,0);
  sendSprite8x8(e,0);
  sendSprite8x8(u,0);
  while (checkButtons() == 0);
  clearDisplay();
  restartGame();
}

void verifyGameCondition(int colisaoDetectada) {
  if (colisaoDetectada == PLAYER_POINT) {
    playerPoints += 1;
    restartGame();
  } else if (colisaoDetectada == MACHINE_POINT) {
    machinePoints += 1;
    restartGame();
  }
  
  if (playerPoints == 10) {
    playerPoints = 0;
    machinePoints = 0;
    endGameScreen(PLAYER_WIN);
    
  } else if (machinePoints == 10) {
    playerPoints = 0;
    machinePoints = 0;
    endGameScreen(MACHINE_WIN);
    
  }

}

void checkEvents() {
  int botaoApertado = checkButtons();
  movePlayer(botaoApertado);
  moveBall();
  moveMachine();
  int collisionDetected = verifyCollisions();
  verifyGameCondition(collisionDetected);
}

void drawFrame() {
  clearBar(posPlayerBefore.x, posPlayerBefore.y);
  bar(posPlayer.x, posPlayer.y);
  clearBar(machinePosBefore.x, machinePosBefore.y);
  bar(machinePos.x, machinePos.y);
  clearBola(posBallBefore.x, posBallBefore.y);
  ball(posBall.x, posBall.y);
}
#include <stdio.h>

#include "game.h"

#define PLAYER_TURN   0
#define COMPUTER_TURN 1

int main(int argc, char *argv[]) {
  GameState *game = init_game(COMPUTER_SKILLED, COMPUTER_TURN);
  run_game(game);

  return 0;
}
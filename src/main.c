#include <stdio.h>

#include "game.h"

int main(int argc, char *argv[]) {
  GameState *game = init_game(TWO_PLAYER);
  run_game(game);

  return 0;
}
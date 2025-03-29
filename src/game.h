#ifndef GAME_H
#define GAME_H

typedef enum {
  TWO_PLAYER,
  COMPUTER_RANDOM,
  COMPUTER_SKILLED,
} GameMode;

typedef struct {
  // the current players turn
  //  0 -> player 1 | human
  //  1 -> player 2 | computer
  int turn;

  // the number of moves made in the game
  int moves;

  // represents the current state of the game
  //  0  ->  playing
  //  1  ->  draw
  //  2  ->  winner
  int state;

  GameMode mode;
  
  unsigned int x_pos;
  unsigned int y_pos;

  int board[3][3];
} GameState;

extern void run_game(GameState *game);
extern GameState *init_game(GameMode mode, int turn);

#endif
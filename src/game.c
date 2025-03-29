#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

#include "game.h"

#define UP    119  // w
#define LEFT   97  // a
#define DOWN  115  // s
#define RIGHT 100  // d
#define ENTER  10  // enter

#define EMPTY  0
#define CROSS  1
#define NOUGHT 2

#define PLAYER_TURN   0
#define COMPUTER_TURN 1

#define PLAYING 0
#define DRAW    1
#define WINNER  2

#define GRID_SIZE 3

#define PLAYER(game) game->turn == PLAYER_TURN ? "player 1" : "player 2"
#define CLEAR_SCREEN() printf("\e[1;1H\e[2J");

GameState *init_game(GameMode mode, int turn) {
  GameState *game = malloc(sizeof(GameState));
  game->state = PLAYING;
  game->mode = mode;
  game->turn = turn;
  game->moves = 0;
  game->x_pos = 0;
  game->y_pos = 0;

  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      game->board[i][j] = 0;
    }  
  }

  return game;
}

static char *square_to_str(GameState *game, int y, int x) {
  return game->board[y][x] == CROSS ? "X" : game->board[y][x] == NOUGHT ? "0" : "-";
}

static void draw_board(GameState *game) {
  CLEAR_SCREEN();

  printf("turn: %s\n", PLAYER(game));
  printf("moves: %d\n", game->moves);
  printf("x: %d | y: %d\n\n", game->x_pos, game->y_pos);

  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      if (game->x_pos == j && game->y_pos == i) {
        printf("[%s]  ", square_to_str(game, i, j));
      } else {
        printf(" %s   ", square_to_str(game, i, j));
      }
    }
    printf("\n\n");
  }
  printf("\n");
}

static inline int square(GameState *game, int y, int x) {
  return game->board[y][x];
}

static void end_game(GameState *game, int state) {
  draw_board(game);

  if (state == WINNER) {
    game->turn = 1 - game->turn;
    printf("\n%s wins.", PLAYER(game));
  }
  else if (state == DRAW) {
    printf("\ndraw.");
  }

  game->state = state;
}

static void check_game(GameState *game) {
  // the last player to move will be the winner
  int last_move = 1 - game->turn;

  // check rows
  for (int i = 0; i < GRID_SIZE; i++) {
    if (square(game, i, 0) != EMPTY && square(game, i, 0) == square(game, i, 1) && square(game, i, 1) == square(game, i, 2)) {
      end_game(game, WINNER);
      return;
    }
  }

  // check columns
  for (int i = 0; i < GRID_SIZE; i++) {
    if (square(game, 0, i) != EMPTY && square(game, 0, i) == square(game, 1, i) && square(game, 1, i) == square(game, 2, i)) {
      end_game(game, WINNER);
      return;
    }
  }

  // diagonals
  if (square(game, 0, 0) != EMPTY && square(game, 0, 0) == square(game, 1, 1) && square(game, 1, 1) == square(game, 2, 2)) {
    end_game(game, WINNER);
    return;
  }
  if (square(game, 0, 2) != EMPTY && square(game, 0, 2) == square(game, 1, 1) && square(game, 1, 1) == square(game, 2, 0)) {
    end_game(game, WINNER);
    return;
  }

  int is_draw = 1;
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      if (square(game, j, i) == EMPTY) {
        is_draw = 0;
        break;
      }
    }
  }

  if (is_draw) {
    end_game(game, DRAW);
  }
}

static void play_turn(GameState *game) {
  if (game->board[game->y_pos][game->x_pos] != EMPTY) {
    printf("invalid move.");
    getchar();
    return;
  }

  printf("%s moved.", PLAYER(game));
  getchar();

  game->board[game->y_pos][game->x_pos] = game->turn == PLAYER_TURN ? NOUGHT : CROSS;
  game->turn = 1 - game->turn;
  game->moves++;

  check_game(game);
}

void computer_random_move(GameState *game) {
  int x, y;
  int played = 0;

  srand(time(NULL));

  while (!played) {
    x = rand() % GRID_SIZE;
    y = rand() % GRID_SIZE;

    if (square(game, y, x) == EMPTY) {
      game->x_pos = x;
      game->y_pos = y;

      play_turn(game);
      played = 1;
    }
  }
}

static int evaluate_game(GameState *game) {
  for (int i = 0; i < GRID_SIZE; i++) {
    if (square(game, i, 0) != EMPTY && square(game, i, 0) == square(game, i, 1) && square(game, i, 1) == square(game, i, 2)) {
      return square(game, i, 0) == CROSS ? 1 : -1;
    }
  }

  // check columns
  for (int i = 0; i < GRID_SIZE; i++) {
    if (square(game, 0, i) != EMPTY && square(game, 0, i) == square(game, 1, i) && square(game, 1, i) == square(game, 2, i)) {
      return square(game, 0, i) == CROSS ? 1 : -1;
    }
  }

  // diagonals
  if (square(game, 0, 0) != EMPTY && square(game, 0, 0) == square(game, 1, 1) && square(game, 1, 1) == square(game, 2, 2)) {
    return square(game, 0, 0) == CROSS ? 1 : -1;
  }
  if (square(game, 0, 2) != EMPTY && square(game, 0, 2) == square(game, 1, 1) && square(game, 1, 1) == square(game, 2, 0)) {
    return square(game, 0, 2) == CROSS ? 1 : -1;
  }

  int is_draw = 1;
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      if (square(game, j, i) == EMPTY) {
        is_draw = 0;
        break;
      }
    }
  }

  if (is_draw) return 0;
  return 0;
}

int minimax(GameState *game, int depth, int is_computer_turn) {
  int score = evaluate_game(game);

  if (score == 1 || score == -1 || score == 0) {
    return score;
  }

  if (is_computer_turn) {
    int best_value = -1000;

    for (int i = 0; i < GRID_SIZE; i++) {
      for (int j = 0; j < GRID_SIZE; j++) {
        if (square(game, i, j) == EMPTY) {
          game->board[i][j] = CROSS;
          int current_value = minimax(game, depth + 1, 0);
          best_value = (current_value > best_value) ? current_value : best_value;
          game->board[i][j] = EMPTY;
        }
      }
    }
    return best_value;
  }
  else {
    int best_value = 1000;

    for (int i = 0; i < GRID_SIZE; i++) {
      for (int j = 0; j < GRID_SIZE; j++) {
        if (square(game, i, j) == EMPTY) {
          game->board[i][j] = NOUGHT;
          int current_value = minimax(game, depth + 1, 1);
          best_value = (current_value < best_value) ? current_value : best_value;
          game->board[i][j] = EMPTY;
        }
      }
    }
    return best_value;
  }
}

static void computer_best_move(GameState *game) {
  int best_value = -1000;
  int best_x_pos = -1;
  int best_y_pos = -1;

  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      if (square(game, i, j) == EMPTY) {
        game->board[i][j] = CROSS;
        int move_value = minimax(game, 0, 0);
        game->board[i][j] = EMPTY;

        if (move_value > best_value) {
          best_value = move_value;
          best_x_pos = j;
          best_y_pos = i;
        }
      }
    }
  }

  game->x_pos = best_x_pos;
  game->y_pos = best_y_pos;

  play_turn(game);
}

static void turn(GameState *game) {
  if (game->mode == COMPUTER_RANDOM && game->turn == COMPUTER_TURN) {
    computer_random_move(game);
    return;
  }

  if (game->mode == COMPUTER_SKILLED && game->turn == COMPUTER_TURN) {
    computer_best_move(game);
    return;
  }

  int c = getchar();
  if (c == ENTER) {
    printf("press enter to confirm move.");
  }

  while (getchar() != '\n');

  if (c == UP && game->y_pos != 0) {
    game->y_pos -= 1;
  }
  else if (c == DOWN && game->y_pos != GRID_SIZE - 1) {
    game->y_pos += 1;
  }
  else if (c == LEFT && game->x_pos != 0) {
    game->x_pos -= 1;
  }
  else if (c == RIGHT && game->x_pos != GRID_SIZE - 1) {
    game->x_pos += 1;
  } 
  else if (c == ENTER) {
    play_turn(game);
  }
}

void run_game(GameState *game) {
  while (game->state == PLAYING) {
    draw_board(game);
    turn(game);
  }
}
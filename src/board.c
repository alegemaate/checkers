#include "board.h"

#include <stdbool.h>

#include "core.h"

// Init the board
void init_board(Board* board, int width, int height) {
  // Size
  board->width = width;
  board->height = height;

  // Selection
  board->selection_x = -1;
  board->selection_y = -1;

  // Null point image
  board->image_board = NULL;

  // Make the board
  board->image_board = generate_board(renderer, board->width, board->height);

  // Multimove activated
  board->multimove = false;

  // Checkers
  board->checkers =
      (Checker**)malloc(sizeof(Checker*) * width * height);  // Max checkers

  for (int i = 0; i < width * height; i++) {
    board->checkers[i] = NULL;
  }

  // Moves
  board->moves =
      (Position**)malloc(sizeof(Position*) * width * height);  // Max moves

  for (int i = 0; i < width * height; i++) {
    board->moves[i] = NULL;
  }
}

// Draw the board
void draw_board(Board* board) {
  // IF it exists
  if (board->image_board != NULL) {
    SDL_Point size;
    SDL_QueryTexture(board->image_board, NULL, NULL, &size.x, &size.y);
    SDL_Rect dest = {0, 0, size.x, size.y};
    SDL_RenderCopy(renderer, board->image_board, NULL, &dest);
  }

  // Draw selection
  SDL_Rect selectionRect;
  selectionRect.x = board->selection_x * board->width / TILES_WIDE;
  selectionRect.y = board->selection_y * board->height / TILES_HIGH;
  selectionRect.w = board->width / TILES_WIDE - 1;
  selectionRect.h = board->height / TILES_HIGH - 1;
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
  SDL_RenderFillRect(renderer, &selectionRect);

  // Available moves
  for (int i = 0; i < board->width * board->height; i++) {
    if (board->moves[i] == NULL) {
      break;
    }

    SDL_Rect moveRect;
    moveRect.x = board->moves[i]->x * board->width / TILES_WIDE;
    moveRect.y = board->moves[i]->y * board->height / TILES_HIGH;
    moveRect.w = board->width / TILES_WIDE - 1;
    moveRect.h = board->height / TILES_HIGH - 1;
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &moveRect);
  }

  // Draw pieces
  for (int i = 0; i < board->width * board->height; i++) {
    if (board->checkers[i] == NULL) {
      break;
    }

    draw_checker(board->checkers[i]);
  }

  // Debug
  // textprintf_ex(tempBuffer, font, 500, 20, 0xFFFFFF, -1, "Moves:%i",
  //               moves.size());
}

// Add checkers
void add_checker(Board* board, Checker* newChecker) {
  for (int i = 0; i < board->width * board->height; i++) {
    if (board->checkers[i] == NULL) {
      board->checkers[i] = newChecker;
      break;
    }
  }
}

// Calculate moves for selected checker
void calculate_moves(Board* board,
                     Position selectedTile,
                     int color,
                     bool king,
                     bool first) {
  printf("Calculating moves\n");

  // Left side and then right
  for (int i = -1; i <= 1; i += 2) {
    // Backward for kings
    for (int t = 0; t <= king; t += 1) {
      printf("Checking: %i %i\n", i, t);
      // Tile to check
      int new_x = selectedTile.x + i;
      int new_y = selectedTile.y + ((color * 2) - 1) * -((t * 2) - 1);
      int checker_at_pos =
          checker_at(board, (Position){.x = new_x, .y = new_y});

      // Single movements
      if (checker_at_pos == -1 && first) {
        for (int i = 0; i < board->width * board->height; i++) {
          if (board->moves[i] == NULL) {
            board->moves[i] = (Position*)malloc(sizeof(Position));
            board->moves[i]->x = new_x;
            board->moves[i]->y = new_y;
            break;
          }
        }
      }

      // Jumps ( color is opposite, space behind to land)
      else if (checker_at_pos >= 0 &&
               board->checkers[checker_at_pos]->color != color &&
               checker_at(board, (Position){.x = new_x + i,
                                            .y = new_y + ((color * 2) - 1) *
                                                             -((t * 2) - 1)}) ==
                   -1) {
        for (int i = 0; i < board->width * board->height; i++) {
          if (board->moves[i] == NULL) {
            board->moves[i] = (Position*)malloc(sizeof(Position));
            board->moves[i]->x = new_x + i;
            board->moves[i]->y = new_y + ((color * 2) - 1) * -((t * 2) - 1);
            break;
          }
        }

        board->checker_jumped.x = new_x;
        board->checker_jumped.y = new_y;
      }
    }
  }

  printf("Moves:\n");
  for (int i = 0; i < board->width * board->height; i++) {
    if (board->moves[i] == NULL) {
      break;
    }

    // Debug
    printf("Move: %i %i\n", board->moves[i]->x, board->moves[i]->y);
  }
}

// Checker at pos
int checker_at(Board* board, Position newPosition) {
  // Out of range
  if (newPosition.x < 0 || newPosition.y < 0 || newPosition.x >= TILES_WIDE ||
      newPosition.y >= TILES_HIGH) {
    return -2;
  }

  // Check
  for (int i = 0; i < board->width * board->height; i++) {
    if (board->checkers[i] == NULL) {
      break;
    }

    if (is_checker_at(board->checkers[i], newPosition.x, newPosition.y)) {
      return i;
      break;
    }
  }
  return -1;
}

// Move at Position
int move_at(Board* board, Position newPosition) {
  // Out of range
  if (newPosition.x < 0 || newPosition.y < 0 || newPosition.x >= TILES_WIDE ||
      newPosition.y >= TILES_HIGH) {
    return -2;
  }
  // Check
  for (int i = 0; i < board->width * board->height; i++) {
    if (board->moves[i] == NULL) {
      break;
    }

    if (board->moves[i]->x == newPosition.x &&
        board->moves[i]->y == newPosition.y) {
      return i;
      break;
    }
  }
  return -1;
}

// Select tile
void select_tile(Board* board, Position newPosition) {
  // Check if it is a move
  if (move_at(board, (Position){.x = newPosition.x, .y = newPosition.y}) >= 0) {
    // Get index of selected checker
    int checker_to_move = checker_at(
        board, (Position){.x = board->selection_x, .y = board->selection_y});
    bool double_jump = (abs(board->selection_y - newPosition.y) > 1);

    jump_checker(board->checkers[checker_to_move],
                 board->selection_x > newPosition.x, double_jump,
                 ((board->checkers[checker_to_move]->color == 0) &&
                  (board->selection_y < newPosition.y)) ||
                     ((board->checkers[checker_to_move]->color == 1) &&
                      (board->selection_y > newPosition.y)));

    // Check if needs kinging
    if (board->checkers[checker_to_move]->y == (TILES_HIGH - 1) ||
        board->checkers[checker_to_move]->y == 0) {
      board->checkers[checker_to_move]->king = true;
    }

    // Erase jumped enemy
    if (double_jump) {
      // Remove jumped checker
      // checkers.erase(checkers.begin() +
      //                checker_at(board, Position(board->checker_jumped.x,
      //                                           board->checker_jumped.y)));
      // Clear all moves (needs recalculating)
      for (int i = 0; i < board->width * board->height; i++) {
        if (board->moves[i] != NULL) {
          free(board->moves[i]);
          board->moves[i] = NULL;
        }
      }

      // Recalculate moves
      calculate_moves(board,
                      (Position){.x = board->checkers[checker_to_move]->x,
                                 .y = board->checkers[checker_to_move]->y},
                      board->checkers[checker_to_move]->color,
                      board->checkers[checker_to_move]->king, false);
      // No more moves
      if (board->moves[0] == NULL) {
        turn = !turn;
        board->multimove = false;
      } else {
        board->multimove = true;
      }
    } else {
      // Change turn
      turn = !turn;
      for (int i = 0; i < board->width * board->height; i++) {
        if (board->moves[i] != NULL) {
          free(board->moves[i]);
        }
        board->moves[i] = NULL;
      }
      board->multimove = false;
    }
  } else {
    // Clear moves
    for (int i = 0; i < board->width * board->height; i++) {
      if (board->moves[i] != NULL) {
        free(board->moves[i]);
      }
      board->moves[i] = NULL;
    }

    // Piece Exists
    board->selection_x = newPosition.x;
    board->selection_y = newPosition.y;
    int selection_index = checker_at(
        board, (Position){.x = board->selection_x, .y = board->selection_y});
    if (selection_index <= -1 ||
        board->checkers[selection_index]->color != turn) {
      board->selection_x = -1;
      board->selection_y = -1;
    } else if (!board->multimove) {
      // Calc new moves
      calculate_moves(
          board, (Position){.x = board->selection_x, .y = board->selection_y},
          board->checkers[selection_index]->color,
          board->checkers[selection_index]->king, true);
    }
  }
}

// Generate board texture
SDL_Texture* generate_board(SDL_Renderer* renderer, int width, int height) {
  SDL_Surface* temp_image =
      SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
  SDL_FillRect(temp_image, NULL,
               SDL_MapRGB(temp_image->format, 0x8C, 0x52, 0x42));

  // Populate squares
  for (int i = 0; i < TILES_WIDE; i += 1) {
    for (int t = 0; t < TILES_HIGH; t += 1) {
      // If it should be drawn
      if ((i % 2 == 0 && t % 2 == 1) || (i % 2 == 1 && t % 2 == 0)) {
        SDL_Rect rect;
        rect.x = i * width / TILES_WIDE;
        rect.y = t * height / TILES_HIGH;
        rect.w = width / TILES_WIDE;
        rect.h = height / TILES_HIGH;
        SDL_FillRect(temp_image, &rect,
                     SDL_MapRGB(temp_image->format, 0xFF, 0xFF, 0xCE));
      }
    }
  }

  // Create texture from surface
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, temp_image);
  SDL_FreeSurface(temp_image);

  // Return the texture
  return texture;
}

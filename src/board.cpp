#include "board.h"
#include "core.h"

// Init the board (no parameters)
board::board() {
  // Size
  this->width = 1;
  this->height = 1;

  // Selection
  selection_x = -1;
  selection_y = -1;

  // Null point image
  image_board = NULL;

  // Multimove activated
  multimove = false;
}

// Init the board
board::board(int width, int height) {
  // Size
  this->width = width;
  this->height = height;

  // Selection
  selection_x = -1;
  selection_y = -1;

  // Null point image
  image_board = NULL;

  // Make the board
  image_board = generate_board(renderer, this->width, this->height);

  // Multimove activated
  multimove = false;
}

// Draw the board
void board::draw() {
  // IF it exists
  if (image_board != NULL) {
    SDL_Point size;
    SDL_QueryTexture(image_board, nullptr, nullptr, &size.x, &size.y);
    SDL_Rect dest = {0, 0, size.x, size.y};
    SDL_RenderCopy(renderer, image_board, nullptr, &dest);
  }

  // Draw selection
  SDL_Rect selectionRect;
  selectionRect.x = selection_x * width / TILES_WIDE;
  selectionRect.y = selection_y * height / TILES_HIGH;
  selectionRect.w = width / TILES_WIDE - 1;
  selectionRect.h = height / TILES_HIGH - 1;
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
  SDL_RenderFillRect(renderer, &selectionRect);

  // Available moves
  for (unsigned int i = 0; i < moves.size(); i++) {
    SDL_Rect moveRect;
    moveRect.x = moves.at(i).x * width / TILES_WIDE;
    moveRect.y = moves.at(i).y * height / TILES_HIGH;
    moveRect.w = width / TILES_WIDE - 1;
    moveRect.h = height / TILES_HIGH - 1;
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &moveRect);
  }

  // Draw pieces
  for (unsigned int i = 0; i < checkers.size(); i++) {
    checkers.at(i).draw();
  }

  // Debug
  // textprintf_ex(tempBuffer, font, 500, 20, 0xFFFFFF, -1, "Moves:%i",
  //               moves.size());
}

// Add checkers
void board::add_checker(checker newChecker) {
  checkers.push_back(newChecker);
}

// Calculate moves for selected checker
void board::calculate_moves(position selectedTile,
                            int color,
                            bool king,
                            bool first) {
  // Left side and then right
  for (int i = -1; i <= 1; i += 2) {
    // Backward for kings
    for (int t = 0; t <= king; t += 1) {
      // Tile to check
      int new_x = selectedTile.x + i;
      int new_y = selectedTile.y + ((color * 2) - 1) * -((t * 2) - 1);

      // Single movements
      if (checker_at(position(new_x, new_y)) == -1 && first) {
        moves.push_back(position(new_x, new_y));
      }

      // Jumps ( color is opposite, space behind to land)
      else if (checker_at(position(new_x, new_y)) >= 0 &&
               checkers.at(checker_at(position(new_x, new_y))).color != color &&
               checker_at(position(
                   new_x + i, new_y + ((color * 2) - 1) * -((t * 2) - 1))) ==
                   -1) {
        moves.push_back(
            position(new_x + i, new_y + ((color * 2) - 1) * -((t * 2) - 1)));
        checker_jumped.x = new_x;
        checker_jumped.y = new_y;
      }
    }
  }
}

// Checker at pos
int board::checker_at(position newPosition) {
  // Out of range
  if (newPosition.x < 0 || newPosition.y < 0 || newPosition.x >= TILES_WIDE ||
      newPosition.y >= TILES_HIGH)
    return -2;
  // Check
  for (unsigned int i = 0; i < checkers.size(); i++) {
    if (checkers.at(i).is_at(newPosition.x, newPosition.y)) {
      return i;
      break;
    }
  }
  return -1;
}

// Move at position
int board::move_at(position newPosition) {
  // Out of range
  if (newPosition.x < 0 || newPosition.y < 0 || newPosition.x >= TILES_WIDE ||
      newPosition.y >= TILES_HIGH)
    return -2;
  // Check
  for (unsigned int i = 0; i < moves.size(); i++) {
    if (moves.at(i).x == newPosition.x && moves.at(i).y == newPosition.y) {
      return i;
      break;
    }
  }
  return -1;
}

// Select tile
void board::select_tile(position newPosition) {
  // Check if it is a move
  if (move_at(position(newPosition.x, newPosition.y)) >= 0) {
    // Get index of selected checker
    int checker_to_move = checker_at(position(selection_x, selection_y));
    bool double_jump = (abs(selection_y - newPosition.y) > 1);
    checkers.at(checker_to_move)
        .jump(selection_x > newPosition.x, double_jump,
              ((checkers.at(checker_to_move).color == 0) &&
               (selection_y < newPosition.y)) ||
                  ((checkers.at(checker_to_move).color == 1) &&
                   (selection_y > newPosition.y)));

    // Check if needs kinging
    if (checkers.at(checker_to_move).get_y() == (TILES_HIGH - 1) ||
        checkers.at(checker_to_move).get_y() == 0) {
      checkers.at(checker_to_move).king_me();
    }

    // Erase jumped enemy
    if (double_jump) {
      // Remove jumped checker
      checkers.erase(checkers.begin() +
                     checker_at(position(checker_jumped.x, checker_jumped.y)));
      // Clear all moves (needs recalculating)
      moves.clear();
      // Recalculate moves
      calculate_moves(position(checkers.at(checker_to_move).get_x(),
                               checkers.at(checker_to_move).get_y()),
                      checkers.at(checker_to_move).color,
                      checkers.at(checker_to_move).king, false);
      // No more moves
      if (moves.size() == 0) {
        globals::turn = !globals::turn;
        multimove = false;
      } else {
        multimove = true;
      }
    } else {
      // Change turn
      globals::turn = !globals::turn;
      moves.clear();
      multimove = false;
    }
  } else {
    // Clear moves
    moves.clear();

    // Piece Exists
    selection_x = newPosition.x;
    selection_y = newPosition.y;
    int selection_index = checker_at(position(selection_x, selection_y));
    if (selection_index <= -1 ||
        checkers.at(selection_index).color != globals::turn) {
      selection_x = -1;
      selection_y = -1;
    } else if (!multimove) {
      // Calc new moves
      calculate_moves(position(selection_x, selection_y),
                      checkers.at(selection_index).color,
                      checkers.at(selection_index).king, true);
    }
  }
}

// Generate board texture
SDL_Texture* board::generate_board(SDL_Renderer* renderer,
                                   int width,
                                   int height) {
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

#include "checker.h"

// Constructor
checker::checker(){
  // Default vars
  this -> x = 0;
  this -> y = 0;
  this -> width = 1;
  this -> height = 1;
  this -> color = COLOR_RED;
  this -> image = NULL;
  this -> image_king = NULL;
  this -> king = false;
}

// Constructor with vars
checker::checker( int x, int y, int width, int height, int color){
  // Init vars
  this -> x = x;
  this -> y = y;
  this -> width = width;
  this -> height = height;
  this -> color = color;
  this -> image = NULL;
  this -> image_king = NULL;
  this -> king = false;

  // Make checker
  this -> image = generate_image( this -> width, this -> height, this -> color);
  this -> image_king = generate_image( this -> width, this -> height, this -> color, true);
}

// Deconstructor
checker::~checker(){
  //dtor
}

// Draw
void checker::draw( BITMAP *tempBuffer){
  // IF it exists
  if( king && image_king != NULL)
    draw_sprite( tempBuffer, image_king, x * width, y * height);
  else if( image != NULL)
    draw_sprite( tempBuffer, image, x * width, y * height);
}

// At position
bool checker::is_at( int x, int y){
  if( this -> x == x && this -> y == y)
    return true;
  return false;
}

// Move
void checker::jump( bool left, bool super, bool backwards){
  y += ((color * 2) - 1) * -((backwards * 2) - 1) * (super + 1);
  if( !left)
    x += super + 1;
  else
    x -= super + 1;
}

// Generate a image
BITMAP *checker::generate_image( int width, int height, int color, bool king){
  // Make it
  BITMAP *tempImage = create_bitmap( width, height);

  // Clear it to transparent
  clear_to_color( tempImage, 0xFF00FF);

  // Circle
  ellipsefill( tempImage, width/2, height/2, width/2 - 6, height/2 - 6, makecol( (230 * ((color + 1) % 1)), 0, 0));
  ellipsefill( tempImage, width/2, height/2, width/2 - 8, height/2 - 8, makecol( (230 * color), 20, 20));

  // King
  if( king){
    ellipsefill( tempImage, width/2, height/2, width/2 - width/3, height/2 - height/3, makecol( 128, 128, 20));
  }

  // Return that image
  return tempImage;
}

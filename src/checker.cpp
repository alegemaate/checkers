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

  // Make checker
  this -> image = generate_image( this -> width, this -> height, this -> color);
}

// Deconstructor
checker::~checker(){
  //dtor
}

// Draw
void checker::draw( BITMAP *tempBuffer){
  // IF it exists
  if( image != NULL)
    draw_sprite( tempBuffer, image, x, y);
}

// At position
bool checker::is_at( int x, int y){
  if( this -> x == x && this -> y == y)
    return true;
  return false;
}

// Move
void checker::jump( bool left, bool super){
  y += height * ((color * 2) - 1);
  if( !left)
    x += width * (super + 1);
  else
    x -= width * (super + 1);
}

// Generate a image
BITMAP *checker::generate_image( int width, int height, int color){
  // Make it
  BITMAP *tempImage = create_bitmap( width, height);

  // Clear it to transparent
  clear_to_color( tempImage, 0xFF00FF);

  // Circle
  ellipsefill( tempImage, width/2, height/2, width/2 - 6, height/2 - 6, makecol( (230 * ((color + 1) % 1)), 0, 0));
  ellipsefill( tempImage, width/2, height/2, width/2 - 8, height/2 - 8, makecol( (230 * color), 20, 20));

  // Return that image
  return tempImage;
}

#include "Shape.h"
#include "shapes.h"

bool Shape::is_resting_on_block(uint8_t* _screen_buffer)
{
  const uint8_t* shape_data = &shapes[this->type][this->rotation * 4];

  // Loop for each bit to check
  for (uint8_t mask = 1; mask != 0; mask <<= 1 ) {
    // Loop backwards trough shape rows
    for (int row_index= 3; row_index >= 0; --row_index) {
      // Bit shift the shape data to the right by x.
      uint8_t shape_row = shape_data[row_index] << this->x;

      // Stop and check if we find a set bit.
      if (shape_row & mask) {
          // The screen row below the set bit.
          uint8_t* screen_row = &_screen_buffer[this->y + row_index + 1];

          // Is the same bit set?
          if (*screen_row & mask) {
              return true;
          }
          else {
              break;
          }
      }
    }
  }
  return false;
}


void Shape::draw(uint8_t* screen_buffer, bool erase)
{
  const uint8_t* shape_data = &shapes[this->type][this->rotation * 4];

  for (int row_index= 0; row_index < 4; ++row_index)
  {
    // Bit shift the shape data to the right by x.
    uint8_t shape_row = shape_data[row_index] << this->x;

    // Get a pointer to the row in the screen buffer
    uint8_t* screen_row = &screen_buffer[this->y + row_index];

    if (erase)
        *screen_row &= ~shape_row;
    else
        *screen_row |= shape_row;
  }
}

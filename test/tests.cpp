#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "source/Shape.h"

// Hideous, but I can't find another way to get yotta to link in the functions
// under test.
#include "source/shapes.c"
#include "source/Shape.cpp"


/*
 * Check that a 'tee' shape sitting above the gap in the last line is not
 * considered to have collided with anything.
 */
static void test_block_collision_detection()
{
  uint8_t screen_buffer[] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00011111,

    0b11111111
  };
  Shape shape;

  shape.x = 5;
  shape.y = 13;
  shape.type = 5;

  shape.draw(screen_buffer);
  assert(screen_buffer[14] == 0b11100000);
  shape.draw(screen_buffer, /*erase = */ true);
  assert(screen_buffer[14] == 0b00000000);
  assert(!shape.is_resting_on_block(screen_buffer));
}

int main()
{
  test_block_collision_detection();
}

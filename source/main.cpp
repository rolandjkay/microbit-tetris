#include <stdint.h>
#include <string.h>
#include "shapes.h"
#include "Shape.h"
#include "Board.h"

#include "microbit-max7219-display-driver/Max7219Display.h"

#ifdef TARGET_LIKE_ARM
#  include "MicroBit.h"
#else
#  include <stdio.h>
#  include "microbit-max7219-display-driver/microbit-dal-osx.h"
#  include <stubs.h>
#endif

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 16
// The addresses are all doubled with respect to the hardware addresses;
// I don't know why
//#define SCREEN_ADDR_1 0x70 << 1
//#define SCREEN_ADDR_2 0x72 << 1

/*
 * Used to compose image that we will flush to display.
 */
/*static uint8_t screen_buffer[17] = {
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  // Note, by turning on all the LEDs on the first row that is just off
  // the screen. We don't need to check for collisions with the bottom of
  // the screen, because the code that checks for collisions between objects
  // will handle it.
  0xff
};*/

/****************************************************************************
 * Hardware
 */

MicroBit uBit;

static const size_t display_indices[] = {1,0};
static Max7219Display display(display_indices, 2, 1);


/****************************************************************************
 * Screen Control
 */
// XXX Needs to be refactored into its own module
#if 0

/*
 * Screen buffer
 * - 16 bytes for each screen; the HT16K33 controls 16 LEDs per row; therefore,
 *   there are two bytes per row, the second should always be zero.
 * - When we send data to the chip, we have to prefix it with a zero byte,
 *   which gives that start address of the write in screen memory. Fortunately,
 *   for the second screen, we can reuse the last byte of the previous screen's
 *   buffer, which is always zero. Thus, the memory layout is exactly the same
 *   as for a screen with 16 rows.
 */

 uint8_t write_cmd_buffer[] = {
     0,  // <- start address = 0
//   row1 row2 row3 row4 row5 row6 row7 row8
     0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0,   // <- first matrix
     0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0,   // <- second matrix
     // A bit of overrun to make our life easier. Shapes are four rows high
     // but later rows may be empty. Having a bit of overrun means we don't
     // trip the bounds check when writing such a shape at the bottom of the
     // display.
     // Note, by turning on all the LEDs on the first row that is just off
     // the screen. We don't need to check for collisions with the bottom of
     // the screen, because the code that checks for collisions between objects
     // will handle it.
     0xff,0, 0,0, 0,0, 0,0
};

uint8_t* screen_buffer = &write_cmd_buffer[1];
static const int screen_buffer_size = sizeof(write_cmd_buffer) - 1;

static char HT16K33_ON = 0x21;
static char HT16K33_DISPLAYON = 0x81;
//static char HT16K33_DISPLAYOFF = 0x80;
// Set dim from 0x00 (1/16th duty ccycle) to 0x0F (16/16 duty cycle)
//static char HT16K33_DIM  = 0xE0 | 0x08 ;
static char HT16K33_BRIGHT  = 0xE0 | 0x0c ;
//static char HT16K33_FULL  = 0xE0 | 0x0f ;

void i2c_write(int addr, char* data, int len) {
    if (uBit.i2c.write(addr, data, len) != 0) {
        uBit.display.printChar('x');
    }
}

int init_display() {
    int result = 0;
    result |= uBit.i2c.write(SCREEN_ADDR_1, &HT16K33_ON, 1);
    result |= uBit.i2c.write(SCREEN_ADDR_2, &HT16K33_ON, 1);
    result |= uBit.i2c.write(SCREEN_ADDR_1, &HT16K33_DISPLAYON, 1);
    result |= uBit.i2c.write(SCREEN_ADDR_2, &HT16K33_DISPLAYON, 1);
    result |= uBit.i2c.write(SCREEN_ADDR_1, &HT16K33_BRIGHT, 1);
    result |= uBit.i2c.write(SCREEN_ADDR_2, &HT16K33_BRIGHT, 1);

    if (result) {
        uBit.display.scroll("LEDs not connected");
    }

    return result;
}

/*
 * Refresh the display to reflect the contents of the screen buffer.
 */
void refresh_display() {
    int result = 0;
    result |= uBit.i2c.write(SCREEN_ADDR_1, (char*)write_cmd_buffer, 17);
    result |= uBit.i2c.write(SCREEN_ADDR_2, (char*)write_cmd_buffer + 16, 17);

    // Hint to user that there was a problem. There are sometimes transient
    // problems when refreshing the display, so don't interupt the game.
    if (result) {
        uBit.display.printChar('x');
    }
}
#endif




/****************************************************************************
 * Global Data
 */

int frame_count = 0;
static Shape shape;
static Board board;

/****************************************************************************
 * Main program.
 */

void update()
{
  // Move shape down
  if (frame_count % 10 == 0)
  {
      shape.y += 1;
      uBit.display.clear();
  }

  frame_count += 1;

  // Check buttons
  if (uBit.buttonAB.isPressed())
  {
      shape.rotation = (shape.rotation + 1) % 4;
  }
  else if (uBit.buttonA.isPressed())
  {
    shape.x += 1;
  }
  else if (uBit.buttonB.isPressed())
  {
    shape.x -= 1;
  }

  /*
   * Collision checks
   */
  // left
  if (shape.x < 0) {
    shape.x = 0;
  }

  // right
  if (shape.x >= BOARD_WIDTH - shape_width[shape.type][shape.rotation]  + 1)
  {
    shape.x = BOARD_WIDTH - 1 - shape_width[shape.type][shape.rotation] + 1;
  }

  // bottom & other blocks
  if (/*shape.y >= BOARD_HEIGHT - shape_height[shape.type][shape.rotation]
     ||*/ shape.is_resting_on_block(board.board_data)) {
    // Draw this shape (permenantly) before switching to next shape
    shape.draw(board.board_data);
    shape.y = 0;
    shape.type = (shape.type + 1) % num_shapes;
  }

  // Check for complete lines
  board.remove_complete_rows(display);
}

int main()
{
  // Initialise the micro:bit runtime.
  uBit.init();
  display.start_display();
  display.set_brightness(7);

  //uBit.i2c.frequency(100000);

  /*
   * Main game loop
   */
  while (true) {
    // Erase the current shape
    shape.draw(board.board_data, true);

    // Update the game data
    update();

    // Gave over check
    // - Is there anything in the top row?
    if (board.board_data[0] != 0x00) {
      uBit.display.scroll("Game over!");
      break;
    }

    // Redraw display
    shape.draw(board.board_data, false);
    board.write_to_display(display);

    // Wait 0.1 s
    wait(0.1);
  }

  // If main exits, there may still be other fibers running or registered event handlers etc.
  // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
  // sit in the idle task forever, in a power efficient sleep.
  release_fiber();
}

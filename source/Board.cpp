#include "Board.h"

#include <stdio.h>
#include <string.h>

#ifdef TARGET_LIKE_ARM
#  include "MicroBit.h"
#  include "microbit-max7219-display-driver/Max7219Display.h"
#else
#  include <stubs.h>
#endif


Board::Board()
{
  memset(board_data, 0, sizeof(board_data));

  // Note, by turning on all the LEDs on the first row that is just off
  // the screen. We don't need to check for collisions with the bottom of
  // the screen, because the code that checks for collisions between objects
  // will handle it.
  board_data[sizeof(board_data) - 1] = 0xff;
}

void Board::write_to_display(Max7219Display& display)
{
  uint8_t display_image[8] = {0};

  // Transpose screen buffer for display.
  for (size_t dst_row = 0; dst_row < 8; ++dst_row) {
    for (int src_row = 7; src_row >= 0; --src_row) {
      display_image[dst_row] |= (board_data[src_row] & (1<<dst_row) ? 1 : 0) << src_row;
    }
  }
  display.set_display(display_image,1, 1);

  memset(display_image, 0, 8);

  for (size_t dst_row = 0; dst_row < 8; ++dst_row) {
    for (int src_row = 7; src_row >= 0; --src_row) {
      display_image[dst_row] |= (board_data[src_row+8] & (1<<dst_row) ? 1 : 0) << src_row;
    }
  }
  display.set_display(display_image,1, 0);

  display.show();
}

/*
 * Clear a row (with animation)
 */
void Board::_clear_row(int row_index, Max7219Display& display)
{
  while (board_data[row_index] >>= 1)
  {
    this->write_to_display(display);
    wait_us(100000);
  }
}

/*
 * Shift all rows above row_index down one, covering row_index.
 */
void Board::_shift(int row_index)
{
  if (row_index > 15 || row_index < 1) {
    printf("Bounds overflow 3");
    return;
  }

  for (; row_index > 0; --row_index) {
    board_data[row_index] = board_data[row_index - 1];
  }
  board_data[0] = 0x00;
}

void Board::remove_complete_rows(Max7219Display& display)
{
  int row_index = 15;
  while (row_index > 0)
  {
    if (board_data[row_index] == 0xff)
    {
      _clear_row(row_index, display);
      _shift(row_index);
    }
    else {
        row_index -= 1;
    }
  }
}

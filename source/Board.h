#ifndef _BOARD_H
#define _BOARD_H
#include <stdint.h>

class Max7219Display;

class Board
{
public:
  Board();

  void write_to_display(Max7219Display& display);
  void remove_complete_rows(Max7219Display& display);

  uint8_t board_data[17];

private:
  void _clear_row(int row_index, Max7219Display& display);
  void _shift(int row_index);

};


#endif

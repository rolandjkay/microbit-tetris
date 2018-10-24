#ifndef _TETRIS_FUNCS_H
#define _TETRIS_FUNCS_H

#include <stdint.h>


/****************************************************************************
 * Shape Object
 */

struct Shape {
    Shape() {
        x = 0;
        y = 0;
        colour = 0;
        type = 5;
        rotation = 0;
    }

    void draw(uint8_t* screen_buffer, bool erase = false);
    bool is_resting_on_block(uint8_t* screen_buffer);

    int x;
    int y;
    int colour;
    int type;
    int rotation;
};



#endif

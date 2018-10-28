/*
 * When compiling for native target, we include these stubs of micro:bit
 * hardware abstractions. This allows us to run the program in the debugger
 * on the host
 */
#ifndef _NATIVE_STUBS_H
#define _NATIVE_STUBS_H

/*
 * Stub micro:bit classes when compiling natively.
 */
struct MicroBitDisplay
{
  void scroll(const char* str)
  {
    printf("%s\n", str);
  }

  void clear() {}
};

/*
 *        1
 *       111
 *  11111000
 */

struct MicroBitButton
{
  bool isPressed() { return false; }
};

struct MicroBit
{
  void init() {}

  MicroBitDisplay display;
  MicroBitButton buttonA;
  MicroBitButton buttonB;
  MicroBitButton buttonAB;
};

/*struct Max7219Display
{
  Max7219Display(const size_t* display_indices,
                 size_t display_width,
                 size_t display_height
               )
  {}

  void start_display() {}
  void set_brightness(uint8_t brightness) {}

  void set_led(size_t x, size_t y, bool value) {}

  void write_bitmap(const uint8_t* bitmap,
                    size_t width, size_t height,
                    size_t stride,
                    size_t x, size_t y) {}

  void set_display(const uint8_t* image,
                   size_t image_stride,
                   size_t display_index) {}

  void show() {}
};

// Stub platform functions
static inline void wait_us(size_t time) {}

*/
static inline void release_fiber() {}

#endif

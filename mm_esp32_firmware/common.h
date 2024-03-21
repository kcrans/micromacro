/* This is a common header file for the host and client C++ files
 * Make your basic configuration choices here so parameters can be
 * checked to see if they are in sync at compile-time. 
 */

/* Make sure NUM_COLS * NUM_ROWS <= 256 */
#define NUM_COLS 3
#define NUM_ROWS 4
/* Map your actual GPIO pins to key locations in the firmware source*/
/* Map key locations to keypresses/macros (i.e. (2, 3) -> 6) in the desktop client source*/
#define BAUDRATE 921600
/* @SymbolSize: 48 */
/* Function: date_step_next */
/* Overlay: DATE */
/* Address: 0x8014B104 */
/* Size: 48 bytes (12 instructions) */
#include "types.h"

extern volatile s16 date_open_step;
extern volatile s16 date_open_sub;
extern volatile s16 date_open_load_no;
extern volatile s16 date_open_cnt;

void date_step_next(void) {
    date_open_step = date_open_step + 1;
    date_open_sub = 0;
    date_open_cnt = 0;
    date_open_load_no = 0;
}

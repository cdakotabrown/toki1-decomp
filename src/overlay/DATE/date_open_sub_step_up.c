/* @SymbolSize: 40 */
/* Function: date_open_sub_step_up */
/* Overlay: DATE */
/* Address: 0x8014B134 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern volatile s16 date_open_sub;
extern volatile s16 date_open_load_no;
extern volatile s16 date_open_cnt;

void date_open_sub_step_up(void) {
    date_open_sub = date_open_sub + 1;
    date_open_cnt = 0;
    date_open_load_no = 0;
}

/* @SymbolSize: 132 */
/* Function: date_match_points */
/* Overlay: DATE */
/* Address: 0x80149154 */
/* Size: 132 bytes (33 instructions) */
#include "types.h"

extern s32 value_cache_find_or_load(s32, s32);
extern s32 printf(void *);

extern volatile u8 cdq_footer_chk;
extern volatile u8 bg_change_tbl1;
extern volatile s32 step_counter;

extern u8 date_match_points_str0[];
extern u8 date_match_points_str1[];

void date_match_points(s32 a0)
{
    s32 v0;
    s32 v1;
    s32 other;

    cdq_footer_chk = 0;
    v0 = value_cache_find_or_load(a0, 1);
    v1 = bg_change_tbl1;
    other = 1 - v1;
    if (v0 == v1) {
        printf(date_match_points_str0);
        step_counter = step_counter + 3;
    } else if (v0 == other) {
        printf(date_match_points_str1);
        step_counter = step_counter + 2;
    }
}

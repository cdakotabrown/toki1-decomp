/* Address: 0x80056760, Size: 36 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern s32 song_set_tbl0;
extern s32 movie_offset_y;

void set_movie_offset(s32 a, s32 b)
{
    song_set_tbl0 = (s32)(s16)a;
    movie_offset_y = (s32)(s16)b;
}

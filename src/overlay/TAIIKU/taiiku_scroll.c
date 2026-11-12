/* @SymbolSize: 56 */
/* Address: 0x801469E8 */
/* Size: 56 bytes (14 insns) */
#include "types.h"

extern void taiiku_scroll_target();
extern void taiiku_scroll_accum();
extern void taiiku_scroll_publish();
extern void taiiku_pose_frame_set();

void taiiku_scroll(void)
{
    taiiku_scroll_target();
    taiiku_scroll_accum();
    taiiku_scroll_publish();
    taiiku_pose_frame_set();
}

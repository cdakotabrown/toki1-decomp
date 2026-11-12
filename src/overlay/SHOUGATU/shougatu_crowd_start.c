/* @SymbolStart: 0x80141DA8 */
/* @SymbolSize: 72 */
/* Address: 0x80141DB0 */
/* Size: 64 bytes (16 instructions) */
#include "types.h"
#include "tokimeki/sprite.h"

extern void sub_sub_step_up();
extern void device_cmd_send();
extern s16 shougatu_crowd_cnt;
extern u8 now_girl;
extern SPRITE_REC Sprites[];

void shougatu_crowd_start(void)
{
    shougatu_crowd_cnt = 1;
    if (now_girl == 0) {
        device_cmd_send(518);
    }
    Sprites[4].x = -100;
    sub_sub_step_up();
}

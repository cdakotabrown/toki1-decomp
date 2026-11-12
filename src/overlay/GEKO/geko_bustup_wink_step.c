/* geko_bustup_wink_step -- GEKO overlay, Address: 0x80141BFC, Size: 168 bytes */
/* @PostProc: scheduler_normalizer --base-address 0x80132000 */
#include "types.h"
#include "tokimeki/sprite.h"

extern void bustup_wink(void);
extern void geko_serifuev_run(void);

extern u8 bustup_cd_read_tbl0;
extern u32 frame_counter;
extern u8 bustup_cw_disp;
extern u8 bustup2_cw_disp;
extern s32 Sprites;
extern s32 bustup_on;

void geko_bustup_wink_step(void)
{
    bustup_cd_read_tbl0 = 0;
    geko_serifuev_run();
    bustup_on = 0;
    bustup2_cw_disp |= 0x80;
    bustup_cw_disp |= 0x80;
    Sprites &= 0x7FFFFFFF;
    (*(s32 *)&((SPRITE_REC *)&Sprites)[1].attribute) &= 0x7FFFFFFF;
    if ((frame_counter % 240U) == 0) {
        bustup_wink();
    }
}

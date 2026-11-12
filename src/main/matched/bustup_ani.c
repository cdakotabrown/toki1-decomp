/* Address: 0x80083A10 */
/* Size: 520 bytes (130 instructions) */
/* Region: Game (IDO 5.2, 0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count auto */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"
#include "tokimeki/sprite.h"

typedef struct {
    u32 f0;
    s16 f4, f6, f8, fA, fC;
    u8  fE, fF;
    s16 f10, f12;
    u8  f14, f15, f16;
    s16 f18, f1A, f1C, f1E;
    u32 f20;
} Entry80041774;

typedef struct {
    u8  f0, f1, f2, f3;
    u8  rest[0x40];
} Rec80083A10;

extern Rec80083A10   bustup_cw_rec[];
extern Entry80041774 Sprites[];
extern s32 frame_counter;
extern s32 bustup_on;
extern s32 date_sprite_on;
extern s32 wink2_stop;
extern s32 bustup_mode;
extern s32 kuchi_stop;

extern void bustup_kuchi_ani(void);
extern void bustup_wink(void);
extern void kuchi_level_set(void);
extern void bustup2_flag_on(void);
extern void bustup_flag_on(void);

s32 bustup_ani(void) {
    if (bustup_on != 0) {
        bustup_cw_rec[1].f3 = bustup_cw_rec[1].f3 | 0x80;
        bustup_cw_rec[0].f3 = bustup_cw_rec[0].f3 | 0x80;
        Sprites[0].f0 = Sprites[0].f0 & 0x7FFFFFFF;
        Sprites[1].f0 = Sprites[1].f0 & 0x7FFFFFFF;
        bustup_kuchi_ani();
        if (kuchi_stop != 0) {
            ((SPRITE_REC *)Sprites)[0].v = 0;
        }
        if ((u32)frame_counter % 240 == 0) {
            bustup_wink();
        }
        return;
    }

    if (date_sprite_on != 0) {
        bustup_cw_rec[1].f3 = bustup_cw_rec[1].f3 & ~0x80;
        bustup_cw_rec[0].f3 = bustup_cw_rec[0].f3 & ~0x80;
        Sprites[0].f0 = Sprites[0].f0 | 0x80000000;
        Sprites[1].f0 = Sprites[1].f0 | 0x80000000;

        if (bustup_mode == 1) {
            if ((u32)frame_counter % 240 == 0) {
                bustup_flag_on();
            }
        } else if (bustup_mode == 0) {
            kuchi_level_set();
        }

        if ((u32)frame_counter % 240 == 0 && wink2_stop == 0) {
            bustup2_flag_on();
        }
        return;
    }

    bustup_cw_rec[1].f3 = bustup_cw_rec[1].f3 & ~0x80;
    bustup_cw_rec[0].f3 = bustup_cw_rec[0].f3 & ~0x80;
    Sprites[0].f0 = Sprites[0].f0 | 0x80000000;
    Sprites[1].f0 = Sprites[1].f0 | 0x80000000;
}

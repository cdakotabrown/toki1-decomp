/* @SymbolStart: 0x80140E90 */
/* @SymbolSize: 404 */
/* Address: 0x80140E98 */
/* Size: 396 bytes (99 instructions) */
/* @RodataBase: 0x80143AB0 */
/* @PostProc: patch_insn_reg_field 0x58:rt:v0:t0 0x5c:rs:v0:t0 0x5c:rt:v0:t0 0x64:rs:v0:t0 0x6c:rt:v0:t0 */
#include "types.h"

extern void game_reset(void);
extern void date_name_set(s32);
extern void shougatu_dispatch1(void);
extern void shougatu_dispatch2(void);
extern void shougatu_dispatch3(void);
extern void shougatu_dispatch4(void);
extern void shougatu_dispatch5(void);
extern void shougatu_dispatch6(void);
extern void shougatu_dispatch7(void);
extern void shougatu_dispatch8(void);
extern void shougatu_dispatch9(void);

typedef struct {
    volatile s16 h;
    u8 pad[54];
} Ent56;

extern Ent56 girl0_yuukou[];
extern volatile u8 now_girl;
extern u8 fade_bri;

typedef struct {
    u8 pad0[7];
    u8 b37;
    s16 b38;
    u8 pad1[12];
    volatile s16 b46;
    s16 b48;
} Blk30;

extern Blk30 cw96_active;

extern u8 now_sub_sub_step;

s32 shougatu_case5_girl_dispatch(void)
{
    u32 idx;
    s32 cnt;

    if (now_sub_sub_step == 0) {
        girl0_yuukou[now_girl].h = 70;
        date_name_set(now_girl);
    }

    idx = now_girl;
    switch (idx) {
    case 0:
        game_reset();
        break;
    case 1:
        shougatu_dispatch1();
        break;
    case 2:
        shougatu_dispatch2();
        break;
    case 3:
        shougatu_dispatch3();
        break;
    case 4:
        shougatu_dispatch4();
        break;
    case 5:
        shougatu_dispatch5();
        break;
    case 6:
        shougatu_dispatch6();
        break;
    case 7:
        shougatu_dispatch7();
        break;
    case 8:
        shougatu_dispatch8();
        break;
    case 9:
        shougatu_dispatch9();
        break;
    case 10:
        game_reset();
        break;
    }

    cw96_active.b37 = fade_bri;
    if (cw96_active.b48 != 0) {
        return;
    }
    if (cw96_active.b38 != 0) {
        return;
    }
    cnt = (s16)(cw96_active.b46 + 1);
    cw96_active.b46 = cnt;
    if (cnt >= 3) {
        cw96_active.b46 = 0;
        return 0;
    }
}

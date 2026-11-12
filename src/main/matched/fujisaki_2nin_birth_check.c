/* Function: fujisaki_2nin_birth_check */
/* Address: 0x80075580 */
/* Size: 244 bytes (61 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:23 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:23 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s32 girl0_birth;
extern s32 p_birthday;
extern u8 now_month;
extern u8 now_year;
extern u8 now_day;

extern void get_grp(void *a0, void *a1);
extern s32 reserved_date_check(u8 a0, s32 a1, u8 a2);
extern s32 birth_day_check(s32 a0);
extern s32 get_g_zyotai_s(s32 a0);

s32 fujisaki_2nin_birth_check(void) {
    s32 sp4C[5];
    s32 sp2C[8];
    s32 pad[2];
    s32 mode;

    (void)&pad;

    get_grp(sp4C, sp2C);

    if ((girl0_birth & 0xF) != (p_birthday & 0xF)) {
        goto fail;
    }
    if (((u32)(girl0_birth << 23) >> 27) == ((u32)(p_birthday << 23) >> 27)) {
        goto mode_code;
    }
fail:
    return 0;

mode_code:
    if (now_month == 3) {
        goto call_af70;
    }
    if (now_month != 4) {
        goto fail2;
    }
    if ((u32)now_day < 4U) {
        goto call_af70;
    }
fail2:
    return 0;

call_af70:
    mode = now_month;
    if (reserved_date_check(now_year, mode, now_day) > 0) {
        return 0;
    }
    if (birth_day_check(15) != 1) {
        return 0;
    }
    if (get_g_zyotai_s(0) & 0x80) {
        return 1;
    }
    return 0;
}

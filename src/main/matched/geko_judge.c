/* Address: 0x80070754 */
/* Size: 444 bytes (111 instructions) */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=69:s8 IDO52_AS1_TIE_REJECT=118,119 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x158 --offset2 0x160 */
#include "types.h"

extern u8 screen_w[];
extern u32 album_work;
extern s8 date_sasoi_judge_tbl0;

extern void get_grp(u8 *, u8 *);
extern s32 get_g_zyotai_s(s32);
extern s32 rand(void);

s32 geko_judge(s8 *arg0) {
    s32 slot;
    s32 i;
    s32 s2;
    u8 sp7C[24];
    u8 sp5C[32];

    *arg0 = 0;
    slot = album_work % 11;
    get_grp(sp7C, sp5C);

    i = 0;
    s2 = slot;

    do {
        slot = s2 % 11;

        if (((u32)(*(s32 *)(screen_w + slot * 56 + 456)) << 30 >> 31) != 0) {
            if (*(s16 *)(screen_w + slot * 56 + 454) < 60) {
                if (get_g_zyotai_s(slot) != 4) {
                    if ((u32)sp7C[slot] >= 17) {
                        if (rand() % 100 < 50) {
                            i++;
                            if (date_sasoi_judge_tbl0 != slot) {
                                *arg0 = (s8)slot;
                                date_sasoi_judge_tbl0 = (s8)slot;
                                return 1;
                            }
                            goto next;
                        }
                    }
                }
            }
        }
        i++;
next:
        s2++;
    } while (i != 11);

    return 0;
}

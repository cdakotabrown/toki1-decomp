/* @SymbolStart: 0x8013D974 */
/* @SymbolSize: 148 */
/* Function: date_basyo12_girl2_step */
/* Overlay: DATE */
/* Address: 0x8013D974 (cross-prologue head) / 0x8013D97C (body) */
/* Size: 148 bytes (37 instructions) */
#include "types.h"

extern void bustup_face_change(s32 a);
extern void param_clamp(void);
extern void goto_sub_sub_step(s32 a);
extern void date_event_sched(void);

extern u8 now_girl;
extern s16 now_serifu;
extern volatile s16 girl2_tokimeki;
extern volatile s16 girl2_yuukou;
extern volatile s16 girl2_syoushin;

void date_basyo12_girl2_step(void)
{
    if (now_girl == 2) {
        bustup_face_change(4);
        now_serifu = 5;
        girl2_tokimeki = girl2_tokimeki + 1;
        girl2_yuukou = girl2_yuukou + 1;
        girl2_syoushin = girl2_syoushin - 20;
        param_clamp();
        goto_sub_sub_step(94);
    } else {
        date_event_sched();
    }
}

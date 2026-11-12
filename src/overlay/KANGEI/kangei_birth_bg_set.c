/* kangei_birth_bg_set - KANGEI overlay, Address: 0x80138E6C, Size: 240 bytes (60 insns) */
/* @SymbolSize: 240 */
/* @CompileFlags: -Wo,-notail */
#include "types.h"

extern s32 get_g_zyotai_s(s32 a0);
extern void dec_bg_cache_step(s32 a0);
extern void event_log_add(s32 a0, s32 a1);
extern void sub_sub_step_up(void);

extern u16 basyo_pack;
extern u8 girl0_basyo;
extern s32 serifu_no;

void kangei_birth_bg_set(void)
{
    s32 result;
    u8 key;
    u8 stackKey;
    u8 v1;
    s32 counter;

    result = get_g_zyotai_s(0);
    key = (u8)(result & 0x7F);

    if (((u32)girl0_basyo >> 4) == ((u32)basyo_pack >> 12)) {
        dec_bg_cache_step(17065);
        stackKey = key;
        v1 = stackKey;
        if ((u32)v1 < 2U) {
            counter = serifu_no;
            if (counter < 2) {
                event_log_add(10, 48);
                goto tail;
            }
        }
        event_log_add(10, 45);
        goto tail;
    }

    dec_bg_cache_step(17037);
    stackKey = key;
    v1 = stackKey;
    if ((u32)v1 < 2U) {
        counter = serifu_no;
        if (counter < 2) {
            event_log_add(10, 49);
            goto tail;
        }
    }
    event_log_add(10, 62);

tail:
    sub_sub_step_up();
}

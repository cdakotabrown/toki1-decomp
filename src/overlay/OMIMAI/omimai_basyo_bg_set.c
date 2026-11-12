/* @SymbolStart: 0x80134384 */
/* @SymbolSize: 676 */
/* @RodataBase: 0x801349F0 */
/* omimai_basyo_bg_set - OMIMAI overlay, Address: 0x8013438C, Size: 668 bytes (167 insns) */
/* @PostProc: patch_insn_reg_field 0x108:rt:v0:t8 0x10c:rs:v0:t8 0x10c:rt:v0:t8 0x114:rd:t8:t9 0x114:rt:v0:t8 0x118:rs:t8:t9 0x120:rt:t8:t9 */
#include "types.h"

extern volatile u8 now_month;
extern volatile u16 basyo_pack;
extern volatile s32 ba_string;
extern s32 sched_icon;

extern volatile u8 omimai_basyo_bg_set_str0;
extern volatile u8 omimai_basyo_bg_set_str1;
extern volatile u8 omimai_basyo_bg_set_str2;
extern volatile u8 omimai_basyo_bg_set_str3;
extern volatile u8 omimai_basyo_bg_set_str4;
extern volatile u8 omimai_basyo_bg_set_str5;
extern volatile u8 omimai_basyo_bg_set_str6;
extern volatile u8 omimai_basyo_bg_set_str7;
extern volatile u8 omimai_basyo_bg_set_str8;
extern volatile u8 omimai_basyo_bg_set_str9;
extern volatile u8 omimai_basyo_bg_set_str10;
extern volatile u8 omimai_basyo_bg_set_str11;
extern volatile u8 omimai_basyo_bg_set_str12;
extern volatile u8 omimai_basyo_bg_set_str13;

extern void strcpy(s32 *, void *);
extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void omimai_basyo_bg_set(void)
{
    s32 t6_val = sched_icon;
    u32 t7;
    u8 v0;
    u32 t9;

    t7 = t6_val & 0xF;
    switch (t7) {
        case 0:
            strcpy(&ba_string, &omimai_basyo_bg_set_str0);
            dec_bg_cache_step(16572);
            break;
        case 1:
            strcpy(&ba_string, &omimai_basyo_bg_set_str1);
            v0 = now_month;
            if ((u32)v0 >= 6 && (u32)v0 < 10) {
                dec_bg_cache_step(16461);
            } else {
                dec_bg_cache_step(16469);
            }
            break;
        case 2:
            strcpy(&ba_string, &omimai_basyo_bg_set_str2);
            dec_bg_cache_step(16497);
            break;
        case 3:
            strcpy(&ba_string, &omimai_basyo_bg_set_str3);
            dec_bg_cache_step(16716);
            break;
        case 4:
            t9 = (u32)basyo_pack >> 12;
            switch (t9) {
                case 0:
                    strcpy(&ba_string, &omimai_basyo_bg_set_str4);
                    dec_bg_cache_step(16572);
                    break;
                case 1:
                    strcpy(&ba_string, &omimai_basyo_bg_set_str5);
                    dec_bg_cache_step(16562);
                    break;
                case 2:
                    strcpy(&ba_string, &omimai_basyo_bg_set_str6);
                    dec_bg_cache_step(16582);
                    break;
                case 3:
                    strcpy(&ba_string, &omimai_basyo_bg_set_str7);
                    dec_bg_cache_step(16592);
                    break;
                case 4:
                    strcpy(&ba_string, &omimai_basyo_bg_set_str8);
                    dec_bg_cache_step(16601);
                    break;
                case 5:
                    strcpy(&ba_string, &omimai_basyo_bg_set_str9);
                    dec_bg_cache_step(16610);
                    break;
                case 6:
                case 7:
                case 8:
                    strcpy(&ba_string, &omimai_basyo_bg_set_str10);
                    dec_bg_cache_step(16535);
                    break;
                case 9:
                    strcpy(&ba_string, &omimai_basyo_bg_set_str11);
                    dec_bg_cache_step(16544);
                    break;
                default:
                    strcpy(&ba_string, &omimai_basyo_bg_set_str12);
                    dec_bg_cache_step(16553);
                    break;
            }
            break;
        default:
            strcpy(&ba_string, &omimai_basyo_bg_set_str13);
            dec_bg_cache_step(16453);
            break;
    }
    sub_sub_step_up();
}

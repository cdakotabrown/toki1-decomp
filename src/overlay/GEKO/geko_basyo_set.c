/* @RodataBase: 0x801447FC */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=8 */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x44 */
/* geko_basyo_set -- GEKO overlay, Address: 0x80136790, Size: 840 bytes (210 insns) */
#include "types.h"

extern void device_port_init(s32 a0, s32 a1);
extern void strcpy(u8 *dst, u8 *src);
extern void dec_bg_cache_step(s32 a0);
extern void event_log_add(s32 a0, s32 a1);
extern void sub_sub_step_up(void);

extern u8  geko_girl_flag;
extern u8  geko_reserved_flag;
extern volatile u8 geko_sv_f10_flag;
extern u16 basyo_pack;
extern u8  day_flag;
extern u8  now_girl;

extern u8 ba_string[];

extern u8 geko_basyo_set_str0[];
extern u8 geko_basyo_set_str1[];
extern u8 geko_basyo_set_str2[];
extern u8 geko_basyo_set_str3[];
extern u8 geko_basyo_set_str4[];
extern u8 geko_basyo_set_str5[];
extern u8 geko_basyo_set_str6[];
extern u8 geko_basyo_set_str7[];
extern u8 geko_basyo_set_str8[];
extern u8 geko_basyo_set_str9[];
extern u8 geko_basyo_set_str10[];
extern u8 geko_basyo_set_str11[];
extern u8 geko_basyo_set_str12[];
extern u8 geko_basyo_set_str13[];
extern u8 geko_basyo_set_str14[];
extern u8 geko_basyo_set_str15[];

void geko_basyo_set(void)
{
    s32 sel;
    s32 a1;

    device_port_init(1, 512);

    if (geko_girl_flag != 0) {
        goto flags;
    }

    if (geko_reserved_flag == 0) {
        strcpy(ba_string, geko_basyo_set_str0);
        dec_bg_cache_step(16453);
        event_log_add(10, 37);
        goto end;
    }

    sel = (u32)(u16)basyo_pack >> 12;
    switch ((u32)sel) {
    case 0:
        strcpy(ba_string, geko_basyo_set_str1);
        dec_bg_cache_step(16572);
        break;
    case 1:
        strcpy(ba_string, geko_basyo_set_str2);
        dec_bg_cache_step(16562);
        break;
    case 2:
        strcpy(ba_string, geko_basyo_set_str3);
        dec_bg_cache_step(16582);
        break;
    case 3:
        strcpy(ba_string, geko_basyo_set_str4);
        dec_bg_cache_step(16592);
        break;
    case 4:
        strcpy(ba_string, geko_basyo_set_str5);
        dec_bg_cache_step(16601);
        break;
    case 5:
        strcpy(ba_string, geko_basyo_set_str6);
        dec_bg_cache_step(16610);
        break;
    case 6:
        strcpy(ba_string, geko_basyo_set_str7);
        dec_bg_cache_step(17046);
        break;
    case 7:
        strcpy(ba_string, geko_basyo_set_str8);
        dec_bg_cache_step(17046);
        break;
    case 8:
        strcpy(ba_string, geko_basyo_set_str9);
        dec_bg_cache_step(17046);
        break;
    case 9:
        strcpy(ba_string, geko_basyo_set_str10);
        dec_bg_cache_step(16544);
        break;
    default:
        strcpy(ba_string, geko_basyo_set_str11);
        dec_bg_cache_step(16553);
        break;
    }

    a1 = (((u32)(u16)basyo_pack >> 12) + 51) & 0xFF;
    event_log_add(10, a1);
    goto end;

flags:
    if (geko_sv_f10_flag != 0) {
        goto chk_char;
    }
    if ((day_flag & 0x20) == 0) {
        goto chk2;
    }
chk_char:
    if (now_girl != 0) {
        goto altA;
    }
    strcpy(ba_string, geko_basyo_set_str12);
    dec_bg_cache_step(16726);
    event_log_add(10, 40);
    goto end;
altA:
    strcpy(ba_string, geko_basyo_set_str13);
    dec_bg_cache_step(17122);
    event_log_add(10, 39);
    goto end;
chk2:
    if (now_girl != 0) {
        goto altB;
    }
    strcpy(ba_string, geko_basyo_set_str14);
    dec_bg_cache_step(16716);
    event_log_add(10, 47);
    goto end;
altB:
    strcpy(ba_string, geko_basyo_set_str15);
    dec_bg_cache_step(17112);
    event_log_add(10, 38);

end:
    sub_sub_step_up();
}

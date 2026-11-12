/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_80055E94.py */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* Function: func_80055E94 */
/* Address: 0x80055E94 */
/* Size: 1352 bytes (338 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
#include "types.h"

typedef struct { u8 data[64]; } S64;
typedef struct { u8 data[8]; } S8;
typedef struct { u8 data[256]; } S256;
typedef struct { u32 data[31]; } S124;
typedef struct { u32 data[14]; } S56;
typedef struct { u32 w; } S4;

extern s32 save_check(s32);
extern void bzero(u8 *, s32);
extern u8 rec_table_trim(u8 *, u8 *, s32);

extern u8 album_slot;
extern u8 album_chr[];

extern u8 voice_level;
extern u8 season_no;
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 now_youbi;
extern u16 pass_days;
extern u8 cal_day_kind[128];
extern u8 player_rec[124];
extern u8 girl_tbl[896];
extern u8 mascot_flag2[16];
extern u8 reserved_date[32];
extern u8 ev_flag00[256];
extern u8 bukatu_rec[48];
extern u8 basyo_hist[4];
extern u8 last_date_spot[16];
extern u8 last_date_spot_timer[16];
extern u8 last_date_spot_flag[16];
extern u8 rec_tbl[2048];
extern u8 girl_no;
extern u8 rec_idx;
extern u8 now_girl;
extern u32 mouse_pad_st;
extern u32 sched_icon;
extern u8 icon_mode;
extern u8 icon_num;
extern u8 bustup_sw;
extern u8 csr_speed;
extern u8 click_mode;
extern u8 click_speed;
extern u8 mouse_btn_mode;
extern u8 menu_mode;

s32 func_80055E94(s32 arg0) {
    u8 *slot;
    u8 *slot2;
    u8 slotIdx;
    u8 *src;
    u8 *dst;
    u8 *end;

    if (save_check(0) == -1) {
        return -1;
    }

    slotIdx = album_slot & 1;
    slot = &album_chr[(u32)slotIdx * 3840];

    if (slot[517] == 0) {
        return -1;
    }
    if (slot[518] == 0) {
        return -1;
    }

    season_no = slot[516];
    now_year = slot[517];
    now_month = slot[518];
    now_day = slot[519];
    now_youbi = slot[520];

    slot2 = &album_chr[(u32)slotIdx * 3840];
    pass_days = *(u16 *)&slot[522];

    dst = (u8 *)cal_day_kind;
    src = &slot2[524];
    end = &cal_day_kind[128];
    do {
        *(S64 *)dst = *(S64 *)src;
        dst += 64;
        src += 64;
    } while ((u32)dst < (u32)end);

    *(S124 *)player_rec = *(S124 *)&slot[652];

    dst = girl_tbl;
    src = &slot2[776];
    end = &girl_tbl[896];
    do {
        *(S56 *)dst = *(S56 *)src;
        dst += 56;
        src += 56;
    } while ((u32)dst < (u32)end);

    dst = mascot_flag2;
    src = &slot2[1672];
    end = &mascot_flag2[16];
    do {
        *dst = *src;
        dst++;
        src++;
    } while ((u32)dst < (u32)end);

    dst = reserved_date;
    src = &slot2[1688];
    end = &reserved_date[32];
    do {
        *(S4 *)dst = *(S4 *)src;
        dst += 4;
        src += 4;
    } while ((u32)dst < (u32)end);

    dst = bukatu_rec;
    src = &slot2[1720];
    end = &bukatu_rec[48];
    do {
        *(S4 *)dst = *(S4 *)src;
        dst += 4;
        src += 4;
    } while ((u32)dst < (u32)end);

    *(S256 *)ev_flag00 = *(S256 *)&slot[1768];

    dst = basyo_hist;
    src = &slot2[2024];
    end = &basyo_hist[4];
    do {
        *dst = *src;
        dst++;
        src++;
    } while ((u32)dst < (u32)end);

    dst = last_date_spot;
    src = &slot2[2028];
    end = &last_date_spot[16];
    do {
        *dst = *src;
        dst++;
        src++;
    } while ((u32)dst < (u32)end);

    dst = last_date_spot_timer;
    src = &slot2[2044];
    end = &last_date_spot_timer[16];
    do {
        *dst = *src;
        dst++;
        src++;
    } while ((u32)dst < (u32)end);

    dst = last_date_spot_flag;
    src = &slot2[2060];
    end = &last_date_spot_flag[16];
    do {
        *dst = *src;
        dst++;
        src++;
    } while ((u32)dst < (u32)end);

    bzero(rec_tbl, 2048);

    {
        u8 *s0, *s1, *s2, *s3;
        u8 *d0, *d1, *d2, *d3;
        s0 = &slot2[2076];
        s1 = &slot2[2084];
        s2 = &slot2[2092];
        s3 = &slot2[2100];
        d0 = &rec_tbl[0];
        d1 = &rec_tbl[8];
        d2 = &rec_tbl[16];
        d3 = &rec_tbl[24];
        end = &rec_tbl[2048];
        do {
            *(S8 *)d0 = *(S8 *)s0;
            *(S8 *)d1 = *(S8 *)s1;
            *(S8 *)d2 = *(S8 *)s2;
            *(S8 *)d3 = *(S8 *)s3;
            s0 += 32; s1 += 32; s2 += 32; s3 += 32;
            d0 += 32; d1 += 32; d2 += 32; d3 += 32;
        } while (d3 != end);
    }

    rec_idx = rec_table_trim(&now_girl, &girl_no, 1);

    icon_mode = slot[4124];
    icon_num = slot[4125];
    menu_mode = slot[4126];
    bustup_sw = slot[4127];
    voice_level = slot[4128];
    *(S4 *)&sched_icon = *(S4 *)&slot[4132];
    csr_speed = slot[4136];
    click_mode = slot[4137];
    click_speed = slot[4138];
    mouse_btn_mode = slot[4139];
    *(S4 *)&mouse_pad_st = *(S4 *)&slot[4140];

    return 1;
}

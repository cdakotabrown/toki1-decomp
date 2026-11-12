/* Address: 0x80055E94 */
/* Size: 1352 bytes (338 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=1 IDO52_FORCE_WEB_COLOR=15:t5,51:ra,195:v1,106:a0,156:a1,159:a2,162:a3,189:t0,191:t1,193:t2 IDO52_UGEN_TEMP_REG=21:t8,25:t7,24:t6,28:t8,29:t7 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --map '40:44' */
#include "types.h"

typedef struct { u8 b[64]; } B64;
typedef struct { u32 w[31]; } W124;
typedef struct { u32 w[14]; } W56;
typedef struct { u8 b[8]; } B8;
typedef struct { u8 b[256]; } B256;

typedef struct { u32 w; } W4;

typedef struct {
    u32 sum;
    u8 s516;
    u8 s517;
    u8 s518;
    u8 s519;
    u8 s520;
    u8 pad521;
    u16 h522;
    B64 blk[2];
    W124 w652;
    W56 r776[16];
    u8 got[16];
    W4 w1688[8];
    W4 w1720[12];
    B256 b1768;
    u8 a2024[4];
    u8 a2028[16];
    u8 a2044[16];
    u8 a2060[16];
    B8 e2076[256];
    u8 f4124;
    u8 f4125;
    u8 f4126;
    u8 f4127;
    u8 f4128;
    u8 pad4129[3];
    W4 w4132;
    u8 f4136;
    u8 f4137;
    u8 f4138;
    u8 f4139;
    W4 w4140;
    u8 pad4144[208];
} Chr;

typedef struct {
    u8 hdr[512];
    Chr slot[4];
} ChrTab;

typedef struct {
    u8 pad[1340];
    u8 got[16];
    u8 pad2[32];
    B256 b256;
    u8 pad3[188];
    u8 a[4];
    u8 b[16];
    u8 c[16];
    u8 d[16];
} Save;

extern ChrTab album_chr;
extern Save screen_w;
extern u8 voice_level;
extern u8 season_no;
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 now_youbi;
extern u16 pass_days;
extern B64 cal_day_kind[2];
extern W124 player_rec;
extern W56 girl_tbl[16];
extern W4 reserved_date[8];
extern W4 bukatu_rec[12];
extern B8 rec_tbl[256];
extern u8 girl_no;
extern W4 mouse_pad_st;
extern W4 sched_icon;
extern u8 rec_idx;
extern u8 now_girl;
extern u8 icon_mode;
extern u8 icon_num;
extern u8 bustup_sw;
extern u8 csr_speed;
extern u8 click_mode;
extern u8 click_speed;
extern u8 mouse_btn_mode;
extern u8 menu_mode;
extern volatile u8 album_slot;

extern s32 save_check();
extern void bzero();
extern s32 rec_table_trim();

s32 save_game_restore(arg0)
s32 arg0;
{
    s32 pad;
    u32 i;
    s32 sel;
    ChrTab *p;
    B8 *d;

    if (save_check(0) == -1) {
        return -1;
    }
    sel = album_slot & 1;
    p = (ChrTab *)((u8 *)&album_chr + sel * 3840);
    if (p->slot[0].s517 == 0) {
        return -1;
    }
    if (p->slot[0].s518 == 0) {
        return -1;
    }
    season_no = p->slot[0].s516;
    now_year = p->slot[0].s517;
    now_month = p->slot[0].s518;
    now_day = p->slot[0].s519;
    now_youbi = p->slot[0].s520;
    pass_days = p->slot[0].h522;
    for (i = 0; i < 2; i++) {
        cal_day_kind[i] = album_chr.slot[sel].blk[i];
    }
    player_rec = p->slot[0].w652;
    for (i = 0; i < 16; i++) {
        girl_tbl[i] = album_chr.slot[sel].r776[i];
    }
    for (i = 0; i < 16; i++) {
        screen_w.got[i] = album_chr.slot[sel].got[i];
    }
    for (i = 0; i < 8; i++) {
        reserved_date[i] = album_chr.slot[sel].w1688[i];
    }
    for (i = 0; i < 12; i++) {
        bukatu_rec[i] = album_chr.slot[sel].w1720[i];
    }
    screen_w.b256 = p->slot[0].b1768;
    for (i = 0; i < 4; i++) {
        screen_w.a[i] = album_chr.slot[sel].a2024[i];
    }
    for (i = 0; i < 16; i++) {
        screen_w.b[i] = album_chr.slot[sel].a2028[i];
    }
    for (i = 0; i < 16; i++) {
        screen_w.c[i] = album_chr.slot[sel].a2044[i];
    }
    for (i = 0; i < 16; i++) {
        screen_w.d[i] = album_chr.slot[sel].a2060[i];
    }
    bzero(rec_tbl, 2048);
    d = rec_tbl;
    for (i = 0; i < 256; i++) {
        d[i] = album_chr.slot[sel].e2076[i];
    }
    rec_idx = rec_table_trim(&now_girl, &girl_no, 1);
    icon_mode = p->slot[0].f4124;
    icon_num = p->slot[0].f4125;
    menu_mode = p->slot[0].f4126;
    bustup_sw = p->slot[0].f4127;
    voice_level = p->slot[0].f4128;
    sched_icon = p->slot[0].w4132;
    csr_speed = p->slot[0].f4136;
    click_mode = p->slot[0].f4137;
    click_speed = p->slot[0].f4138;
    mouse_btn_mode = p->slot[0].f4139;
    mouse_pad_st = p->slot[0].w4140;
    return 1;
}

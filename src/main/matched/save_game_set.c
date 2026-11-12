/* Address: 0x80055950 */
/* Size: 1152 bytes (288 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x80055950 */
/* @SymbolSize: 1152 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x438 --offset2 0x43c */
/* @PostProc: swap_instructions --offset1 0x438 --offset2 0x440 */
/* @PostProc: patch_insn_reg_field 0x25c:rt:t7:t0 0x260:rs:t7:t0 0x260:rt:t7:t0 0x264:rs:t7:t0 0x264:rt:t0:t3 0x268:rd:t8:t2 0x26c:rs:t7:t0 0x270:rs:t7:t0 0x274:rs:t7:t0 0x274:rt:t7:t0 0x278:rs:t8:t2 0x27c:rs:t8:t2 0x280:rs:t7:t0 0x284:rs:t7:t0 0x288:rs:t8:t2 0x288:rt:t8:t2 0x28c:rs:t8:t2 0x290:rs:t8:t2 0x294:rs:t7:t0 0x298:rs:t7:t0 0x2a0:rs:t8:t2 0x2a4:rs:t7:t0 0x2a4:rt:t0:t3 0x2a8:rs:t8:t2 0x2ac:rs:t7:t0 0x2b0:rs:t7:t0 0x2bc:rs:t8:t2 0x2cc:rs:t8:t2 0x2d0:rt:t1:t5 0x2e4:rt:t1:t5 0x2fc:rt:t3:t6 0x310:rt:t3:t6 0x328:rt:t5:t7 0x33c:rt:t5:t7 0x354:rt:t2:t4 0x368:rt:t2:t4 0x388:rt:t4:t8 0x38c:rt:t4:t8 0x3a0:rt:t4:t8 0x3ac:rt:t4:t8 0x3b0:rt:t9:t3 0x3b4:rs:t9:t3 0x3b4:rt:t9:t3 0x3bc:rt:t9:t3 0x3c4:rt:t7:t2 0x3cc:rs:t7:t2 0x3cc:rt:t7:t2 0x3d0:rt:t8:t5 0x3d4:rt:t7:t2 0x3d8:rs:t8:t5 0x3d8:rt:t8:t5 0x3dc:rt:t1:t6 0x3e0:rt:t8:t5 0x3e4:rs:t1:t6 0x3e4:rt:t1:t6 0x3e8:rt:t3:t7 0x3ec:rs:t3:t7 0x3ec:rt:t3:t7 0x3f0:rt:t1:t6 0x3f4:rs:t3:t7 0x3f8:rt:t6:t9 0x400:rs:t6:t9 0x400:rt:t6:t9 0x404:rt:t4:t8 0x408:rt:t6:t9 0x40c:rs:t4:t8 0x40c:rt:t4:t8 0x410:rt:t9:t3 0x414:rt:t4:t8 0x418:rs:t9:t3 0x418:rt:t9:t3 0x420:rt:t9:t3 0x428:rt:t7:t2 0x42c:rs:t7:t2 0x42c:rt:t7:t2 0x434:rs:t7:t2 0x438:rt:a1:a0 0x43c:rd:a0:a1 0x44c:rs:a1:a0 0x44c:rt:t5:t4 0x450:rs:a1:a0 0x450:rt:t3:t7 0x454:rs:a1:a0 0x454:rt:t2:t1 0x458:rs:a0:a1 0x458:rt:t5:t4 0x458:rd:a0:a1 0x45c:rs:a1:a0 0x45c:rt:t6:t9 0x460:rs:a0:a1 0x460:rt:t3:t7 0x460:rd:a0:a1 0x468:rs:a0:a1 0x468:rt:t2:t1 0x468:rd:a0:a1 0x46c:rs:a1:a0 0x46c:rt:a1:a0 0x474:rs:a0:a1 0x474:rt:t6:t9 0x474:rd:a0:a1 0x47c:rt:a0:a1 */
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
extern W4 mouse_pad_st;
extern W4 sched_icon;
extern u8 icon_mode;
extern u8 icon_num;
extern u8 bustup_sw;
extern u8 csr_speed;
extern u8 click_mode;
extern u8 click_speed;
extern u8 mouse_btn_mode;
extern u8 menu_mode;

void save_game_set(sel)
s32 sel;
{
    u32 i;
    u32 sum;
    u8 *p;

    album_chr.slot[sel].s516 = season_no;
    album_chr.slot[sel].s517 = now_year;
    album_chr.slot[sel].s518 = now_month;
    album_chr.slot[sel].s519 = now_day;
    album_chr.slot[sel].s520 = now_youbi;
    album_chr.slot[sel].h522 = pass_days;
    album_chr.slot[sel].blk[0] = cal_day_kind[0];
    album_chr.slot[sel].blk[1] = cal_day_kind[1];
    album_chr.slot[sel].w652 = player_rec;
    for (i = 0; i < 16; i++) {
        album_chr.slot[sel].r776[i] = girl_tbl[i];
    }
    for (i = 0; i < 16; i++) {
        album_chr.slot[sel].got[i] = screen_w.got[i];
    }
    for (i = 0; i < 8; i++) {
        album_chr.slot[sel].w1688[i] = reserved_date[i];
    }
    for (i = 0; i < 12; i++) {
        album_chr.slot[sel].w1720[i] = bukatu_rec[i];
    }
    album_chr.slot[sel].b1768 = screen_w.b256;
    for (i = 0; i < 4; i++) {
        album_chr.slot[sel].a2024[i] = screen_w.a[i];
    }
    for (i = 0; i < 16; i++) {
        album_chr.slot[sel].a2028[i] = screen_w.b[i];
    }
    for (i = 0; i < 16; i++) {
        album_chr.slot[sel].a2044[i] = screen_w.c[i];
    }
    for (i = 0; i < 16; i++) {
        album_chr.slot[sel].a2060[i] = screen_w.d[i];
    }
    for (i = 0; i < 256; i++) {
        album_chr.slot[sel].e2076[i] = rec_tbl[i];
    }
    album_chr.slot[sel].f4124 = icon_mode;
    album_chr.slot[sel].f4125 = icon_num;
    album_chr.slot[sel].f4126 = menu_mode;
    album_chr.slot[sel].f4127 = bustup_sw;
    album_chr.slot[sel].f4128 = voice_level;
    album_chr.slot[sel].w4132 = sched_icon;
    album_chr.slot[sel].f4136 = csr_speed;
    album_chr.slot[sel].f4137 = click_mode;
    album_chr.slot[sel].f4138 = click_speed;
    album_chr.slot[sel].f4139 = mouse_btn_mode;
    album_chr.slot[sel].w4140 = mouse_pad_st;
    sum = 0;
    p = (u8 *)&album_chr.slot[sel];
    for (i = 4; i < 3840; i++) {
        sum += p[i];
    }
    album_chr.slot[sel].sum = sum;
}

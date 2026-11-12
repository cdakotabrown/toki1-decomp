/* Function: save_sys_set */
/* Address: 0x800553B0 */
/* Size: 780 bytes (195 instructions) -- the recorded 776 cut the `jr ra` */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 (-Wo,-loopunroll,0) */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic1 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=8:t0,14:t3,18:t3 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_insn_reg_field 0x220:rt:t3:v0 0x224:rs:t3:v0 0x224:rt:t3:v0 0x22c:rs:t3:v0 */
/* @PostProc: patch_insn_reg_field 0x25c:rt:t1:t3 0x260:rs:t1:t3 0x260:rt:t1:t3 0x268:rt:t2:t1 0x26c:rs:t2:t1 0x26c:rt:t2:t1 0x270:rt:t1:t3 0x278:rt:t4:t2 0x27c:rs:t4:t2 0x27c:rt:t4:t2 0x280:rt:t2:t1 0x2a0:rt:t4:t2 */
/* @PostProc: patch_insn_reg_field 0x2a4:rt:a1:t4 0x2a8:rt:t8:t5 0x2ac:rt:a1:t4 0x2b0:rd:t5:t7 0x2b0:rt:a1:t4 0x2b4:rd:t6:t7 0x2b4:rt:t5:t7 0x2b8:rs:t8:t5 0x2b8:rt:t8:t5 0x2bc:rs:t6:t7 0x2bc:rt:t8:t5 */
/* @PostProc: patch_insn_reg_field 0x2c0:rt:v1:v0 0x2c4:rd:v0:v1 0x2d0:rt:t9:t6 0x2d0:rs:v1:v0 0x2d4:rt:t0:t8 0x2d4:rs:v1:v0 0x2d8:rt:t3:t9 0x2d8:rs:v1:v0 0x2dc:rd:v0:v1 0x2dc:rs:v0:v1 0x2dc:rt:t9:t6 0x2e0:rt:t1:t0 0x2e0:rs:v1:v0 0x2e4:rd:v0:v1 0x2e4:rs:v0:v1 0x2e4:rt:t0:t8 0x2ec:rd:v0:v1 0x2ec:rs:v0:v1 0x2ec:rt:t3:t9 0x2f0:rs:v1:v0 0x2f0:rt:v1:v0 0x2f8:rd:v0:v1 0x2f8:rs:v0:v1 0x2f8:rt:t1:t0 0x2fc:rt:v0:v1 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x2fc --offset2 0x300 */
/* @PostProc: swap_instructions --offset1 0x304 --offset2 0x308 */
#include "types.h"
#include "tokimeki/card_file.h"

typedef struct { u32 w[5]; } Words5;
typedef struct { u32 w[6]; } Words6;

extern u8 album_work[];
extern u8 album_status[];
extern u8 mascot_flag[];
extern u8 sys_flags;
extern u8 sys_song_no;
extern u8 last_album;
extern u8 opt_flag;
extern u8 save_rec_cnt;
extern Words5 tt_config;
extern Words6 taco_rec;
extern u8 save_buf[];
extern u8 name_buf0[];
extern u8 name_buf1[];
extern u8 name_buf2[];
extern u8 save_blk0[];
extern u8 save_blk1[];
extern u8 save_blk2[];
extern u8 save_blk3[];
extern u8 save_blk4[];
extern u8 save_blk5[];
extern u8 mascot_x_sav[];
extern u8 cursor_sav;
extern u8 cursor_sav_hi;
extern u8 cursor_sav_cw;
extern u8 sys_idx_tbl[];

extern Words5 album_chr_rec0_sys_tt_config_sav0;
extern Words6 album_chr_rec0_sys_taco_rec_sav0;
extern u8 save_sys_buf1[];
extern u8 save_sys_buf2[];
extern u8 save_sys_buf3[];
extern u8 save_sys_buf4[];
extern u8 save_sys_buf5[];
extern u8 save_sys_buf6[];
extern u8 save_sys_buf7[];
extern u8 save_sys_buf8[];
extern u8 save_sys_buf9[];
extern u8 icon_xy[];
extern u8 mascot_x[];
extern u8 mascot_hi;
extern u8 mascot_cw;
extern u8 save_sys_buf10[];
extern u8 album_chr[];

extern s16 cursor_work_copy_tbl0;

extern void memcpy(u8 *, u8 *, s32);

void save_sys_set(s32 arg0) {
    u8 ctrlByte;
    s32 i;
    u8 *base;
    s32 sum;
    u8 *ptr;
    s32 offset;

    ctrlByte = last_album;
    album_work[ctrlByte + 4] = 1;

    memcpy((&(*(CARD_FILE *)album_chr).rec[0].sys.album_status_sav[0]), album_status, 16);
    memcpy((&(*(CARD_FILE *)album_chr).rec[0].sys.mascot_flag_sav[0]), mascot_flag, 16);

    (*(CARD_FILE *)album_chr).rec[0].sys.sys_flags_sav = sys_flags;
    (*(CARD_FILE *)album_chr).rec[0].sys.sys_song_no_sav = sys_song_no;

    if (last_album != 0) {
        (*(CARD_FILE *)album_chr).rec[0].sys.last_album_sav = last_album % 15U;
    }

    album_chr_rec0_sys_tt_config_sav0 = tt_config;
    album_chr_rec0_sys_taco_rec_sav0 = taco_rec;

    (*(CARD_FILE *)album_chr).rec[0].sys.opt_flag_sav = opt_flag;
    (*(CARD_FILE *)album_chr).rec[0].sys.rec_cnt = save_rec_cnt;

    memcpy((&(*(CARD_FILE *)album_chr).rec[0].sys.save_buf_sav[0]), save_buf, 2048);
    memcpy(save_sys_buf1, name_buf0, 16);
    memcpy(save_sys_buf2, name_buf1, 16);
    memcpy(save_sys_buf3, name_buf2, 16);
    memcpy(save_sys_buf4, save_blk0, 128);
    memcpy(save_sys_buf5, save_blk1, 128);
    memcpy(save_sys_buf6, save_blk2, 256);
    memcpy(save_sys_buf7, sys_idx_tbl, 16);
    memcpy(save_sys_buf8, save_blk3, 576);
    memcpy(save_sys_buf9, save_blk4, 256);

    if (last_album != 0) {
        memcpy(save_blk5, icon_xy, 64);
        memcpy(mascot_x_sav, mascot_x, 4);
        cursor_sav = (u8)(s8)cursor_work_copy_tbl0;
        cursor_sav_hi = mascot_hi;
        cursor_sav_cw = mascot_cw;
        memcpy(save_sys_buf10, save_blk5, 256);
    }

    offset = (arg0 << 4) - arg0;
    base = album_chr + (offset << 8);
    ptr = base + 516;
    sum = 0;
    i = 4;
    do {
        sum += ptr[0];
        sum += ptr[1];
        sum += ptr[2];
        sum += ptr[3];
        i += 4;
        ptr += 4;
    } while (i != 3840);
    *(s32 *)(base + 512) = sum;
}

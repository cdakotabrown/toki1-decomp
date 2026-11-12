/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_AS1_TIE_SRC_ORDER=bb:3 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x84 */
/* Function: save_work_restore - loads one save slot's payload into the working */
/* Address: 0x800556BC */
/* Size: 660 bytes (165 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: patch_insn_reg_field 0x8c:rt:t8:t9 0xac:rt:t7:t8 0xb4:rt:t7:t8 0xc4:rt:t7:t8 0xcc:rt:t7:t8 0xdc:rs:t8:t9 0xdc:rt:t8:t9 0xe4:rs:t8:t9 0xe4:rt:t9:t0 0xe8:rs:t9:t0 0xf4:rt:t0:t1 0xf8:rs:t0:t1 0xf8:rt:t0:t1 0xfc:rs:t0:t1 0x100:rt:t2:t4 0x108:rs:t0:t1 0x108:rt:t2:t4 0x114:rs:t0:t1 0x118:rt:t2:t4 0x120:rs:t0:t1 0x120:rt:t2:t4 0x12c:rs:t0:t1 0x130:rt:t2:t4 0x138:rs:t0:t1 0x138:rt:t2:t4 0x13c:rt:t3:t7 0x144:rt:t3:t7 0x148:rt:t4:t6 0x158:rt:t4:t6 0x254:rt:t6:t5 0x258:rs:t6:t5 0x258:rt:t6:t5 0x260:rt:t5:t8 0x264:rs:t5:t8 0x264:rt:t5:t8 0x268:rt:t6:t5 0x270:rt:t7:t9 0x274:rs:t7:t9 0x274:rt:t7:t9 0x278:rt:t5:t8 0x280:rt:t7:t9 */
#include "types.h"

extern s32 save_check(s32 arg);
extern void memcpy(void *dst, void *src, s32 size);

typedef struct { u32 a, b, c, d, e; } Block5;
typedef struct { u32 a, b, c, d, e, f; } Block6;

extern u8 album_slot;
extern u8 mascot_flag[16];
extern u8 sys_flags;
extern u8 sys_song_no;
extern u8 last_album;
extern u8 opt_flag;
extern u8 save_rec_cnt;
extern Block5 tt_config;
extern u8 bunkaken_flags;
extern Block6 taco_rec;
extern u8 save_buf[2048];
extern u8 name_buf0[16];
extern u8 name_buf1[16];
extern u8 name_buf2[16];
extern u8 save_blk0[128];
extern u8 save_blk1[128];
extern u8 save_blk2[256];
extern u8 save_blk3[576];
extern u8 save_blk4[256];
extern u8 save_blk5[256];
extern u8 mascot_x_sav[4];
extern u8 cursor_sav;
extern u8 cursor_sav_hi;
extern u8 cursor_sav_cw;
extern u8 sys_idx_tbl[16];
extern s16 cursor_work_copy_tbl0;
extern u8 album_chr[];
extern u8 icon_xy[64];
extern u8 mascot_x[4];
extern u8 mascot_hi;
extern u8 mascot_cw;

s32 save_work_restore(s32 arg0) {
    u8 *saveSlot;
    u8 flagByte;
    u32 flagWord;
    u32 off;

    if (save_check(0) == -1) {
        return -1;
    }

    off = album_slot * 0xF00 + 0x200;
    saveSlot = album_chr + off;

    memcpy(mascot_flag, saveSlot + 20, 16);

    sys_flags = saveSlot[36];
    sys_song_no = saveSlot[37];
    last_album = saveSlot[38];

    flagByte = bunkaken_flags;
    if ((flagByte & 1) == 0) {
        tt_config = *(Block5 *)(saveSlot + 40);
    }

    flagWord = taco_rec.a;
    if ((flagWord & 1) == 0) {
        taco_rec = *(Block6 *)(saveSlot + 60);
    }

    opt_flag = saveSlot[84];
    save_rec_cnt = saveSlot[85];

    memcpy(save_buf, saveSlot + 86, 2048);
    memcpy(name_buf0, saveSlot + 2134, 16);
    memcpy(name_buf1, saveSlot + 2150, 16);
    memcpy(name_buf2, saveSlot + 2166, 16);
    memcpy(save_blk0, saveSlot + 2182, 128);
    memcpy(save_blk1, saveSlot + 2310, 128);
    memcpy(save_blk2, saveSlot + 2438, 256);
    memcpy(sys_idx_tbl, saveSlot + 2694, 16);
    memcpy(save_blk3, saveSlot + 2712, 576);
    memcpy(save_blk4, saveSlot + 3288, 256);
    memcpy(save_blk5, saveSlot + 3544, 256);

    memcpy(icon_xy, save_blk5, 64);
    memcpy(mascot_x, mascot_x_sav, 4);

    cursor_work_copy_tbl0 = (s16)cursor_sav;
    mascot_hi = cursor_sav_hi;
    mascot_cw = cursor_sav_cw;
}

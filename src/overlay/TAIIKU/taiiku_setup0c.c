/* @SymbolSize: 720 */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic0 */
/* @CompileEnv: IDO52_FORM_SITE=lda:0:150:0,lda:0xffffffa8:204:0,lda:0xffffffcc:7:0 IDO52_FORM_SPLIT=lda:0xffffffcc:204 */
/* Address: 0x80145534 */
/* Size: 720 bytes (180 insns) */
/* @PostProc: patch_stack_complete --old 192 --new 168 */
/* @PostProc: remap_stack_slots --from 104 --to 72 --from 108 --to 76 --from 140 --to 108 --from 188 --to 156 */
/* @PostProc: swap_instructions --offset1 0xd8 --offset2 0xdc */
/* @PostProc: swap_instructions --offset1 0xfc --offset2 0x100 */
/* @PostProc: swap_instructions --offset1 0x120 --offset2 0x124 */
/* @PostProc: swap_instructions --offset1 0x154 --offset2 0x158 */
/* @PostProc: patch_insn_reg_field 0x1c:rt:v0:v1 0x28:rs:v0:v1 0x28:rt:v0:v1 0x34:rt:t2:t1 0x5c:rt:t2:t1 0x64:rt:t4:t3 0x68:rs:t4:t3 0x68:rt:t4:t3 0x6c:rs:t4:t3 0x6c:rt:t1:t6 0x70:rt:t3:t2 0x74:rs:t4:t3 0x78:rs:t4:t3 0x78:rt:t4:t3 0x7c:rs:t3:t2 0x80:rs:t4:t3 0x84:rs:t3:t2 0x84:rt:t3:t2 0x88:rs:t3:t2 0x8c:rs:t4:t3 0x90:rs:t4:t3 0x90:rt:t1:t6 0x94:rs:t3:t2 0x98:rs:v0:v1 0xa4:rs:v0:v1 0xb0:rs:v0:v1 0xb8:rs:v0:v1 0xd0:rt:v1:v0 0xdc:rs:v1:v0 0xdc:rt:v1:v0 0xe0:rt:v0:v1 0xe4:rs:v1:v0 0xe4:rt:v1:v0 0xe8:rs:v1:v0 0xf0:rs:v1:v0 0xf0:rt:v0:v1 0xf4:rt:v1:v0 0x100:rs:v1:v0 0x100:rt:v1:v0 0x104:rs:v1:v0 0x104:rt:v1:v0 0x108:rs:v1:v0 0x110:rs:v1:v0 0x110:rt:v0:v1 0x114:rt:t9:t0 0x12c:rt:t9:t0 0x130:rt:t2:t9 0x144:rt:t2:t9 0x148:rt:t8:t1 0x15c:rt:t0:a0 0x160:rt:t8:t1 0x164:rt:a0:t7 0x168:rt:a1:t5 0x16c:rt:a2:t4 0x170:rt:a3:t8 0x17c:rt:a0:t7 0x180:rt:a1:t5 0x184:rt:a2:t4 0x188:rt:t0:a0 0x18c:rt:a3:t8 0x1e4:rt:t7:t6 0x1e8:rs:t7:t6 0x228:rt:t6:t3 0x22c:rs:t6:t3 0x260:rt:t5:t2 0x268:rs:t5:t2 */
#include "types.h"

typedef struct { s32 a, b, c; } Vec3;
typedef struct { s32 w[4]; } Blk16;
typedef struct { s32 w[12]; } Blk48;
typedef struct { s32 w[9]; } Blk36;

extern s32 sub_step_up();
extern s32 sub_sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 get_CD_State2();
extern s32 cd_read_start2();
extern s32 game_reset();
extern s32 search_load_tpage_buf_lock();
extern s32 class_clear_all();
extern s32 set_flag_b3_high_bit();
extern s32 date_x_taku_set();
extern s32 Fade_Out_Init();
extern s32 bri_fade_out();
extern s32 taiiku_bufs_set();
extern s32 taiiku_tbl_init();
extern void taiiku_module_init();
extern void taiiku_panel_init();
extern s32 taiiku_race_init4();

extern s32 taiiku_setup0c_pl_tpg;
extern Blk48 taiiku_setup0c_tbl0;
extern Blk36 taiiku_setup0c_tbl1;
extern u8 now_sub_sub_step;
extern s32 screen_w[];
extern s32 bukatu_tbl_end[];
extern s32 bukatu_tbl[];
extern s32 taiiku_spoon_pl_tpg;
extern s32 taiiku_spoon_sprite_tpg[12];
extern s32 taiiku_spoon_scene_tpg[];
extern s32 taiiku_spoon_buf_1;

void taiiku_setup0c(void) {
    s32 buf108[13];
    s32 buf72[9];
    s32 *p;
    s32 *so;
    s32 *dstp;
    Vec3 *vs;
    Vec3 *ve;
    Vec3 *vd;
    Blk16 *bsrc;
    Blk16 *bdst;
    u8 sel;
    s32 r;
    s32 i;
    s32 c0;
    s32 c1;
    s32 c2;
    s32 c3;

    buf108[12] = taiiku_setup0c_pl_tpg;
    sel = now_sub_sub_step;
    *(Blk48 *)buf108 = taiiku_setup0c_tbl0;
    *(Blk36 *)buf72 = taiiku_setup0c_tbl1;

    switch ((u32)sel) {
    case 0:
        date_x_taku_set(1, 1);
        p = screen_w;
        do {
            p++;
            p[1166] = 8;
        } while (p < bukatu_tbl_end);
        p = screen_w;
        do {
            p++;
            p[1198] = 8;
        } while (p < bukatu_tbl);
        taiiku_spoon_pl_tpg = buf108[12];
        so = buf108;
        dstp = taiiku_spoon_sprite_tpg;
        do {
            *dstp = *so;
            so++;
            dstp++;
        } while (so < &buf108[12]);
        taiiku_spoon_scene_tpg[0] = buf72[0];
        bsrc = (Blk16 *)(buf72 + 1);
        bdst = (Blk16 *)(&taiiku_spoon_scene_tpg[1]);
        do {
            c1 = bsrc->w[1];
            c2 = bsrc->w[2];
            c3 = bsrc->w[3];
            c0 = bsrc->w[0];
            bsrc++;
            bdst->w[1] = c1;
            bdst->w[2] = c2;
            bdst->w[3] = c3;
            bdst->w[0] = c0;
            bdst++;
        } while (bsrc != (Blk16 *)buf108);
        class_clear_all(0);
        set_flag_b3_high_bit(0);
        sub_sub_step_up();
        break;
    case 1:
        cd_read_start2(30, (s32)0x80180000, 0xBAD7);
        Fade_Out_Init();
        sub_sub_step_up();
        break;
    case 2:
        bri_fade_out();
        r = get_CD_State2();
        if (r & 1) {
            cd_read_start2(38, (s32)0x8018F000, 0xB970);
            sub_sub_step_up();
        }
        break;
    case 3:
        r = bri_fade_out();
        if (r != 0) {
            r = get_CD_State2();
            if (r & 1) {
                taiiku_tbl_init();
                taiiku_bufs_set();
                search_load_tpage_buf_lock(taiiku_spoon_pl_tpg, 1);
                for (i = 0; i != 16; i++) {
                    palette_upload_enqueue(taiiku_spoon_buf_1, (i + 16) & 0xFF, 1, 1, i);
                }
                taiiku_race_init4();
                taiiku_module_init();
                taiiku_panel_init();
                cd_read_start2(185, (s32)0x8018F000, 0xB8B7);
                sub_step_up();
            }
        }
        break;
    default:
        game_reset();
        break;
    }
}

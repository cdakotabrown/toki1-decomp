/* Address: 0x8014690C, Size: 1012 bytes (253 insns) */
/* @SymbolSize: 1012 */
/* @PostProc: patch_stack_complete --old 40 --new 56 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @PostProc: scheduler_normalizer --const-fp --store-fp --fence 0-90,91-126,127-152,153-185,186-218,219-252 */
/* @PostProc: patch_insn_reg_field 0x28:rd:v1:a0 0x38:rd:v1:a0 0x54:rt:v1:a0 0x80:rd:a0:v1 0x84:rs:a0:v1 0x88:rd:v0:t6 0x88:rt:a0:v1 0x8c:rs:a0:v1 0x90:rd:v0:t6 0x98:rs:v0:t6 0xa0:rt:v1:a0 0x20c:rt:v1:v0 0x210:rs:v1:v0 0x210:rt:v1:v0 0x268:rs:v1:v0 0x268:rt:v1:v0 0x274:rt:v1:v0 */
#include "types.h"

extern s32 get_CD_State(void);
extern s32 SD_CDLevelGet(void);
extern void class_clear_n(s32, s32);

extern volatile s32 frame_counter;
extern volatile s32 name_ent_kuchi_now;
extern volatile s32 name_ent_kuchi_level_buf[4];
extern volatile s32 D_8014D194_a;

extern volatile s32 name_ent_kuchi_level_buf_1;
extern volatile s32 name_ent_kuchi_level_buf_2;
extern volatile s32 name_ent_kuchi_level_buf_3;
extern volatile u8 cw97_prio;
extern volatile u8 cw97_mode;
extern volatile u8 cw97_disp;
extern volatile u8 cw97_sel;
extern volatile s32 cw97_img;
extern volatile s32 cw97_anim_tbl;
extern volatile s16 cw97_frame;
extern volatile s16 cw97_x;
extern volatile s16 cw97_y;
extern volatile s32 cw97_hdr;
extern volatile s32 cw97_attr;
extern volatile u8 cw98_prio;
extern volatile u8 cw98_anim_flag;
extern volatile u8 cw98_disp;
extern volatile u8 cw98_tpage;
extern s16 cw98_tick;
extern volatile s32 cw98_cels;
extern volatile s32 cw98_anim_tbl;
extern volatile s16 cw98_frame;
extern volatile s16 cw98_x;
extern volatile s16 cw98_y;
extern volatile s32 cw98_hdr;
extern volatile s32 cw98_attr;
extern volatile u8 cw99_prio;
extern volatile u8 cw99_anim_flag;
extern volatile u8 cw99_disp;
extern volatile u8 cw99_tpage;
extern volatile s32 cw99_cels;
extern volatile s32 cw99_anim_tbl;
extern volatile s16 cw99_x;
extern volatile s16 cw99_y;
extern volatile s32 cw99_hdr;
extern volatile s32 cw99_attr;
extern volatile u8 cw102_prio;
extern volatile u8 cw102_anim_flag;
extern volatile u8 cw102_disp;
extern volatile u8 cw102_tpage;
extern volatile s32 cw102_cels;
extern volatile s32 cw102_anim_tbl;
extern volatile s16 cw102_anim;
extern volatile s16 cw102_x;
extern volatile s16 cw102_y;
extern volatile s32 cw102_hdr;
extern volatile s32 cw102_attr;

void name_ent_kuchi_ani6(void)
{
    s32 score;
    s32 sum;
    s32 rank;

    if (get_CD_State() & 4) {
        score = SD_CDLevelGet();
    } else {
        score = 0;
    }
    name_ent_kuchi_level_buf[frame_counter & 3] = score;
    sum = name_ent_kuchi_level_buf_3 + D_8014D194_a + name_ent_kuchi_level_buf_1 + name_ent_kuchi_level_buf_2;
    rank = (sum / 4) / 10;
    name_ent_kuchi_now = score;
    if (rank >= 4) {
        rank = 3;
    }
    class_clear_n(97, rank);
    if (get_CD_State() & 4) {
        cw97_prio = 9;
        cw97_mode = 64;
        cw97_attr = 0x41000000;
        cw97_disp = 164;
        cw97_img = (s32)0x801842C4;
        cw97_anim_tbl = (s32)0x801842F0;
        cw97_hdr = (s32)0x801842BC;
        cw97_frame = 6 - rank;
        cw97_sel = 128;
        cw97_x = -160;
        cw97_y = -120;
    }
}

void name_ent_cw102_set(void)
{
    cw102_prio = 9;
    cw102_anim_flag = 3;
    cw102_attr = 0x41000000;
    cw102_disp = 164;
    cw102_cels = (s32)0x801842C4;
    cw102_anim_tbl = (s32)0x801842F0;
    cw102_hdr = (s32)0x801842BC;
    cw102_anim = 0;
    cw102_tpage = 128;
    cw102_x = -160;
    cw102_y = -120;
}

void name_ent_cw98_anim(void)
{
    s32 cnt;

    cnt = cw98_tick;
    cw98_prio = 9;
    cw98_anim_flag = 64;
    cw98_attr = 0x41000000;
    cw98_disp = 164;
    cw98_cels = (s32)0x801842C4;
    cw98_anim_tbl = (s32)0x801842F0;
    cw98_hdr = (s32)0x801842BC;
    cnt = (s16)(cnt + 1);
    if (cnt >= 5) {
        cw98_tick = cnt;
        cw98_frame = 10;
    } else {
        cw98_tick = cnt;
        if (cnt >= 5) {
            cw98_frame = 9;
        } else {
            cw98_frame = 8;
        }
    }
    cw98_tpage = 128;
    cw98_x = -160;
    cw98_y = -120;
}

void name_ent_cw99_set0(void)
{
    cw99_prio = 9;
    cw99_anim_flag = 0;
    cw99_attr = 0x41000000;
    cw99_disp = 164;
    cw99_cels = (s32)0x801800F8;
    cw99_anim_tbl = (s32)0x80180114;
    cw99_hdr = (s32)0x801800F4;
    cw99_tpage = 128;
    cw99_x = -160;
    cw99_y = -120;
}

void name_ent_cw99_set5(void)
{
    cw99_prio = 9;
    cw99_anim_flag = 5;
    cw99_attr = 0x41000000;
    cw99_disp = 164;
    cw99_cels = (s32)0x801800F8;
    cw99_anim_tbl = (s32)0x80180114;
    cw99_hdr = (s32)0x801800F4;
    cw99_tpage = 128;
    cw99_x = -160;
    cw99_y = -120;
}

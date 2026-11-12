/* @CompileFlags: -Wc,-pic1 */
/* name_ent_kuchi_ania - NAME_ENT overlay, Address: 0x80148634, Size: 484 bytes (121 insns) */
/* @SymbolSize: 484 */
/* @PostProc: patch_stack_complete --old 56 --new 64 */
/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: swap_instructions --offset1 0xe0 --offset2 0xfc */
/* @PostProc: patch_insn_reg_field 0x28:rd:v1:a0 0x38:rd:v1:a0 0x54:rt:v1:a0 0x80:rd:a0:v1 0x84:rs:a0:v1 0x88:rd:v0:t6 0x88:rt:a0:v1 0x8c:rs:a0:v1 0x90:rd:v0:t6 0x98:rs:v0:t6 0xa0:rt:v1:a0 */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 get_CD_State(void);
extern s32 SD_CDLevelGet(void);
extern void class_clear_n(s32 a0);

extern s32 frame_counter;
extern s32 name_ent_kuchi_level_buf[];
extern s32 name_ent_kuchi_now;

extern u8  cw97_prio;
extern u8  cw97_mode;
extern u8  cw97_disp;
extern u8  cw97_sel;
extern s16 cw97_frame;
extern s16 cw97_x;
extern s16 cw97_y;
extern u32 cw97_img;
extern u32 cw97_anim_tbl;
extern u32 cw97_hdr;
extern u32 cw97_attr;

extern s16 cw98_frame;
extern s16 cw98_x;
extern s16 cw98_y;
extern CLASS_W class_w[];

void name_ent_kuchi_ania(void)
{
    s32 sel;
    s32 sum;
    s32 rating;
    u32 colA;
    u32 ptrA;
    u32 ptrB;
    u32 ptrC;

    if (get_CD_State() & 4) {
        sel = SD_CDLevelGet();
    } else {
        sel = 0;
    }

    name_ent_kuchi_level_buf[frame_counter & 3] = sel;
    sum = name_ent_kuchi_level_buf[0] + name_ent_kuchi_level_buf[1] + name_ent_kuchi_level_buf[2] + name_ent_kuchi_level_buf[3];
    rating = (sum / 4) / 10;
    name_ent_kuchi_now = sel;
    if (rating >= 4) {
        rating = 3;
    }

    class_clear_n(97);
    class_clear_n(98);

    if (get_CD_State() & 4) {
        colA = 0x41000000;
        ptrA = 0x80180150;
        ptrB = 0x80180174;
        ptrC = 0x80180140;

        cw97_prio = 9;
        cw97_mode = 64;
        cw97_attr = colA;
        cw97_disp = 164;
        cw97_img = ptrA;
        cw97_anim_tbl = ptrB;
        cw97_hdr = ptrC;
        cw97_frame = rating + 1;
        cw97_sel = 128;
        cw97_x = -160;
        cw97_y = -120;

        class_w[98].prio = 9;
        class_w[98].anim_flag = 64;
        class_w[98].attribute = colA;
        class_w[98].disp_flag = 164;
        (*(u32 *)&class_w[98].cels) = ptrA;
        (*(u32 *)&class_w[98].anim_tbl) = ptrB;
        (*(u32 *)&class_w[98].hdr) = ptrC;
        cw98_frame = rating + 5;
        class_w[98].tpage_sel = 128;
        cw98_x = -160;
        cw98_y = -120;
    }
}

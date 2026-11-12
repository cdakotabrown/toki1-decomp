/* Address: 0x800766E0 (true start 0x800766C0: the first 255 stores are */
/* Size: 368 bytes (92 instructions) over the corrected window */
/* Region: Game (IDO 5.2) */
/* @SymbolStart: 0x800766C0 */
/* @SymbolSize: 368 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x48:rt:a1:a0 0xd0:rs:a1:a0 0xd0:rt:a1:a0 0xd8:rs:a1:a0 0xfc:rs:a1:a0 0x11c:rs:a1:a0 0x148:rs:a1:a0 */
#include "types.h"

typedef struct {
    u32 id : 5;
    u32 pad : 20;
    u32 grp : 4;
    u32 on : 1;
    u32 rest : 2;
} Ent;

typedef struct {
    u8 a;
    u8 b;
    u8 c;
    u8 d;
} Rec;

typedef struct {
    u8 pad0[1356];
    Ent ent[8];
    u8 pad1388[304];
    Rec rec[32];
    u8 idx;
    u8 pad1821[7];
    u8 col[4];
} Save;

extern Save screen_w;
extern u8 now_girl;
extern u8 girl_no;
extern u8 machiawase_flag;
extern u8 p_sei;
extern u8 machiawase_init_str0;

void machiawase_init()
{
    s32 list[33];
    Save *sv;

    screen_w.col[0] = 255;
    screen_w.col[1] = 255;
    screen_w.col[2] = 255;
    screen_w.col[3] = 255;
    if (collect_distinct_active_entry_ids(list) >= 2) {
        k_reset(1);
        k_sub_reset_point_set();
        gnsx(&p_sei);
        sndisp(&machiawase_init_str0, 0, 31);
        k_sub_disp_start(1);
        sub_sub_step_up();
        return;
    }
    sv = &screen_w;
    screen_w.col[0] = screen_w.ent[0].id;
    screen_w.col[1] = screen_w.ent[0].id;
    screen_w.col[2] = 255;
    screen_w.col[3] = 255;
    sv->rec[screen_w.idx].a = 128;
    sv->rec[screen_w.idx].b = screen_w.ent[0].grp;
    sv->rec[screen_w.idx].c = screen_w.ent[0].id;
    now_girl = screen_w.ent[0].grp;
    girl_no = screen_w.ent[0].grp;
    machiawase_flag = ((u8 *)sv)[1359] & 1;
    goto_sub_sub_step(2);
}

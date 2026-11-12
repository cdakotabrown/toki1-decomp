/* @PostProc: patch_insn_reg_field 0x110:rs:t7:t4 0x110:rt:t4:t7 0x11c:rt:t5:t7 0x124:rd:t8:t5 0x124:rt:t5:t7 0x128:rt:t8:t5 0x12c:rt:t9:t8 0x134:rt:t9:t8 0x138:rt:t6:t9 0x13c:rt:t7:t6 0x140:rt:t7:t6 0x15c:rt:t6:t9 0x160:rt:t5:t7 0x164:rt:t9:t8 0x168:rs:t9:t8 0x168:rt:t9:t8 0x16c:rs:t5:t7 0x16c:rt:t5:t7 0x174:rt:t8:t5 0x180:rd:t6:t9 0x180:rt:t9:t8 0x184:rd:t8:t5 0x184:rs:t8:t5 0x184:rt:t5:t7 0x188:rs:t8:t5 0x188:rt:t8:t5 0x190:rd:t6:t9 0x190:rs:t6:t9 0x190:rt:t9:t8 0x198:rd:t6:t9 0x198:rt:t6:t9 0x1a4:rd:t7:t6 0x1a4:rs:t8:t5 0x1a4:rt:t6:t9 0x1a8:rd:t5:t7 0x1a8:rt:t7:t6 0x1b4:rs:t5:t7 0x1b4:rt:t9:t7 0x1b8:rt:t6:t5 0x1bc:rt:t9:t7 0x1c0:rt:t6:t5 0x1c4:rt:t7:t9 0x1cc:rt:t7:t9 0x1d0:rt:t5:t6 0x1dc:rt:t9:t7 0x1e0:rt:t9:t7 0x1f0:rt:t5:t6 */
/* Address: 0x801403E4 */
/* Size: 508 bytes (127 instructions) */
#include "types.h"

typedef struct { s32 w[12]; } Blk48;
typedef struct { s32 w[9]; } Blk36;

extern void shougatu_tri_disp(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void dtd_on(s32);

extern s16 cw97_scaley;
extern s16 cw97_x;
extern s16 cw97_y;
extern u8 now_year;
extern u8 shougatu_menu_slot;
extern u8 shougatu_menu_girl_tbl[];
extern Blk48 shougatu_pair_disp_tbl0;
extern Blk36 shougatu_pair_disp_tbl1;

void shougatu_pair_disp(void)
{
    s32 pad;
    s16 ofs;
    s16 adj;
    Blk48 sp112;
    Blk36 sp76;

    sp112 = shougatu_pair_disp_tbl0;
    sp76 = shougatu_pair_disp_tbl1;

    ofs = 28 - (cw97_scaley / 512) * 4;
    adj = 1;
    if (cw97_scaley == 4096) {
        ofs = 0;
        adj = 0;
    }

    shougatu_tri_disp(cw97_x - adj - 28, cw97_y + ofs - 28,
                  cw97_x - 1, cw97_y + ofs - 28,
                  cw97_x - adj - 28, cw97_y - 1,
                  sp112.w[((u8 *)&sp76 + shougatu_menu_girl_tbl[shougatu_menu_slot])[
                          now_year * 12 - 1140] * 2],
                  sp112.w[((u8 *)&sp76 + shougatu_menu_girl_tbl[shougatu_menu_slot])[
                          now_year * 12 - 1140] * 2 + 1], 9, 0);

    shougatu_tri_disp(cw97_x + adj + 28, cw97_y - ofs + 28,
                  cw97_x + adj + 28, cw97_y,
                  cw97_x, cw97_y - ofs + 28,
                  sp112.w[((u8 *)&sp76 + shougatu_menu_girl_tbl[shougatu_menu_slot])[
                          now_year * 12 - 1140] * 2],
                  sp112.w[((u8 *)&sp76 + shougatu_menu_girl_tbl[shougatu_menu_slot])[
                          now_year * 12 - 1140] * 2 + 1], 9, 0);

    dtd_on(11);
}

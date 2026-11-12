/* @RodataBase: 0x80144824 */
/* @PostProc: patch_stack_complete --old 104 --new 112 */
/* @PostProc: swap_instructions --offset1 0x10c --offset2 0x114 */
/* @PostProc: remap_stack_slots --map '36:40,40:44,44:48,48:52,52:56,60:64,72:76,80:84' */
/* geko_bustup_cd_read -- GEKO overlay, Address: 0x80136BCC, Size: 604 bytes (151 insns) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 bustup_cd_read(s32 a0);
extern s32 girl_season_load(void);
extern s32 girl_message_load(void);

extern u8  now_month;
extern u16 basyo_pack;
extern u8  now_girl;
extern u8  geko_girl_flag;
extern u8  geko_reserved_flag;

typedef struct {
    s32 v[11];
} TabA;

typedef struct {
    s32 v[2];
} TabB;

extern TabA geko_bustup_cd_read_tbl0;
extern TabB geko_bustup_cd_read_tbl1;
extern TabB geko_bustup_cd_read_tbl2;
extern TabB geko_bustup_cd_read_tbl3;

void geko_bustup_cd_read(void)
{
    TabA a;
    TabB b;
    TabB c;
    TabB d;
    u32 mode;
    u32 sel;

    mode = now_month;

    a = geko_bustup_cd_read_tbl0;
    b = geko_bustup_cd_read_tbl1;
    c = geko_bustup_cd_read_tbl2;
    d = geko_bustup_cd_read_tbl3;

    if (mode < 6U || mode >= 10U) {
        a.v[0] = 21578;
        a.v[3] = 21578;
        a.v[5] = 21578;
        b.v[0] = 23873;
        c.v[1] = 21038;
        d.v[1] = 22703;
    }

    if (geko_girl_flag != 0) {
        goto flags;
    }

    if (geko_reserved_flag == 0) {
        girl_message_load();
        goto end;
    }

    sel = now_girl;
    switch ((u32)sel) {
    case 0:
        bustup_cd_read(a.v[(u32)basyo_pack >> 12]);
        break;
    case 1:
        bustup_cd_read(b.v[(u32)basyo_pack >> 12]);
        break;
    case 2:
        bustup_cd_read(c.v[((u32)basyo_pack >> 12) - 2]);
        break;
    case 3:
        bustup_cd_read(d.v[((u32)basyo_pack >> 12) - 4]);
        break;
    case 4:
        bustup_cd_read(26348);
        break;
    case 5:
        bustup_cd_read(24548);
        break;
    case 6:
        bustup_cd_read(25178);
        break;
    default:
        bustup_cd_read(26978);
        break;
    }

    sub_sub_step_up();
    goto end;

flags:
    girl_season_load();

end:
    ;
}

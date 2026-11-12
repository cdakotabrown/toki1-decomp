/* @RodataBase: 0x801396D8 */
/* @PostProc: patch_stack_complete --old 96 --new 112 */
/* @PostProc: remap_stack_slots --map '28:40,32:44,36:48,40:52,44:56,52:64,64:76,72:84' */
/* Address: 0x80132C2C */
/* Size: 540 bytes (135 insns) */
#include "types.h"

extern void bustup_cd_read(s32);
extern void sub_sub_step_up(void);

typedef struct {
    s32 w[11];
} Blk44;

typedef struct {
    s32 w[2];
} Blk8;

extern Blk44 kangei_bustup_read_tbl0;
extern Blk8 kangei_bustup_read_tbl1;
extern Blk8 kangei_bustup_read_tbl2;
extern Blk8 kangei_bustup_read_tbl3;
extern u8 now_month;
extern u16 basyo_pack;
extern u8 now_girl;

void kangei_bustup_read(void)
{
    Blk44 tbl0;
    Blk8 tbl1;
    Blk8 tbl2;
    Blk8 tbl3;
    u32 mode;

    mode = now_month;

    tbl0 = kangei_bustup_read_tbl0;
    tbl1 = kangei_bustup_read_tbl1;
    tbl2 = kangei_bustup_read_tbl2;
    tbl3 = kangei_bustup_read_tbl3;

    if (mode < 6 || mode >= 10) {
        tbl0.w[0] = 21578;
        tbl0.w[3] = 21578;
        tbl0.w[5] = 21578;
        tbl1.w[0] = 23873;
        tbl2.w[1] = 21038;
        tbl3.w[1] = 22703;
    }

    switch (now_girl) {
    case 0:
        bustup_cd_read(tbl0.w[(u32)basyo_pack >> 12]);
        break;
    case 1:
        bustup_cd_read(tbl1.w[(u32)basyo_pack >> 12]);
        break;
    case 2:
        bustup_cd_read(tbl2.w[((u32)basyo_pack >> 12) - 2]);
        break;
    case 3:
        bustup_cd_read(tbl3.w[((u32)basyo_pack >> 12) - 4]);
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
}

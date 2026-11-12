/* kangei_switch_bustup_read - KANGEI overlay, Address: 0x8013455C, Size: 472 bytes */
/* @RodataBase: 0x80139824 */
/* @PostProc: patch_stack_complete --old 96 --new 112 */
/* @PostProc: remap_stack_slots --map '28:40,36:48,44:56,52:64' */
#include "types.h"

extern void bustup_cd_read(s32);
extern void sub_sub_step_up(void);

typedef struct {
    s32 w[11];
} Blk44;

typedef struct {
    s32 w[2];
} Blk8;

extern Blk44 kangei_bustup_read2_tbl0;
extern Blk8 kangei_bustup_read2_tbl1;
extern Blk8 kangei_bustup_read2_tbl2;
extern Blk8 kangei_bustup_read2_tbl3;

extern u8 now_girl;
extern volatile u16 basyo_pack;

void kangei_switch_bustup_read(void)
{
    Blk44 blk0;
    Blk8 blk1;
    Blk8 blk2;
    Blk8 blk3;

    blk0 = kangei_bustup_read2_tbl0;
    blk1 = kangei_bustup_read2_tbl1;
    blk2 = kangei_bustup_read2_tbl2;
    blk3 = kangei_bustup_read2_tbl3;

    switch (now_girl) {
    case 0:
        bustup_cd_read(blk0.w[(u32)basyo_pack >> 12]);
        break;
    case 1:
        bustup_cd_read(blk1.w[(u32)basyo_pack >> 12]);
        break;
    case 2:
        bustup_cd_read(blk2.w[((u32)basyo_pack >> 12) - 2]);
        break;
    case 3:
        bustup_cd_read(blk3.w[((u32)basyo_pack >> 12) - 4]);
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

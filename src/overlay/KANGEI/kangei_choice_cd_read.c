/* kangei_choice_cd_read - KANGEI overlay, Address: 0x801362F0, Size: 420 bytes */
/* @PostProc: patch_insn_reg_field 0x38:rt:v1:v0 0x40:rs:v1:v0 0x64:rs:v1:v0 0xf0:rt:v0:t3 0xf4:rs:v0:t3 0xf4:rt:v0:t3 0xfc:rs:v0:t3 0x108:rs:v0:t3 0x114:rs:v0:t3 0x120:rs:v0:t3 */
#include "types.h"

extern void srand(s32);
extern s32 rand(void);
extern void sub_sub_step_up(void);
extern void cd_read_start2(s32, void *, s32);

extern s32 frame_counter;
extern u8 now_girl;
extern u16 basyo_pack;
extern u8 kangei_choice_no;
extern u8 kangei_choice_tbl[];
extern u8 kangei_choice_bg_no;

void kangei_choice_cd_read(void)
{
    s32 rnd;
    s32 rem;
    u8 entry;
    u32 flags;

    srand(frame_counter);

    entry = kangei_choice_tbl[kangei_choice_no];
    if (entry == 1) {
        sub_sub_step_up();
        sub_sub_step_up();
    } else if (entry == 2) {
        sub_sub_step_up();
        sub_sub_step_up();
    } else {
        now_girl = 11;
        rnd = rand();
        flags = basyo_pack;
        rem = rnd % 4;
        kangei_choice_bg_no = rem;
        if ((flags >> 12) == 10) {
            rnd = rand();
            rem = rnd % 2;
            if (rem) {
                kangei_choice_bg_no = 4;
            }
        }
        switch (kangei_choice_bg_no) {
        case 0:
            cd_read_start2(53, (void *)0x801B4400, 0x8FF3);
            break;
        case 1:
            cd_read_start2(53, (void *)0x801B4400, 0x9028);
            break;
        case 2:
            cd_read_start2(53, (void *)0x801B4400, 0x905D);
            break;
        case 3:
            cd_read_start2(53, (void *)0x801B4400, 0x9092);
            break;
        default:
            cd_read_start2(53, (void *)0x801B4400, 0x90C7);
            break;
        }
    }
    sub_sub_step_up();
}

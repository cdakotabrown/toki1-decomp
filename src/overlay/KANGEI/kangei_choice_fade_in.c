/* @SymbolSize: 164 */
/* kangei_choice_fade_in - KANGEI overlay, Address: 0x80136528, Size: 156 bytes (39 insns) */
/* @PostProc: patch_insn_reg_field 0x3c:rt:v0:t7 0x40:rd:v0:t7 0x40:rs:v0:t7 0x44:rs:v0:t7 0x44:rt:v0:t7 0x4c:rs:v0:t7 0x6c:rt:t7:t8 0x70:rs:t7:t8 0x70:rt:t7:t8 0x74:rt:v0:t9 0x78:rd:v0:t9 0x78:rs:v0:t9 0x78:rt:t7:t8 0x7c:rs:v0:t9 0x7c:rt:v0:t9 0x84:rs:v0:t9 */
#include "types.h"

extern void device_cmd_send(s32);
extern void fade_in_step(void);
extern void date_fade_in(void);

extern s32 step_counter;

extern u8 kangei_choice_no;

extern u8 kangei_choice_tbl[4];

void
kangei_choice_fade_in(void)
{
    u8 idx;
    u8 entryVal;
    s32 counter;
    s32 lt;

    counter = step_counter;
    lt = (s32)((u32)counter < 1U);
    step_counter = counter + 1;

    if (lt != 0) {

        idx = kangei_choice_no;
        entryVal = kangei_choice_tbl[idx];
        if (entryVal == 0) {
            device_cmd_send(516);
        } else {
            device_cmd_send(517);
        }
    }

    idx = kangei_choice_no;
    entryVal = kangei_choice_tbl[idx];
    if (entryVal == 1) {
        fade_in_step();
    } else {
        date_fade_in();
    }
}

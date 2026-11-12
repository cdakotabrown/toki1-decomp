/* Function: bustup_load_step */
/* Address: 0x80086828 (cross-prologue head absorbed; recorded symbol was 0x80086830) */
/* Size: 300 bytes (75 instructions) */
/* Region: Library address range (0x80086000+) but IDO 5.2 game-style codegen */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @SymbolStart: 0x80086828 */
/* @SymbolSize: 300 */
#include "types.h"

extern u8 step_mode;
extern s32 *bustup_check_init_tbl0;
extern s32 *bustup_check_init_tbl1;
extern s32 *bustup_check_init_tbl2;

extern void bustup_cd_read(void);
extern s32 get_CD_State2(void);
extern s32 block_checksum_check(s32, u32);
extern void sub_sub_step_up(void);

void bustup_load_step(void)
{
    if (step_mode == 0) {
        bustup_cd_read();
        step_mode++;
        return;
    }

    if ((get_CD_State2() & 1) != 0) {
        if (bustup_check_init_tbl0[0] != 0x801820AC || bustup_check_init_tbl0[1] != 0x801860AC ||
            bustup_check_init_tbl0[2] != 0x8018A0AC || bustup_check_init_tbl0[3] != 0x8018E0AC ||
            bustup_check_init_tbl0[4] != 0x801920AC || bustup_check_init_tbl2[0] != 0x8018005C ||
            bustup_check_init_tbl2[1] != 0x80180070 || bustup_check_init_tbl1[0] != 0x80180000) {
            step_mode = 0;
            return;
        }
        if (block_checksum_check(45, 0x80180000) == 0) {
            step_mode = 0;
            return;
        }
        sub_sub_step_up();
    }
}

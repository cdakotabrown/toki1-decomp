/* @SymbolStart: 0x80138078 */
/* @SymbolSize: 180 */
/* @PostProc: patch_insn_reg_field 0x18:rt:a0:v0 0x1c:rs:a0:v0 */
#include "types.h"

extern void kangei_table_pick(void);
extern void kangei_player_birthday_dispatch(void);
extern void sub_step_up(void);

extern s32 girl0_birth;
extern s32 p_birthday;
extern u8 now_day;
extern u8 now_month;
extern u8 kangei_birth_done;
extern u8 kangei_icon_flag;

void kangei_slot_match(void)
{
    s32 slot;
    u8 want;

    slot = girl0_birth;
    want = now_day;

    if ((u32)(slot << 23) >> 27 == want) {
        if ((slot & 0xF) == now_month) {
            if (kangei_birth_done == 0) {
                kangei_table_pick();
                return;
            }
        }
    }

    slot = p_birthday;
    if ((u32)(slot << 23) >> 27 == want) {
        if ((slot & 0xF) == now_month) {
            kangei_player_birthday_dispatch();
            return;
        }
    }

    sub_step_up();

    kangei_icon_flag = 0;
}

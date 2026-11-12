/* @SymbolStart: 0x8013CAC0 */
/* @RodataBase: 0x8014C154 */
/* @SymbolSize: 188 */
/* /* name_ent_sub1 - NAME_ENT overlay, Address: 0x8013CAC8, Size: 188 bytes (47 insns) */
#include "types.h"

extern void name_ent_msg0_init(void);
extern void name_ent_card_result(void);
extern void name_ent_msg2_init(void);
extern void name_ent_msg3_wait(void);
extern void name_ent_msg4_init(void);
extern void name_ent_msg5_select(void);
extern void name_ent_card_insert_disp(void);
extern void name_ent_save_flag_clear(void);
extern u8 now_sub_sub_step;

void name_ent_sub1(void)
{

    switch (now_sub_sub_step) {
    case 0:
        name_ent_msg0_init();
        break;
    case 1:
        name_ent_card_result();
        break;
    case 2:
        name_ent_msg2_init();
        break;
    case 3:
        name_ent_msg3_wait();
        break;
    case 4:
        name_ent_msg4_init();
        break;
    case 5:
        name_ent_msg5_select();
        break;
    case 6:
        name_ent_card_insert_disp();
        break;
    case 7:
        name_ent_save_flag_clear();
        break;
    default:
        return;
    }
}

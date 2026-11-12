/* @RodataBase: 0x8014C4D4 */
/* name_ent_main - NAME_ENT overlay, Address: 0x80142300, Size: 580 bytes (145 insns) */
/* @SymbolSize: 580 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xb8 */
/* @PostProc: patch_insn_reg_field 0x8c:rt:t4:t6 0x90:rt:t6:t8 0x94:rt:t8:t0 0x98:rt:t0:t4 0xa0:rs:t4:t6 0xa0:rt:t5:t7 0xa4:rs:t6:t8 0xa4:rt:t7:t9 0xa8:rs:t8:t0 0xa8:rt:t9:t1 0xac:rs:t0:t4 0xac:rt:t1:t5 0xb0:rt:t9:t1 0xb4:rt:t7:t9 0xb8:rt:t5:t7 0xc0:rt:t1:t5 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void name_hilite_set(s32 a0);
extern void name_ent_sub0(void);
extern void name_ent_sub1(void);
extern void name_ent_card_menu_main(void);
extern void name_ent_xa_exit_main(void);
extern void name_ent_file_error_main(void);
extern void name_ent_mainc0(void);
extern void name_ent_maind0(void);
extern void name_ent_mainf0(void);
extern void name_ent_mainff(void);
extern void name_ent_iris_show(void);
extern void name_key_blink(void);
extern void k_disp_inc(void);
extern void step_directional_scroll_once(void);
extern void name_hdr_set(void);
extern void name_ent_keytile_disp(void);
extern void name_ent_white_fade(void);

extern u8 class_w[];
extern volatile u8 cw141_disp;
extern volatile u8 cw142_disp;
extern volatile u8 cw143_disp;

extern volatile s32 step_state;
extern volatile u8 now_sub_step;
extern u8 menu_mode;
extern s32 name_ent_menu_lock;

void name_ent_main(void)
{
    u8 *p;
    u8 *end;

    end = (&((CLASS_W *)class_w)[13].active);
    p = class_w;
    do {
        p[8707] &= 0x7F;
        p += 68;
    } while (p < end);

    cw141_disp &= 0x7F;
    cw142_disp &= 0x7F;
    cw143_disp &= 0x7F;

    end = (&((CLASS_W *)class_w)[11].active);
    p = (&((CLASS_W *)class_w)[3].active);
    do {
        p[9659] &= 0x7F;
        p[9727] &= 0x7F;
        p[9795] &= 0x7F;
        p[9591] &= 0x7F;
        p += 272;
    } while (p != end);

    name_hilite_set(0);
    step_state = step_state + 1;

    switch ((u32)now_sub_step) {
    case 0:
        name_ent_sub0();
        break;
    case 1:
        name_ent_sub1();
        break;
    case 2:
        name_ent_card_menu_main();
        break;
    case 3:
        name_ent_xa_exit_main();
        break;
    case 4:
        name_ent_file_error_main();
        break;
    case 192:
        name_ent_mainc0();
        break;
    case 208:
        name_ent_maind0();
        break;
    case 240:
        name_ent_mainf0();
        break;
    case 255:
        name_ent_mainff();
        break;
    }

    name_ent_iris_show();
    name_key_blink();
    k_disp_inc();
    step_directional_scroll_once();
    name_hdr_set();
    name_ent_keytile_disp();
    name_ent_white_fade();

    if (name_ent_menu_lock != 0) {
        menu_mode = 0;
    }
}

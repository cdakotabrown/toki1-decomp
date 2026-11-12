/* @SymbolStart: 0x801401B4 */
/* @RodataBase: 0x8014C234 */
/* @SymbolSize: 1004 */
#include "types.h"

extern s32 step_counter;
extern u8 now_sub_sub_step;

extern void name_ent_card_search0(void);
extern void name_ent_sysfile_load(void);
extern void name_ent_main2_2(void);
extern void name_ent_save3(void);
extern void name_ent_album_first_load(void);
extern void name_ent_main2_21(void);
extern void name_ent_album_loading_disp(void);
extern void name_ent_album_chosen_load(void);
extern void name_ent_album_restore(void);
extern void name_ent_new_album_ask_disp(void);
extern void name_ent_new_album_ask_select(void);
extern void name_ent_no_album_notice_disp(void);
extern void name_ent_no_album_notice_select(void);
extern void name_ent_album_create_slot_disp(void);
extern void name_ent_album_create_slot_select(void);
extern void name_ent_sysfile_info_disp(void);
extern void name_ent_sysfile_info_select(void);
extern void name_ent_sysfile_ask_disp(void);
extern void name_ent_sysfile_ask_select(void);
extern void name_ent_album_make4a(void);
extern void name_ent_album_make4c(void);
extern void name_ent_dispa0(void);
extern void name_ent_selecta1(void);
extern void name_ent_dispa2(void);
extern void name_ent_selecta3(void);
extern void name_ent_dispa4(void);
extern void name_ent_selecta5(void);
extern void name_ent_loada6(void);
extern void name_ent_loada7(void);
extern void name_ent_album_makea8(void);
extern void name_ent_album_makea9(void);
extern void name_ent_dispc0(void);
extern void name_ent_selectc1(void);
extern void name_ent_loadc2(void);
extern void name_ent_selectc3(void);
extern void name_ent_dispc4(void);
extern void name_ent_selectc5(void);
extern void name_ent_main2_c6(void);
extern void name_ent_album_copyc7(void);
extern void name_ent_album_copyc8(void);
extern void name_ent_dispe0(void);
extern void name_ent_selecte1(void);
extern void name_ent_dispe2(void);
extern void name_ent_selecte3(void);
extern void name_ent_album_deletee4(void);
extern void name_ent_album_deletee5(void);
extern void name_ent_main2_e6(void);
extern void name_ent_option_restore(void);
extern void goto_step(s32 arg);

void name_ent_card_menu_main(void)
{
    s32 step;

    step_counter++;
    step = now_sub_sub_step;
    switch ((u32)step) {
    case 0:   name_ent_card_search0(); break;
    case 1:   name_ent_sysfile_load(); break;
    case 2:   name_ent_main2_2(); break;
    case 3:   name_ent_save3(); break;
    case 32:  name_ent_album_first_load(); break;
    case 33:  name_ent_main2_21(); break;
    case 34:  name_ent_album_loading_disp(); break;
    case 35:  name_ent_album_chosen_load(); break;
    case 36:  name_ent_album_restore(); break;
    case 64:  name_ent_new_album_ask_disp(); break;
    case 65:  name_ent_new_album_ask_select(); break;
    case 66:  name_ent_no_album_notice_disp(); break;
    case 67:  name_ent_no_album_notice_select(); break;
    case 68:  name_ent_album_create_slot_disp(); break;
    case 69:  name_ent_album_create_slot_select(); break;
    case 70:  name_ent_sysfile_info_disp(); break;
    case 71:  name_ent_sysfile_info_select(); break;
    case 72:  name_ent_sysfile_ask_disp(); break;
    case 73:  name_ent_sysfile_ask_select(); break;
    case 74:  name_ent_album_make4a(); break;
    case 76:  name_ent_album_make4c(); break;
    case 160: name_ent_dispa0(); break;
    case 161: name_ent_selecta1(); break;
    case 162: name_ent_dispa2(); break;
    case 163: name_ent_selecta3(); break;
    case 164: name_ent_dispa4(); break;
    case 165: name_ent_selecta5(); break;
    case 166: name_ent_loada6(); break;
    case 167: name_ent_loada7(); break;
    case 168: name_ent_album_makea8(); break;
    case 169: name_ent_album_makea9(); break;
    case 192: name_ent_dispc0(); break;
    case 193: name_ent_selectc1(); break;
    case 194: name_ent_loadc2(); break;
    case 195: name_ent_selectc3(); break;
    case 196: name_ent_dispc4(); break;
    case 197: name_ent_selectc5(); break;
    case 198: name_ent_main2_c6(); break;
    case 199: name_ent_album_copyc7(); break;
    case 200: name_ent_album_copyc8(); break;
    case 224: name_ent_dispe0(); break;
    case 225: name_ent_selecte1(); break;
    case 226: name_ent_dispe2(); break;
    case 227: name_ent_selecte3(); break;
    case 228: name_ent_album_deletee4(); break;
    case 229: name_ent_album_deletee5(); break;
    case 230: name_ent_main2_e6(); break;
    default:
        name_ent_option_restore();
        goto_step(16);
        break;
    }
}

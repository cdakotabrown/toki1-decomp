/* @SymbolStart: 0x80134F78 */
/* @SymbolSize: 508 */
/* @RodataBase: 0x8014B610 */
#include "types.h"

extern s32 pad_data;
extern void func_80132000(s32);
extern void name_ent_names_disp(void);

extern s32 pad_trig;
extern u8  name_ent_fuji_blood;
extern u8  name_ent_p_mon;
extern u8  name_ent_p_blood;
extern u8  name_ent_p_day;
extern u8  name_ent_fuji_mon;
extern u8  name_ent_fuji_day;

void name_ent_names_select(void)
{
    s32 sel;
    sel = pad_data;
    if (sel & 0x100) {
        switch (pad_trig - 1) {
        case 3:
            func_80132000(10);
            name_ent_names_disp();
            break;
        case 0:
            func_80132000(1);
            name_ent_names_disp();
            break;
        case 7:
            func_80132000(2);
            name_ent_names_disp();
            break;
        case 1:
            func_80132000(9);
            name_ent_names_disp();
            break;
        case 4:
            func_80132000(4);
            name_ent_names_disp();
            break;
        case 9:
            func_80132000(3);
            name_ent_names_disp();
            break;
        case 11:
            func_80132000(5);
            name_ent_names_disp();
            break;
        case 2:
            func_80132000(6);
            name_ent_names_disp();
            break;
        case 8:
            func_80132000(7);
            name_ent_names_disp();
            break;
        case 5:
            func_80132000(8);
            name_ent_names_disp();
            break;
        case 12:
            func_80132000(11);
            name_ent_names_disp();
            break;
        case 6:
            func_80132000(12);
            name_ent_names_disp();
            break;
        case 13:
            func_80132000(13);
            name_ent_names_disp();
            break;
        case 10:
            func_80132000(14);
            name_ent_names_disp();
            break;
        case 14:
            func_80132000(0);
            name_ent_p_mon = 11;
            name_ent_p_day = 9;
            name_ent_p_blood = 0;
            name_ent_fuji_mon = 1;
            name_ent_fuji_day = 13;
            name_ent_fuji_blood = 3;
            name_ent_names_disp();
            break;
        }
    }
}

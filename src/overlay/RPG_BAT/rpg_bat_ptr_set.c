/* @CompileFlags: -cckr */
/* @RodataBase: 0x8015CEA0 */
/* @SymbolSize: 3032 */
/* rpg_bat_ptr_set - RPG_BAT overlay, Address: 0x8014D380, 3032 bytes (758 */
#include "types.h"

extern s32 rpg_bat_mode;
extern s32 rpg_bat_girl;
extern s32 rpg_bat_bukatu_no;
extern s32 rpg_bat_bukatu_flag;
extern s32 rpg_bat_res_hdr[];
extern s32 rpg_bat_res_img[];
extern s32 rpg_bat_res_anim[];
extern s32 rpg_bat_res_pal[];

rpg_bat_ptr_set()
{
    switch (rpg_bat_mode) {
    case 0x10000:
        rpg_bat_res_hdr[2] = 0x8018C1F8;
        rpg_bat_res_hdr[3] = 0x801BF9A4;
        rpg_bat_res_hdr[1] = 0x801948E0;
        rpg_bat_res_hdr[0] = 0x801B5758;
        rpg_bat_res_img[2] = 0x8018C204;
        rpg_bat_res_img[3] = 0x80194598;
        rpg_bat_res_img[1] = 0x801948EC;
        rpg_bat_res_img[0] = 0x801B576C;
        rpg_bat_res_anim[2] = 0x8018C24C;
        rpg_bat_res_anim[3] = 0x8019460C;
        rpg_bat_res_anim[1] = 0x8019494C;
        rpg_bat_res_anim[0] = 0x801B5874;
        rpg_bat_res_pal[1] = 0x801B594C;
        rpg_bat_res_pal[3] = 0x801B794C;
        rpg_bat_res_pal[2] = 0x801B994C;
        rpg_bat_res_pal[4] = 0x801BB94C;
        rpg_bat_res_pal[0] = 0x801BD94C;
        break;
    case 0x20000:
        rpg_bat_res_hdr[2] = 0x801B781C;
        rpg_bat_res_hdr[3] = 0x801B7864;
        rpg_bat_res_hdr[1] = 0x8018C7E4;
        rpg_bat_res_hdr[0] = 0x801AD620;
        rpg_bat_res_img[2] = 0x80184160;
        rpg_bat_res_img[3] = 0x8018C4E0;
        rpg_bat_res_img[1] = 0x8018C7F0;
        rpg_bat_res_img[0] = 0x801AD634;
        rpg_bat_res_anim[2] = 0x80184190;
        rpg_bat_res_anim[3] = 0x8018C554;
        rpg_bat_res_anim[1] = 0x8018C844;
        rpg_bat_res_anim[0] = 0x801AD73C;
        rpg_bat_res_pal[1] = 0x801AD814;
        rpg_bat_res_pal[3] = 0x801AF814;
        rpg_bat_res_pal[2] = 0x801B1814;
        rpg_bat_res_pal[4] = 0x801B3814;
        rpg_bat_res_pal[0] = 0x801B5814;
        break;
    case 0x40000:
        rpg_bat_res_hdr[2] = 0x801B7864;
        rpg_bat_res_hdr[3] = 0x801B78AC;
        rpg_bat_res_hdr[1] = 0x8018C810;
        rpg_bat_res_hdr[0] = 0x801AD638;
        rpg_bat_res_img[2] = 0x801841FC;
        rpg_bat_res_img[3] = 0x8018C59C;
        rpg_bat_res_img[1] = 0x8018C81C;
        rpg_bat_res_img[0] = 0x801AD64C;
        rpg_bat_res_anim[2] = 0x8018423C;
        rpg_bat_res_anim[3] = 0x8018C610;
        rpg_bat_res_anim[1] = 0x8018C85C;
        rpg_bat_res_anim[0] = 0x801AD754;
        rpg_bat_res_pal[1] = 0x801AD82C;
        rpg_bat_res_pal[3] = 0x801AF82C;
        rpg_bat_res_pal[2] = 0x801B182C;
        rpg_bat_res_pal[4] = 0x801B382C;
        rpg_bat_res_pal[0] = 0x801B582C;
        break;
    case 0x80000000:
        rpg_bat_res_hdr[3] = 0x801CC360;
        rpg_bat_res_hdr[1] = 0x801A1234;
        rpg_bat_res_hdr[0] = 0x801C20A4;
        rpg_bat_res_img[3] = 0x801A0C08;
        rpg_bat_res_img[1] = 0x801A1240;
        rpg_bat_res_img[0] = 0x801C20B8;
        rpg_bat_res_anim[3] = 0x801A0C7C;
        rpg_bat_res_anim[1] = 0x801A12B4;
        rpg_bat_res_anim[0] = 0x801C21C0;
        rpg_bat_res_pal[1] = 0x801C2298;
        rpg_bat_res_pal[4] = 0x801C8298;
        rpg_bat_res_pal[0] = 0x801CA298;
        rpg_bat_res_pal[3] = 0x801C6298;
        rpg_bat_res_hdr[5] = 0x80180790;
        rpg_bat_res_img[5] = 0x801807A8;
        rpg_bat_res_anim[5] = 0x80180864;
        break;
    case 0x8000000:
        rpg_bat_res_hdr[3] = 0x801CA1CC;
        rpg_bat_res_hdr[1] = 0x80189030;
        rpg_bat_res_hdr[0] = 0x801BFFA0;
        rpg_bat_res_img[3] = 0x8018833C;
        rpg_bat_res_img[1] = 0x8018904C;
        rpg_bat_res_img[0] = 0x801BFFB4;
        rpg_bat_res_anim[3] = 0x801883B0;
        rpg_bat_res_anim[1] = 0x80189174;
        rpg_bat_res_anim[0] = 0x801C00BC;
        rpg_bat_res_pal[1] = 0x801C0194;
        rpg_bat_res_pal[3] = 0x801C2194;
        rpg_bat_res_pal[2] = 0x801C4194;
        rpg_bat_res_pal[4] = 0x801C6194;
        rpg_bat_res_pal[0] = 0x801C8194;
        rpg_bat_res_pal[6] = 0x801A91D8;
        break;
    case 0x100000:
        rpg_bat_res_hdr[3] = 0x801B0BC8;
        rpg_bat_res_hdr[1] = 0x80188AC0;
        rpg_bat_res_img[3] = 0x8018833C;
        rpg_bat_res_img[1] = 0x80188ADC;
        rpg_bat_res_anim[3] = 0x801883B0;
        rpg_bat_res_anim[1] = 0x80188B50;
        rpg_bat_res_hdr[0] = 0x801C5DC8;
        rpg_bat_res_img[0] = 0x801C5DDC;
        rpg_bat_res_anim[0] = 0x801C5EE4;
        rpg_bat_res_pal[1] = 0x801A6B90;
        rpg_bat_res_pal[3] = 0x801A8B90;
        rpg_bat_res_pal[2] = 0x801AAB90;
        rpg_bat_res_pal[4] = 0x801ACB90;
        rpg_bat_res_pal[0] = 0x801AEB90;
        rpg_bat_res_pal[6] = 0x801A4B90;
        break;
    }
    if (rpg_bat_bukatu_flag == 1) {
        switch (rpg_bat_bukatu_no) {
        case 0:
            rpg_bat_res_hdr[4] = 0x801E86C0;
            rpg_bat_res_img[4] = 0x801E86D0;
            rpg_bat_res_anim[4] = 0x801E8778;
            break;
        case 1:
            rpg_bat_res_hdr[4] = 0x801D82A0;
            rpg_bat_res_img[4] = 0x801D82B0;
            rpg_bat_res_anim[4] = 0x801D82F4;
            break;
        case 2:
            rpg_bat_res_hdr[4] = 0x801D8868;
            rpg_bat_res_img[4] = 0x801D8878;
            rpg_bat_res_anim[4] = 0x801D892C;
            break;
        case 3:
            rpg_bat_res_hdr[4] = 0x801EC528;
            rpg_bat_res_img[4] = 0x801EC53C;
            rpg_bat_res_anim[4] = 0x801EC59C;
            rpg_bat_res_pal[5] = 0x801EC5CC;
            break;
        case 4:
            rpg_bat_res_hdr[4] = 0x801D8268;
            rpg_bat_res_img[4] = 0x801D827C;
            rpg_bat_res_anim[4] = 0x801D82BC;
            rpg_bat_res_pal[5] = 0x801EC2E0;
            break;
        case 5:
            rpg_bat_res_hdr[4] = 0x801D8814;
            rpg_bat_res_img[4] = 0x801D8824;
            rpg_bat_res_anim[4] = 0x801D88A8;
            break;
        case 6:
            rpg_bat_res_hdr[4] = 0x801E4308;
            rpg_bat_res_img[4] = 0x801E4314;
            rpg_bat_res_anim[4] = 0x801E4364;
            break;
        case 7:
            rpg_bat_res_hdr[4] = 0x801E49B0;
            rpg_bat_res_img[4] = 0x801E49BC;
            rpg_bat_res_anim[4] = 0x801E4A6C;
            break;
        case 8:
            rpg_bat_res_hdr[4] = 0x801EC58C;
            rpg_bat_res_img[4] = 0x801EC5A0;
            rpg_bat_res_anim[4] = 0x801EC630;
            break;
        case 9:
            rpg_bat_res_hdr[4] = 0x801ED374;
            rpg_bat_res_img[4] = 0x801ED388;
            rpg_bat_res_anim[4] = 0x801ED580;
            break;
        case 10:
            rpg_bat_res_hdr[4] = 0x801E43C8;
            rpg_bat_res_img[4] = 0x801E43D4;
            rpg_bat_res_anim[4] = 0x801E4440;
            break;
        case 11:
        default:
            rpg_bat_res_hdr[4] = 0x801D9370;
            rpg_bat_res_img[4] = 0x801D9388;
            rpg_bat_res_anim[4] = 0x801D9550;
            break;
        }
    }
    switch (rpg_bat_girl) {
    case 0:
        rpg_bat_res_hdr[5] = 0x801CB164;
        rpg_bat_res_img[5] = 0x801C7114;
        rpg_bat_res_anim[5] = 0x801C7138;
        break;
    case 1:
        rpg_bat_res_hdr[5] = 0x801CF2CC;
        rpg_bat_res_img[5] = 0x801CF250;
        rpg_bat_res_anim[5] = 0x801CF290;
        break;
    case 2:
        if (rpg_bat_mode == 0x80000000) {
            break;
        }
        rpg_bat_res_hdr[5] = 0x801D776C;
        rpg_bat_res_img[5] = 0x801D777C;
        rpg_bat_res_anim[5] = 0x801D7834;
        break;
    case 3:
        rpg_bat_res_hdr[5] = 0x801D31F0;
        rpg_bat_res_img[5] = 0x801D31FC;
        rpg_bat_res_anim[5] = 0x801D323C;
        break;
    case 4:
        rpg_bat_res_hdr[5] = 0x801CF380;
        rpg_bat_res_img[5] = 0x801CF2EC;
        rpg_bat_res_anim[5] = 0x801CF34C;
        break;
    case 5:
        rpg_bat_res_hdr[5] = 0x801CF228;
        rpg_bat_res_img[5] = 0x801CF1B8;
        rpg_bat_res_anim[5] = 0x801CF1F4;
        break;
    case 6:
        rpg_bat_res_hdr[5] = 0x801D32F4;
        rpg_bat_res_img[5] = 0x801D3300;
        rpg_bat_res_anim[5] = 0x801D3358;
        break;
    case 7:
        rpg_bat_res_hdr[5] = 0x801D3348;
        rpg_bat_res_img[5] = 0x801D32AC;
        rpg_bat_res_anim[5] = 0x801D3304;
        break;
    case 8:
        rpg_bat_res_hdr[5] = 0x801CF2B0;
        rpg_bat_res_img[5] = 0x801C7230;
        rpg_bat_res_anim[5] = 0x801C7278;
        break;
    case 9:
        rpg_bat_res_hdr[5] = 0x801CB1FC;
        rpg_bat_res_img[5] = 0x801CB18C;
        rpg_bat_res_anim[5] = 0x801CB1B8;
        break;
    case 10:
        rpg_bat_res_hdr[5] = 0x801CF2EC;
        rpg_bat_res_img[5] = 0x801CF254;
        rpg_bat_res_anim[5] = 0x801CF2A4;
        break;
    case 11:
    case 12:
    case 14:
        break;
    case 13:
        rpg_bat_res_hdr[5] = 0x801D32C8;
        rpg_bat_res_img[5] = 0x801D32D4;
        rpg_bat_res_anim[5] = 0x801D332C;
        break;
    }
}

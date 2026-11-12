/* @SymbolStart: 0x8014B134 */
/* @SymbolSize: 1436 */
/* @RodataBase: 0x8014D9EC */
/* Address: 0x8014B13C (window starts 0x8014B134, cross-prologue head) */
/* Size: 1428 bytes (357 instructions) + 8-byte hoisted head = 1436 */
#include "types.h"

extern void sub_step_up(void);
extern u8 *get_k_work(s32);
extern void k_sub_reset(void);
extern void gnsx(u8 *);
extern void sndisp(u8 *, s32, s32);
extern void xa_wait(void);
extern s32 etc_soudan_para_count(void);
extern s32 get_etc_best_idx(void);

extern u8 p_sei[];
extern u8 now_girl;
extern u8 step_mode;
extern s32 soudan_gaku_low;
extern s32 soudan_unkon_low;
extern s32 soudan_zat_low;
extern s32 soudan_you_low;
extern u8 etc_soudan_main1_1_str0[];
extern u8 etc_soudan_main1_1_str2[];
extern u8 etc_soudan_main1_1_str1[];
extern u8 etc_soudan_main1_1_str3[];
extern u8 etc_soudan_main1_1_str4[];
extern u8 etc_soudan_main1_1_str5[];
extern u8 etc_soudan_main1_1_str8[];
extern u8 etc_soudan_main1_1_str6[];
extern u8 etc_soudan_main1_1_str7[];
extern u8 etc_soudan_main1_1_str9[];
extern u8 etc_soudan_main1_1_str10[];
extern u8 etc_soudan_main1_1_str12[];
extern u8 etc_soudan_main1_1_str11[];

void etc_soudan_main1_1(void)
{
    s32 sel;
    s32 res;
    u8 *p;

    switch (step_mode) {
    case 0:
        sel = etc_soudan_para_count();
        if (sel == 7) {
            step_mode = 16;
        } else if (sel == 0) {
            step_mode = 32;
        } else {
            step_mode = 48;
        }
        break;
    case 16:
        k_sub_reset();
        gnsx(p_sei);
        sndisp(etc_soudan_main1_1_str0, 0, 15);
        step_mode = step_mode + 1;
        break;
    case 17:
        xa_wait();
        break;
    case 18:
        sub_step_up();
        break;
    case 32:
        k_sub_reset();
        gnsx(p_sei);
        sndisp(etc_soudan_main1_1_str1, 0, 15);
        step_mode = step_mode + 1;
        break;
    case 33:
        xa_wait();
        break;
    case 34:
        step_mode = 64;
        break;
    case 48:
        k_sub_reset();
        gnsx(p_sei);
        sndisp(etc_soudan_main1_1_str2, 0, 15);
        step_mode = step_mode + 1;
        break;
    case 49:
        xa_wait();
        break;
    case 50:
        k_sub_reset();
        res = soudan_gaku_low;
        if (res == 0) {
            step_mode = step_mode + 2;
        } else if (res == 1) {
            gnsx(p_sei);
            sndisp(etc_soudan_main1_1_str3, 0, 15);
            step_mode = step_mode + 1;
        } else if (res == 2) {
            gnsx(p_sei);
            sndisp(etc_soudan_main1_1_str4, 0, 15);
            step_mode = step_mode + 1;
        } else {
            gnsx(p_sei);
            sndisp(etc_soudan_main1_1_str5, 0, 15);
            step_mode = step_mode + 1;
        }
        p = get_k_work(3);
        p[4] = 1;
        break;
    case 51:
        xa_wait();
        break;
    case 52:
        k_sub_reset();
        res = soudan_unkon_low;
        if (res == 0) {
            step_mode = step_mode + 2;
        } else if (res == 1) {
            gnsx(p_sei);
            sndisp(etc_soudan_main1_1_str6, 0, 15);
            step_mode = step_mode + 1;
        } else if (res == 2) {
            gnsx(p_sei);
            sndisp(etc_soudan_main1_1_str7, 0, 15);
            step_mode = step_mode + 1;
        } else {
            gnsx(p_sei);
            sndisp(etc_soudan_main1_1_str8, 0, 15);
            step_mode = step_mode + 1;
        }
        p = get_k_work(3);
        p[4] = 1;
        break;
    case 53:
        xa_wait();
        break;
    case 54:
        k_sub_reset();
        if (soudan_zat_low != 0) {
            gnsx(p_sei);
            sndisp(etc_soudan_main1_1_str9, 0, 15);
            p = get_k_work(3);
            p[4] = 1;
            step_mode = step_mode + 1;
        } else {
            step_mode = step_mode + 2;
        }
        break;
    case 55:
        xa_wait();
        break;
    case 56:
        k_sub_reset();
        if (soudan_you_low != 0) {
            gnsx(p_sei);
            sndisp(etc_soudan_main1_1_str10, 0, 15);
            p = get_k_work(3);
            p[4] = 1;
            step_mode = step_mode + 1;
        } else {
            step_mode = step_mode + 2;
        }
        break;
    case 57:
        xa_wait();
        break;
    case 58:
        step_mode = 64;
        break;
    case 64:
        res = get_etc_best_idx();
        if (res == 0) {
            step_mode = 80;
        } else {
            k_sub_reset();
            gnsx(p_sei);
            now_girl = res;
            sndisp(etc_soudan_main1_1_str11, 0, 15);
            step_mode = step_mode + 1;
        }
        break;
    case 65:
        xa_wait();
        break;
    case 66:
        step_mode = 80;
        break;
    case 80:
        k_sub_reset();
        gnsx(p_sei);
        sndisp(etc_soudan_main1_1_str12, 0, 15);
        step_mode = step_mode + 1;
        break;
    case 81:
        xa_wait();
        break;
    case 82:
        sub_step_up();
        break;
    }
}

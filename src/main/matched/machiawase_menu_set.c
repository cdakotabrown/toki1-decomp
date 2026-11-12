/* Function: machiawase_menu_set */
/* Address: 0x80076830 */
/* Size: 932 bytes (233 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=5:a1 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:a0 0x14:rt:a0:a2 0x18:rs:v0:a0 0x18:rt:v0:a0 0x24:rs:v0:a0 0x30:rs:v0:a0 0x3c:rs:v0:a0 0x50:rs:v0:a0 0x50:rt:a0:a2 0x54:rs:v0:a0 */
#include "types.h"

extern u8 machiawase_mode;
extern u8 machiawase_basyo;
extern u8 machiawase_basyo2;
extern u8 machiawase_basyo3;
extern s32 machiawase_menu_set_tbl0[];
extern s32 machiawase_menu_set_str5;
extern s32 machiawase_menu_set_str0;
extern s32 machiawase_menu_set_str1;
extern s32 machiawase_menu_set_str2;
extern s32 machiawase_menu_set_str3;
extern s32 machiawase_menu_set_str4;

extern s32 collect_distinct_active_entry_ids(void *buf);
extern s32 rand(void);
extern void x_taku_menu_set(s32 a0, s32 a1);
extern void strcpy(void *dst, void *src);
extern void strcat(void *dst, void *src);
extern void x_taku_string_set(void *a, void *b, void *c, s32 d);

void machiawase_menu_set(void) {
    s32 padTop;
    s32 sp190;
    s32 sp18C;
    s32 sp188;
    s32 sp108[32];
    s32 pad3[3];
    s8 spBC[64];
    s8 sp7C[64];
    s8 sp3C[64];
    s32 padBot[2];
    s32 result;
    s32 rnd;
    s32 idx;
    s32 i;

    i = 0;
    do {
        sp3C[i + 1] = 0;
        sp7C[i + 1] = 0;
        spBC[i + 1] = 0;
        sp3C[i + 2] = 0;
        sp7C[i + 2] = 0;
        spBC[i + 2] = 0;
        sp3C[i + 3] = 0;
        sp7C[i + 3] = 0;
        spBC[i + 3] = 0;
        i += 4;
        sp3C[i - 4] = 0;
        sp7C[i - 4] = 0;
        spBC[i - 4] = 0;
    } while (i != 64);

    result = collect_distinct_active_entry_ids(sp108);
    if (result >= 3) {

        rnd = rand();
        idx = rnd % 3;
        if (idx < 0) {
            idx = 0;
        }
        sp188 = sp108[idx % 3];
        sp18C = sp108[(idx + 1) % 3];
        sp190 = sp108[(idx + 2) % 3];
        machiawase_mode = 2;
        x_taku_menu_set(1, 3);
        strcpy(spBC, &machiawase_menu_set_str0);
        strcat(spBC, (void *)machiawase_menu_set_tbl0[sp188]);
        strcat(spBC, &machiawase_menu_set_str5);
        strcpy(sp7C, &machiawase_menu_set_str1);
        strcat(sp7C, (void *)machiawase_menu_set_tbl0[sp18C]);
        strcat(sp7C, &machiawase_menu_set_str5);
        strcpy(sp3C, &machiawase_menu_set_str2);
        strcat(sp3C, (void *)machiawase_menu_set_tbl0[sp190]);
        strcat(sp3C, &machiawase_menu_set_str5);
        x_taku_string_set(spBC, sp7C, sp3C, 3);
    } else {

        rnd = rand();
        idx = rnd & 1;
        if (idx < 0) {
            idx = 0;
        }
        sp188 = sp108[idx % 2];
        sp18C = sp108[(idx + 1) % 2];
        sp190 = 255;
        machiawase_mode = 1;
        x_taku_menu_set(1, 2);
        strcpy(spBC, &machiawase_menu_set_str3);
        strcat(spBC, (void *)machiawase_menu_set_tbl0[sp188]);
        strcat(spBC, &machiawase_menu_set_str5);
        strcpy(sp7C, &machiawase_menu_set_str4);
        strcat(sp7C, (void *)machiawase_menu_set_tbl0[sp18C]);
        strcat(sp7C, &machiawase_menu_set_str5);
        x_taku_string_set(spBC, sp7C, (void *)0, 2);
    }
    machiawase_basyo = (u8)sp188;
    machiawase_basyo2 = (u8)sp18C;
    machiawase_basyo3 = (u8)sp190;
}

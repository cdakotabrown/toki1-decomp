/* @SymbolStart: 0x8014AEA8 */
/* @SymbolSize: 424 */
/* @PostProc: patch_insn_reg_field 0xb0:rt:v1:v0 0xb4:rs:v1:v0 0xb4:rt:v1:v0 0xbc:rs:v1:v0 0xbc:rt:v1:v0 0xc0:rt:v1:v0 0xc8:rt:v1:v0 0xcc:rs:v1:v0 0xcc:rt:v1:v0 0xdc:rs:v1:v0 0xdc:rt:v1:v0 0xe8:rt:v1:v0 0xf8:rs:v1:v0 0xf8:rt:v1:v0 0x100:rt:v1:v0 */
/* Address: 0x8014AEAC (1-word cross-prologue head at 0x8014AEA8 absorbed) */
/* Size: 424 bytes (106 instructions) */
#include "types.h"

typedef struct {
    s16 pad;
    s16 h;
} Rec;

extern void printf(s32 *, s32, s32);

extern Rec screen_intmode[];
extern s16 para_bun;
extern s16 para_ri;
extern s16 para_gei;
extern s16 para_un;
extern s16 para_zat;
extern s16 para_you;
extern s16 para_kon;
extern s32 etc_soudan_param_fmt[];
extern s32 soudan_gaku_low;
extern s32 soudan_unkon_low;
extern s32 soudan_zat_low;
extern s32 soudan_you_low;

s32 etc_soudan_para_count(void)
{
    s32 cnt;
    s32 i;
    s32 n;

    soudan_unkon_low = 0;
    soudan_zat_low = 0;
    soudan_you_low = 0;
    soudan_gaku_low = 0;

    cnt = 0;
    i = 0;
    for (n = 1; n != 8; n++) {
        printf(etc_soudan_param_fmt, n, screen_intmode[i + 63].h);
        if (screen_intmode[i + 63].h >= 101) {
            cnt++;
        }
        i++;
    }

    if (para_bun < 101) {
        soudan_gaku_low = soudan_gaku_low + 1;
    }
    if (para_ri < 101) {
        soudan_gaku_low = soudan_gaku_low + 1;
    }
    if (para_gei < 101) {
        soudan_gaku_low = soudan_gaku_low + 1;
    }
    if (para_un < 101) {
        soudan_unkon_low = soudan_unkon_low + 1;
    }
    if (para_kon < 101) {
        soudan_unkon_low = soudan_unkon_low + 2;
    }
    if (para_zat < 101) {
        soudan_zat_low = soudan_zat_low + 1;
    }
    if (para_you < 101) {
        soudan_you_low = soudan_you_low + 1;
    }
    return cnt;
}

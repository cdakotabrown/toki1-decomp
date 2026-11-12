/* @RodataBase: 0x8015D7B4 */
/* @PostProc: swap_registers v0:t6 --unidirectional */
/* @PostProc: swap_registers t7:t6 --unidirectional */
/* @PostProc: swap_registers t8:v0 --unidirectional */
/* @PostProc: swap_registers t9:v0 --unidirectional */
/* @PostProc: swap_registers v1:v0 --unidirectional */
/* @PostProc: swap_registers t0:v0 --unidirectional */
/* Address: 0x801431D4 */
/* Size: 440 bytes (110 insns) */
#include "types.h"

extern s32 k_disp_start(s32);
extern s32 k_reset(s32);
extern s32 basyo_init(s32);

#define now_year (*(u8 *)0x800EE79E)
#define bunka_bukatu (*(s32 *)0x8012B398)

extern u8 bunkasai_basyo_init_str0;
extern u8 bunkasai_basyo_init_str1;
extern u8 bunkasai_basyo_init_str2;
extern u8 bunkasai_basyo_init_str3;
extern u8 bunkasai_basyo_init_str4;
extern u8 bunkasai_basyo_init_str5;
extern u8 bunkasai_basyo_init_str6;
extern u8 bunkasai_basyo_init_str7;
extern u8 bunkasai_basyo_init_str8;
extern u8 bunkasai_basyo_init_str9;
extern u8 bunkasai_basyo_init_str10;
extern u8 bunkasai_basyo_init_str11;

void bunkasai_basyo_init(void) {
    u8 val;
    u32 t6;

    k_reset(0);
    t6 = (u32)bunka_bukatu;
    switch (t6) {
        case 0:
            val = now_year;
            switch (val) {
            case 95: basyo_init((s32)&bunkasai_basyo_init_str0); break;
            case 96: basyo_init((s32)&bunkasai_basyo_init_str1); break;
            case 97: basyo_init((s32)&bunkasai_basyo_init_str2); break;
            }
            break;
        case 1:
            basyo_init((s32)&bunkasai_basyo_init_str3);
            break;
        case 2:
            basyo_init((s32)&bunkasai_basyo_init_str4);
            break;
        case 3:
            val = now_year;
            switch (val) {
            case 95: basyo_init((s32)&bunkasai_basyo_init_str5); break;
            case 96: basyo_init((s32)&bunkasai_basyo_init_str6); break;
            case 97: basyo_init((s32)&bunkasai_basyo_init_str7); break;
            }
            break;
        case 4:
            val = now_year;
            switch (val) {
            case 95: basyo_init((s32)&bunkasai_basyo_init_str8); break;
            case 96: basyo_init((s32)&bunkasai_basyo_init_str9); break;
            case 97: basyo_init((s32)&bunkasai_basyo_init_str10); break;
            }
            break;
        case 5:
            basyo_init((s32)&bunkasai_basyo_init_str11);
            break;
        }
    k_disp_start(1);
}

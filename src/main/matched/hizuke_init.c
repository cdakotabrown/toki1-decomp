/* Function: hizuke_init */
/* Address: 0x80064B74 */
/* Size: 1048 bytes (262 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x90 --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x94 --offset2 0x98 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
#include "types.h"

extern u32 basyo_init_tbl1;
extern u32 basyo_init_tbl2;
extern u32 _sys_default_tpage_set_tbl2;
extern s16 basyo_init_tbl3;

extern u8 class_w;

extern u8 now_year;
extern volatile u8 now_month;
extern volatile u8 now_day;
extern u8 now_youbi;
extern u8 cal_day_kind[];

extern void class_clear_n(s32 a0);
extern s32 keta_n(s32 a0, s32 a1);

void hizuke_init(void) {
    u8 *base;
    s32 i;
    u8 val;
    u32 *pDFC5C;
    u32 *pDFF14;
    u32 *pDFC30;
    s32 result;
    u32 kind;

    pDFC30 = &_sys_default_tpage_set_tbl2;
    pDFF14 = &basyo_init_tbl2;
    pDFC5C = &basyo_init_tbl1;
    base = &class_w;
    i = 0;

    do {
        class_clear_n(i + 16);
        base[1089] = 4;
        base[1090] = 64;
        *(u32 *)&base[1144] = 0x40000000;
        base[1091] = 132;
        *(u32 *)&base[1100] = (u32)pDFC5C;
        *(u32 *)&base[1104] = (u32)pDFF14;
        *(u32 *)&base[1140] = (u32)pDFC30;
        i++;
        base += 68;
        *(s16 *)&base[1040] = basyo_init_tbl3;
        base[1024] = 8;
        base[1025] = 1;
    } while (i < 8);

    result = keta_n(now_year, 2);
    *(s16 *)(&class_w + 1180) = (s16)(result + 68);
    *(s16 *)(&class_w + 1194) = 140;
    *(s16 *)(&class_w + 1198) = -65;

    result = keta_n(now_year, 1);
    *(s16 *)(&class_w + 1248) = (s16)(result + 68);
    *(s16 *)(&class_w + 1262) = 148;
    *(s16 *)(&class_w + 1266) = -65;

    val = now_month;
    if ((u32)val >= 10u) {
        result = keta_n((s16)val, 2);
        val = now_month;
        *(s16 *)(&class_w + 1316) = (s16)(result + 68);
        *(s16 *)(&class_w + 1330) = 140;
        *(s16 *)(&class_w + 1334) = -34;
        result = keta_n((s16)val, 1);
        *(s16 *)(&class_w + 1384) = (s16)(result + 68);
        *(s16 *)(&class_w + 1398) = 148;
        *(s16 *)(&class_w + 1402) = -34;
    } else {
        *(&class_w + 1295) = 0;
        result = keta_n((s16)val, 2);
        val = now_month;
        *(s16 *)(&class_w + 1316) = (s16)(result + 68);
        *(s16 *)(&class_w + 1330) = 255;
        *(s16 *)(&class_w + 1334) = 255;
        result = keta_n((s16)val, 1);
        *(s16 *)(&class_w + 1384) = (s16)(result + 68);
        *(s16 *)(&class_w + 1398) = 144;
        *(s16 *)(&class_w + 1402) = -34;
    }

    val = now_day;
    if ((u32)val >= 10u) {
        result = keta_n((s16)val, 2);
        val = now_day;
        *(s16 *)(&class_w + 1452) = (s16)(result + 68);
        *(s16 *)(&class_w + 1466) = 140;
        *(s16 *)(&class_w + 1470) = -1;
        result = keta_n((s16)val, 1);
        *(s16 *)(&class_w + 1520) = (s16)(result + 68);
        *(s16 *)(&class_w + 1534) = 148;
        *(s16 *)(&class_w + 1538) = -1;
    } else {
        *(&class_w + 1431) = 0;
        result = keta_n((s16)val, 2);
        val = now_day;
        *(s16 *)(&class_w + 1452) = (s16)(result + 68);
        *(s16 *)(&class_w + 1466) = 255;
        *(s16 *)(&class_w + 1470) = 255;
        result = keta_n((s16)val, 1);
        *(s16 *)(&class_w + 1520) = (s16)(result + 68);
        *(s16 *)(&class_w + 1534) = 144;
        *(s16 *)(&class_w + 1538) = -1;
    }

    *(s16 *)(&class_w + 1588) = (s16)(now_youbi + 82);
    *(s16 *)(&class_w + 1602) = 146;
    *(s16 *)(&class_w + 1606) = 31;

    if (now_youbi == 5) {
        *(&class_w + 1631) = 67;
    } else if (now_youbi == 6) {
        *(&class_w + 1631) = 68;
    } else {
        *(&class_w + 1631) = 0;
    }

    kind = cal_day_kind[now_day];
    if (kind == 1 || kind == 2) {
        *(&class_w + 1631) = 68;
    }

    *(s16 *)(&class_w + 1112) = 60;
    *(s16 *)(&class_w + 1126) = 144;
    *(s16 *)(&class_w + 1130) = -18;
}

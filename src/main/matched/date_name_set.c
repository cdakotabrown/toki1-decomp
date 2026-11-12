/* Function: date_name_set */
/* Address: 0x800849C0 */
/* Size: 148 bytes (37 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_trailing_instructions --count 3 --nop-only */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern void strcpy(void* a0, void* a1);
extern void get_p_name(void* a0, s32 a1);
extern void get_g_name(void* a0, s32 a1);

extern u8 p_name[];
extern u8 pp_name[];
extern u8 p_name2[];
extern u8 g_name[];
extern u8 g_name2[];

extern u8 p_sei;
extern u8 p_mei;

extern void* date_name_set_tbl0[];

void date_name_set(s32 arg0) {
    s32 unused;
    u32 tmp;
    (void)&arg0;
    strcpy(p_name, &p_sei);
    strcpy(pp_name, &p_mei);
    tmp = (u8)arg0;
    if (tmp < 14U) {
        get_p_name(p_name2, (s32)tmp);
        strcpy(g_name, date_name_set_tbl0[*(u8*)&arg0]);
        get_g_name(g_name2, (s32)tmp);
    }
}

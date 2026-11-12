/* Function: func_80098E44 */
/* Address: 0x80098E44 */
/* Size: 340 bytes (85 instructions) */
/* Region: Library (0x80086000+) */
/* Likely Symbol: GsInitGraph (from o.bin) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O1 */
/* @PostProc: maspsx --aspsx-version=2.67 */
/* @PostProc: strip_leading_instructions --count 22 */
/* @PostProc: custom_script custom_func_80098E44.py */
#include "types.h"

extern void ResetGraph(s32);
extern void PutDrawEnv(void *);
extern s32 GetVideoMode(void);
extern void PutDispEnv(void *);

void func_80098E44(s16 arg0, s16 arg1, s32 arg2, s8 arg3, u16 arg4) {
    s16 *p;
    s32 mode;

    ResetGraph(0);
    *(s16 *)0x801316AA = 0;
    p = (s16 *)0x801316A8;
    *p = 0;
    *(s16 *)0x801316B2 = 0;
    *(s16 *)0x801316B0 = 0;
    *(s16 *)0x801316AE = 0;
    *(s16 *)0x801316AC = 0;
    *(s16 *)0x801316B4 = 0;
    *(s8 *)0x801316B6 = arg3;
    *(s8 *)0x801316B7 = 0;
    *(s8 *)0x801316B8 = 0;
    PutDrawEnv((char *)p - 8);
    p = (s16 *)0x80131700;
    *p = 0;
    *(s16 *)0x80131702 = 0;
    *(s16 *)0x80131704 = arg0;
    *(s16 *)0x80131706 = arg1;
    *(s16 *)0x80131708 = 0;
    *(s16 *)0x8013170A = 0;
    *(s16 *)0x8013170C = 0;
    *(s16 *)0x8013170E = 0;
    mode = arg2;
    if (GetVideoMode() == 1) {
        *(s16 *)0x8013170A = 24;
        *(s8 *)0x80131712 = 1;
    }
    *(s8 *)0x80131710 = (s8)(mode & 1);
    *(s16 *)0x80131714 = (s16)(arg2 & 4);
    *(s8 *)0x80131711 = (s8)arg4;
    PutDispEnv(p);
}

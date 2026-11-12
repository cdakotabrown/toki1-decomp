/* Function: func_800A0218 */
/* Address: 0x800A0218 */
/* Size: 412 bytes (103 instructions) */
/* Region: Library (0x80086000+) */
/* CONFIRMED UNMATCHABLE FROM C (2026-03-28) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_ContPortaTime.py */
#include "types.h"

extern u8 D_800EBD62;
extern void (*GPU_printf)(s32, s32 *);
extern s32 libgpu_clearotag_trace_fmt;

s32 *func_800A0218(s32 *arg0, s32 arg1) {
    s32 *a3;
    s32 a2;
    s32 rem;
    u32 a0;
    s32 *v1;
    u32 t8;
    u32 t0;

    a3 = arg0;
    a2 = arg1;

    if ((u32)D_800EBD62 >= 2u) {
        GPU_printf((s32)&libgpu_clearotag_trace_fmt, a3);
    }

    a2 = a2 - 1;

    rem = a2 & 3;

    if (a2 == 0) {
        a0 = 0x00FFFFFF;
        *a3 = (s32)a0;
        return a3;
    }

    rem = (s32)-(s32)rem;

    if (rem != 0) {
        s32 end = rem + a2;
        a0 = 0x00FFFFFF;
        do {
            t8 = (u32)*a3 & a0;
            *(volatile s32 *)a3 = (s32)t8;
            t8 = (u32)*a3;
            a2--;
            v1 = a3 + 1;
            t0 = (u32)v1 ^ t8;
            t0 = (t0 << 8) >> 8;
            *a3 = (s32)(t0 ^ t8);
            a3 = v1;
        } while (end != a2);

        if (a2 == 0) {
            *a3 = (s32)a0;
            return a3;
        }
    } else {
        a0 = 0x00FFFFFF;
    }

    do {
        t8 = (u32)*a3 & a0;
        *(volatile s32 *)a3 = (s32)t8;
        t8 = (u32)*a3;
        a2 -= 4;
        v1 = a3 + 1;
        t0 = (u32)v1 ^ t8;
        t0 = (t0 << 8) >> 8;
        *a3 = (s32)(t0 ^ t8);
        a3 = v1;

        t8 = (u32)*a3 & a0;
        *(volatile s32 *)a3 = (s32)t8;
        t8 = (u32)*a3;
        v1 = a3 + 1;
        t0 = (u32)v1 ^ t8;
        t0 = (t0 << 8) >> 8;
        *a3 = (s32)(t0 ^ t8);
        a3 = v1;

        t8 = (u32)*a3 & a0;
        *(volatile s32 *)a3 = (s32)t8;
        t8 = (u32)*a3;
        v1 = a3 + 1;
        t0 = (u32)v1 ^ t8;
        t0 = (t0 << 8) >> 8;
        *a3 = (s32)(t0 ^ t8);
        a3 = v1;

        t8 = (u32)*a3 & a0;
        *(volatile s32 *)a3 = (s32)t8;
        t8 = (u32)*a3;
        v1 = a3 + 1;
        t0 = (u32)v1 ^ t8;
        t0 = (t0 << 8) >> 8;
        *a3 = (s32)(t0 ^ t8);
        a3 = v1;
    } while (a2 != 0);

    *a3 = (s32)a0;
    return a3;
}

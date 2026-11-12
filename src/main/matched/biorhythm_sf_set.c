/* Address: 0x8006BE48, Size: 1376 bytes (344 insns incl. tail pad) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_AS1_TIE_REJECT=10,13,16 IDO52_AS1_XBB_SKIP_CAND=5 */
/* @PostProc: remap_stack_slots --map '40:52,52:44,62:50' --keep-frame */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_insn_reg_field 0x88:rs:t2:v0 0x88:rt:v0:t2 0xc0:rs:t4:v0 0xc0:rt:v0:t4 0xf8:rs:t6:v0 0xf8:rt:v0:t6 0x130:rs:t8:v0 0x130:rt:v0:t8 0x3a0:rs:t2:v0 0x3a0:rt:v0:t2 0x3d8:rs:t4:v0 0x3d8:rt:v0:t4 0x438:rt:t7:t5 0x43c:rt:t7:t5 0x440:rt:t5:t6 0x444:rs:t5:t6 0x444:rd:t6:t7 0x448:rs:t7:t5 0x458:rs:t7:t5 0x480:rd:t8:t0 0x498:rd:t9:t8 0x524:rd:t2:t9 0x48c:rs:t6:t7 0x48c:rt:t8:t0 0x4a0:rd:t9:t8 0x4a8:rt:t9:t8 0x51c:rt:t3:v0 0x52c:rd:t2:t9 0x530:rs:t2:t9 0x534:rt:t3:v0 0x53c:rt:t3:v0 */
#include "types.h"

extern s32 biorhythm_sf2(s32, s32, s32, s32);
extern s32 rand(void);

extern u16 pass_days;
extern s32 sched_icon;
extern s16 para_bun;
extern s16 para_ri;
extern s16 para_gei;
extern s16 para_un;
extern u16 basyo_pack;
extern s16 para_zat;
extern s16 para_you;
extern s16 para_str;
extern struct { u16 h; u16 pad; } bukatu_rec[];

s32 biorhythm_sf_set(s32 arg0)
{
    u16 sp50;
    s32 t0;
    s32 t1;
    s32 v0;
    s32 v1;
    s32 probes;

    v0 = pass_days;
    pass_days = v0 + arg0;
    sp50 = v0;
    t0 = 0;

    switch (sched_icon & 0xF) {
    case 0:
        v0 = biorhythm_sf2(0, 0, 1, 16);
        t0 = v0 + para_bun / 16;
        break;
    case 1:
        v0 = biorhythm_sf2(0, 0, 1, 16);
        t0 = v0 + para_ri / 16;
        break;
    case 2:
        v0 = biorhythm_sf2(0, 1, 0, 16);
        t0 = v0 + para_gei / 16;
        break;
    case 3:
        v0 = biorhythm_sf2(1, 0, 0, 16);
        t0 = v0 + para_un / 16;
        break;
    case 4:
        v1 = (u32)basyo_pack >> 12;
        switch (v1) {
        case 0:
            t0 = biorhythm_sf2(0, 0, 1, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        case 1:
            t0 = biorhythm_sf2(0, 1, 0, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        case 2:
            t0 = biorhythm_sf2(0, 0, 1, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        case 3:
            t0 = biorhythm_sf2(0, 0, 1, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        case 4:
            t0 = biorhythm_sf2(0, 1, 0, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        case 5:
            t0 = biorhythm_sf2(0, 1, 0, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        case 6:
            t0 = biorhythm_sf2(1, 0, 0, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        case 7:
            t0 = biorhythm_sf2(1, 0, 0, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        case 8:
            t0 = biorhythm_sf2(1, 0, 0, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        case 9:
            t0 = biorhythm_sf2(1, 0, 0, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        case 10:
            t0 = biorhythm_sf2(1, 0, 0, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        }
        t0 += (u32)bukatu_rec[v1].h >> 4;
        break;
    case 5:
        v0 = biorhythm_sf2(0, 1, 0, 16);
        t0 = v0 + para_zat / 16;
        break;
    case 6:
        v0 = biorhythm_sf2(0, 1, 0, 16);
        t0 = v0 + para_you / 16;
        break;
    case 7:
        biorhythm_sf2(1, 1, 1, 16);
        t0 = 16;
        break;
    }

    t0 += 5;
    if (para_str < 97) goto small;
    t1 = 4;
    goto have_t1;
small:
    t1 = para_str / 32;
have_t1:
    v0 = rand();
    t0 = ((8 - t1) * (v0 % (t0 + 4))) / 8;

    probes = 0;
    if (biorhythm_sf2(1, 0, 0, 3) == 0) {
        probes = 1;
    }
    if (biorhythm_sf2(0, 1, 0, 3) == 0) {
        probes += 1;
    }
    if (biorhythm_sf2(0, 0, 1, 3) == 0) {
        probes += 1;
    }
    if (probes == 3) {
        t0 = t0 / 4;
    }

    pass_days = sp50;
    if (t0 >= 5) {
        return 0;
    }
    return 1;
}

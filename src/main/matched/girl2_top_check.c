/* Function: girl2_top_check */
/* Address: 0x800711D0 */
/* Debug Symbol: girl2_top_check (REVERTED - offset 136, uncertain) (from o.bin, high match) */
/* Size: 368 bytes (92 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 via ido-static-recomp */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script postproc_shadow_file_name.py */
#include "types.h"

extern void get_grp(s32 *arg0, s16 *arg1);

extern u8 denwa_cnt;
extern u8 ev_flag4f;
extern s32 girl2_flag;

s32 girl2_top_check(void) {
    s32 sp5C[5];
    s16 sp3C[16];
    s32 pad[4];
    s32 best_idx;
    s32 best_val;
    s32 i;
    s32 val;
    s16 *ptr;

    (void)pad;
    get_grp(&sp5C[0], sp3C);

    best_idx = 0;
    best_val = 0;

    if (sp3C[0] < 500) {
        if (denwa_cnt >= 70U) {
            if (ev_flag4f == 2) {
                return 0;
            }
        }
    }

    if (sp3C[2] < 500) {
        return 0;
    }

    if ((girl2_flag << 30) < 0) {
    } else {
        return 0;
    }

    if (sp3C[0] > 0) {
        best_val = sp3C[0];
    }

    if (best_val < sp3C[1]) {
        best_idx = 1;
        best_val = sp3C[1];
    }

    if (best_val < sp3C[2]) {
        best_idx = 2;
        best_val = sp3C[2];
    }

    i = 3;
    ptr = &sp3C[3];
    do {
        val = ptr[0];
        if (best_val < val) {
            best_idx = i;
            best_val = val;
        }

        val = ptr[1];
        if (best_val < val) {
            best_idx = i + 1;
            best_val = val;
        }

        val = ptr[2];
        if (best_val < val) {
            best_idx = i + 2;
            best_val = val;
        }

        val = ptr[3];
        if (best_val < val) {
            best_idx = i + 3;
            best_val = val;
        }

        ptr += 4;
        i += 4;
    } while (i != 11);

    if (best_idx == 2) {
        return 1;
    }

    return 0;
}

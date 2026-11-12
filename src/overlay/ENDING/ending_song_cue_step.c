/* @CompileFlags: -Wc,-pic0 */
/* @SymbolSize: 384 */
/* ending_song_cue_step - ENDING overlay, Address: 0x8013BC88, Size: 384 bytes (96 insns) */
/* @PostProc: swap_instructions --offset1 212 --offset2 216 */
/* @PostProc: patch_insn_reg_field 0x24:rt:a1:a2 0x28:rs:a1:a2 0x34:rs:a1:a2 0x3c:rt:a2:a1 0x40:rs:a1:a2 0x4c:rs:a1:a2 0x58:rs:a1:a2 0x64:rs:a1:a2 0x70:rs:a1:a2 0x80:rs:a1:a2 0x98:rd:a2:a1 0x9c:rt:a2:a1 0xa0:rd:a2:a1 0xcc:rt:a2:a1 0xd8:rt:a2:a1 0xdc:rt:a1:a2 0xe4:rs:a2:a1 0xe8:rt:a2:a1 0xec:rs:a1:a2 0x104:rt:a2:a1 0x108:rt:a2:a1 0x110:rt:a2:a1 */
#include "types.h"

typedef struct {
    s16 cue[13];
} CueTable;

extern CueTable ending_song_cue_step_tbl;
extern s16 ending_song_frame;

extern void k_reset(s32);
extern void ending_song_line_disp(s16);
extern s32 get_CD_State(void);
extern s32 get_CD_l_com(void);
extern void sub_sub_step_up(void);

void ending_song_cue_step(void)
{
    CueTable tbl;
    s16 frame;
    s16 hit;
    s16 i;
    s32 arg;
    s32 pad;

    tbl = ending_song_cue_step_tbl;
    frame = ending_song_frame;
    hit = 255;
    i = 0;
    do {
        if (frame >= tbl.cue[i]) {
            hit = i;
        }
        i = i + 1;
    } while (i < 13);

    if (frame == 1000) {
        k_reset(1);
        frame = ending_song_frame;
    }

    if (hit != 255) {
        if (frame == tbl.cue[hit]) {
            k_reset(1);
            ending_song_line_disp(hit);
            frame = ending_song_frame;
        }
    }

    if (frame == 5430) {
        k_reset(1);
    }

    pad = get_CD_State();
    if ((pad & 4) != 0) {
        ending_song_frame = ending_song_frame + 1;
    }

    if (get_CD_l_com() == 9) {
        sub_sub_step_up();
    }
}

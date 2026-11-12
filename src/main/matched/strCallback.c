/* Function: strCallback */
/* Address: 0x80056904 */
/* Size: 632 bytes (158 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x800568FC */
/* @SymbolSize: 640 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=6:t8 */
/* @PostProc: remap_stack_slots --map 32:48,34:50 */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern void StCdInterrupt(void);
extern void LoadImage(s16 *pos, s32 arg1);
extern void DrawSync(s32 arg);
extern void DecDCTout(s32 arg0, s32 arg1);

extern s32 movie_main_tbl1;
extern s32 song_set_tbl0;
extern s32 movie_offset_y;
extern s32 StCdIntrFlag;

typedef struct {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
} Xywh;

typedef struct {
    u8 pad0[16];
    s32 field_16;
    s16 sub0_0;
    s16 sub0_1;
    s16 sub0_2;
    s16 pad1[5];
    s32 counter;
    Xywh xywh;
    s32 field_48;
} SpotState;

extern SpotState *movie_state;

void strCallback(void)
{
    Xywh pos;
    s32 flag;

    flag = movie_main_tbl1 & 1;

    if (flag != 0) {
        if (StCdIntrFlag != 0) {
            StCdInterrupt();
            flag = movie_main_tbl1 & 1;
            StCdIntrFlag = 0;
        }
    }

    pos = movie_state->xywh;

    if (flag != 0) {
        pos.x = pos.x + (song_set_tbl0 * 3 + 2) / 3;
        pos.y = pos.y + (movie_offset_y * 3 + 2) / 3;
    } else {
        pos.x = pos.x + song_set_tbl0;
        pos.y = pos.y + movie_offset_y;
    }

    LoadImage((s16 *)&pos, movie_state->field_16);
    DrawSync(0);

    movie_state->xywh.x = movie_state->xywh.x + movie_state->xywh.w;

    if (movie_state->xywh.x <
        ((SpotState *)((u8 *)movie_state + movie_state->counter * 8))->sub0_2 +
        ((SpotState *)((u8 *)movie_state + movie_state->counter * 8))->sub0_0) {

        DecDCTout(movie_state->field_16,
            ((s32)movie_state->xywh.w * (s32)movie_state->xywh.h) / 2);
        return;
    }

    movie_state->field_48 = 1;

    if (movie_state->counter != 0) {
        movie_state->counter = 0;
    } else {
        movie_state->counter = 1;
    }

    movie_state->xywh.x = ((SpotState *)((u8 *)movie_state + movie_state->counter * 8))->sub0_0;
    movie_state->xywh.y = ((SpotState *)((u8 *)movie_state + movie_state->counter * 8))->sub0_1;
}

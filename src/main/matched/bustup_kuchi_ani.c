/* Address: 0x80063808 */
/* Size: 552 bytes (138 instructions) */
/* Region: Game (IDO 5.2 -O2, -Wc,-pic1) */
/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=4:1:1 IDO52_UGEN_TEMP_REG_INPLACE=13,25 IDO52_UGEN_TEMP_REG=13:a0,25:v0 IDO52_FORCE_WEB_COLOR=39:t0,34:a3,142:a1,51:v1,75:a2 IDO52_AS1_TIE_REJECT=148,151,153 IDO52_AS1_TIE_TAKE=177 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s16 entrySample;
    u8  pad02[0x31];
    u8  flag80;
    u8  pad34[0x10];
    s16 altBucketSrc;
    u8  pad46[0x1E];
    u32 padState0;
} PadBlockA;

typedef struct {
    u32 padState1;
    u8  pad04[0x0B];
    u8  bucket;
    u8  pad10[0x14];
    u32 padState2;
    u8  pad28[0x0B];
    u8  bucketShifted;
} PadBlockB;

extern PadBlockA bustup_cw_frame;
extern PadBlockB Sprites;

extern u8  voice_level;
extern volatile s32 frame_counter;
extern u8  now_girl;
extern s32 pad_data;
extern s16 kuchi_level[];
extern u8  bustup_kuchi_ani_tbl0[][3];
extern s32 bustup_kuchi_ani_tbl1;

extern s32 get_CD_State(void);
extern s32 SD_CDLevelGet(void);

void bustup_kuchi_ani(void)
{
    s32 v0;
    s32 a3;
    s32 ef;
    s32 saved;
    u8 ee79c;
    s32 mx;
    s16 *p;
    s16 *pe;
    u8 *row;
    s32 a6;
    s32 i;

    bustup_cw_frame.flag80 = 0x80;
    bustup_cw_frame.padState0 = bustup_cw_frame.padState0 & 0x7FFFFFFF;
    Sprites.padState1 = Sprites.padState1 & 0x7FFFFFFF;
    Sprites.padState2 = Sprites.padState2 & 0x7FFFFFFF;

    saved = bustup_cw_frame.entrySample;
    ee79c = voice_level;

    if (ee79c != 0) {
        v0 = get_CD_State();
        if (v0 & 4) {
            a3 = SD_CDLevelGet() + 1;
        } else {
            a3 = 0;
        }

        ef = frame_counter;
        kuchi_level[ef & 3] = a3;

        mx = 0;
        p = &kuchi_level[0];
        pe = &kuchi_level[4];
        do {
            v0 = *p;
            p++;
            if (mx < v0) {
                mx = v0;
            }
        } while (p < pe);
        row = bustup_kuchi_ani_tbl0[now_girl];
        a6 = 0;
        for (i = 0; i != 3; i++) {
            if (*row < (u32)a3) {
                a6 = i + 1;
            }
            row++;
        }
        if (!(a6 < 4)) {
            a6 = 3;
        }

        if (Sprites.bucket != 0 && (ef & 1)) {

        } else if (a3 == 0) {
            Sprites.bucket = 0;
        } else if (a6 == 0) {
            Sprites.bucket = 0;
        } else if (a6 == 1) {
            Sprites.bucket = 16;
        } else if (a6 == 2) {
            Sprites.bucket = 32;
        } else if (a6 >= 3) {
            Sprites.bucket = 48;
        }

        if (pad_data & 0x80) {
            bustup_kuchi_ani_tbl1 = 0;
        }
        if (bustup_kuchi_ani_tbl1 < a3) {
            bustup_kuchi_ani_tbl1 = a3;
        }
    } else {
        Sprites.bucket = (u8)((s32)bustup_cw_frame.altBucketSrc << 4);
    }

    Sprites.bucketShifted = (u8)(saved << 5);
}

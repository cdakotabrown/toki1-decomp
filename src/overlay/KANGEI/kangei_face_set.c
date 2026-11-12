/* @RodataBase: 0x80139840 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:1:deny IDO52_AS1_NO_PASS1_PROP=8 IDO52_AS1_XBB_SKIP_CAND=occ:1 */
/* Address: 0x80134734, Size: 688 bytes (172 insns) */
#include "types.h"

typedef struct {
    u16 like;
    u16 pad;
} Aff;

typedef struct {
    u8 pad0[0xF4];
    u16 info;
} Save;

extern Save screen_w;
extern u8 now_girl;
extern u8 now_year;
extern Aff bukatu_rec[];
extern u16 basyo_pack;
extern short kangei_now_s_in;
extern short kangei_now_s;

kangei_face_set()
{
    unsigned mood;
    int idx;
    u16 like;
    short grade;
    short extra;

    mood = get_g_zyotai_s(now_girl);
    mood = mood & 0x7F;
    if (mood < 2) {
        mood = 0;
    } else if (mood == 2) {
        mood = 1;
    } else if (mood == 3) {
        mood = 2;
    } else {
        mood = 3;
    }
    idx = screen_w.info >> 12;
    like = bukatu_rec[idx].like;
    grade = like / 50;
    if (like >= 100) {
        grade = 2;
    }
    if (like >= 200) {
        grade = 3;
    }
    if (like >= 356) {
        grade = 4;
    }
    kangei_now_s_in = 0;
    switch (now_girl) {
    case 0:
        extra = idx;
        if (idx == 9) {
            extra = 8;
            if (like >= 356) {
                grade = 5;
            }
        }
        if (idx == 10) {
            extra = 8;
            if (like >= 356) {
                grade = 6;
            }
        }
        kangei_now_s = extra * 20 + grade * 4 + mood + 3;
        if (now_year == 97 && idx == 6) {
            kangei_now_s = mood + 191;
        }
        break;
    case 1:
        kangei_now_s = idx * 20 + grade * 4 + mood + 8;
        break;
    case 2:
        kangei_now_s = idx * 20 + grade * 4 + mood - 32;
        break;
    case 3:
        kangei_now_s = idx * 20 + grade * 4 + mood - 72;
        break;
    case 4:
        kangei_now_s = idx * 24 + grade * 4 + mood - 137;
        if (now_year == 97 && (basyo_pack >> 12) == 6) {
            kangei_now_s = mood + 27;
        }
        break;
    case 5:
        kangei_now_s = grade * 4 + mood + 7;
        break;
    case 6:
        kangei_now_s = grade * 4 + mood + 7;
        break;
    default:
        kangei_now_s = grade * 4 + mood + 3;
        break;
    }
    kangei_girl_msg_init();
    sub_sub_step_up();
}

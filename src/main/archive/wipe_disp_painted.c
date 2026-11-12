/* Function: wipe_disp */
/* Likely Symbol: wipe_disp (from o.bin) */
/* Address: 0x8007C968 */
/* Size: 2732 bytes (683 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_8007C968.py */
/* @PostProc: scheduler_normalizer */
/* @PostProc: custom_script remap_post_scheduler_8007C968.py */
#include "types.h"

extern s32 frame_counter;
extern s32 step_counter;
extern u8 now_month;
extern s32 wipe_kind;
extern s32 fade_flag;
extern u8 fade_bri;

extern void srand(s32);
extern void line_set(s16, s16, s16, s16, s32, s32, s32);
extern void wipe_tile_disp(s32);

s32 wipe_disp(void) {
    s32 v1;
    s32 s2;
    s16 s0;
    s32 s1;
    s32 s3;
    s32 s4;
    s32 s5;
    s32 v0;

    srand(frame_counter);

    v1 = step_counter;

    if (v1 == 0) {
        wipe_kind = (u32)now_month % 7u;
    }

    if (wipe_kind != 6) {
        fade_flag = 2;
    }

    v0 = (u8)(fade_bri + 8);
    fade_bri = (u8)v0;

    if ((u32)v0 >= 128) {
        fade_bri = 128;
    }

    s2 = 0;

    switch (wipe_kind - 1) {
    case 0:
        s2 = v1 * 5;
        if (s2 >= 80) {
            break;
        }
        s0 = -41;
        do {
            line_set(-160, (s16)(s0 - s2), 160, (s16)(s0 - s2), 0, 0, 13);
            line_set(-160, (s16)(s2 - 41), 160, (s16)(s2 - 41), 0, 0, 13);
            s2 = (s16)(s2 + 1);
        } while (s2 < 80);
        v1 = step_counter;
        v1 = v1 + 1;
        break;

    case 1:
        s2 = (s16)v1;
        if (s2 >= 16) {
            break;
        }
        do {
            s0 = 0;
            do {
                line_set(-160, (s16)((s0 << 4) + s2 - 120), 160, (s16)((s0 << 4) + s2 - 120), 0, 0, 13);
                s0 = (s16)(s0 + 1);
            } while (s0 < 10);
            s2 = (s16)(s2 + 1);
        } while (s2 < 16);
        v1 = step_counter;
        v1 = v1 + 1;
        break;

    case 2:
        s2 = v1;
        s2 = (s16)(s2 << 5);
        if (s2 >= 480) {
            break;
        }
        do {
            if (s2 < 160) {
                line_set((s16)(s2 - 160), -120, -160, (s16)(s2 - 120), 0, 0, 13);
            } else if (s2 < 320) {
                line_set((s16)(s2 - 160), -120, (s16)(s2 - 320), 39, 0, 0, 13);
            } else {
                line_set(160, (s16)(s2 - 440), (s16)(s2 - 320), 39, 0, 0, 13);
            }
            s2 = (s16)(s2 + 1);
        } while (s2 < 480);
        v1 = step_counter;
        v1 = v1 + 1;
        break;

    case 3:
        v0 = v1 << 6;
        s2 = (s16)v0;
        if (s2 < 480) {
            do {
                if (s2 < 160) {
                    line_set((s16)(s2 - 160), -120, -160, (s16)(s2 - 120), 0, 0, 13);
                } else if (s2 < 320) {
                    line_set((s16)(s2 - 160), -120, (s16)(s2 - 320), 39, 0, 0, 13);
                } else {
                    line_set(160, (s16)(s2 - 440), (s16)(s2 - 320), 39, 0, 0, 13);
                }
                s2 = (s16)(s2 + 4);
            } while (s2 < 480);
        }

        v1 = step_counter;
        v0 = v1 << 6;
        s2 = (s16)v0;
        if (s2 < 480) {
            do {
                if (s2 < 160) {
                    line_set((s16)(s2 - 159), -120, -160, (s16)(s2 - 119), 0, 0, 13);
                } else if (s2 < 320) {
                    line_set((s16)(s2 - 159), -120, (s16)(s2 - 319), 39, 0, 0, 13);
                } else {
                    line_set(160, (s16)(s2 - 439), (s16)(s2 - 319), 39, 0, 0, 13);
                }
                s2 = (s16)(s2 + 4);
            } while (s2 < 480);
        }

        v1 = step_counter;
        v0 = v1 << 6;
        if ((u32)v0 >= 480) {
            s0 = v0 - 480;
        } else {
            s0 = 0;
        }
        s0 = (s16)s0;
        s2 = (s16)s0;
        if (s0 < 480) {
            do {
                if (s2 < 160) {
                    line_set((s16)(s2 - 158), -120, -160, (s16)(s2 - 118), 0, 0, 13);
                } else if (s2 < 320) {
                    line_set((s16)(s2 - 158), -120, (s16)(s2 - 318), 39, 0, 0, 13);
                } else {
                    line_set(160, (s16)(s2 - 438), (s16)(s2 - 318), 39, 0, 0, 13);
                }
                s2 = (s16)(s2 + 4);
            } while (s2 < 480);
        }

        s2 = (s16)s0;
        v1 = step_counter;
        if (s0 >= 480) {
            break;
        }
        do {
            if (s2 < 160) {
                line_set((s16)(s2 - 157), -120, -160, (s16)(s2 - 117), 0, 0, 13);
            } else if (s2 < 320) {
                line_set((s16)(s2 - 157), -120, (s16)(s2 - 317), 39, 0, 0, 13);
            } else {
                line_set(160, (s16)(s2 - 437), (s16)(s2 - 317), 39, 0, 0, 13);
            }
            s2 = (s16)(s2 + 4);
        } while (s2 < 480);
        v1 = step_counter;
        v1 = v1 + 1;
        break;

    case 4:
        s2 = (s16)(v1 * 10);
        if (s2 >= 160) {
            break;
        }
        do {
            s0 = (s16)(160 - s2);
            s1 = (s16)(s0 - 160);
            s5 = (s16)(159 - s0);
            if (s0 >= 0) {
                v0 = s0 >> 1;
            } else {
                v0 = (s0 + 1) >> 1;
            }
            s3 = (s16)(v0 - 120);
            line_set(s1, s3, s5, s3, 0, 0, 13);
            s4 = (s16)(39 - v0);
            line_set(s1, s4, s5, s4, 0, 0, 13);
            line_set(s1, s3, s1, s4, 0, 0, 13);
            line_set((s16)(160 - s0), s3, s5, s4, 0, 0, 13);
            s2 = (s16)(s2 + 1);
        } while (s2 < 160);
        v1 = step_counter;
        v1 = v1 + 1;
        break;

    case 5:
        if ((u32)v1 < 16) {
            step_counter = v1;
            wipe_tile_disp(v1 >> 1);
        } else {
            wipe_tile_disp(8);
        }
        v1 = step_counter;
        v1 = v1 + 1;
        break;

    default:
        s0 = v1;
        do {
            s0 = (s16)(s0 << 1);
            s1 = (s16)(s2 << 5);
            if (s1 < 160) {
                if (s0 < 32) {
                    do {
                        v0 = s1 + s0;
                        line_set((s16)(v0 - 160), -120, -160, (s16)(v0 - 120), 0, 0, 13);
                        s0 = (s16)(s0 + 1);
                    } while (s0 < 32);
                    v1 = step_counter;
                }
            } else if (s1 < 320) {
                if (s0 < 32) {
                    do {
                        v0 = s1 + s0;
                        line_set((s16)(v0 - 160), -120, (s16)(v0 - 320), 40, 0, 0, 13);
                        s0 = (s16)(s0 + 1);
                    } while (s0 < 32);
                    v1 = step_counter;
                }
            } else {
                if (s0 < 32) {
                    do {
                        v0 = s1 + s0;
                        line_set(160, (s16)(v0 - 440), (s16)(v0 - 320), 40, 0, 0, 13);
                        s0 = (s16)(s0 + 1);
                    } while (s0 < 32);
                    v1 = step_counter;
                }
            }
            s2 = (s16)(s2 + 1);
            s0 = v1;
        } while (s2 < 15);
    }

    v1 = v1 + 1;
    step_counter = v1;
    if ((u32)v1 < 16) {
        return 0;
    }
    fade_flag = 2;
    return 1;
}

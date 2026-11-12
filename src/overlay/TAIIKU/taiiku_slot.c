/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=t9:10 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xb4 */
/* @PostProc: patch_insn_reg_field 0xa4:rd:t2:t3 0xb0:rt:t4:t2 0xb4:rs:t2:t3 0xb4:rt:t3:t4 0xbc:rs:t3:t2 0x17c:rt:t2:t3 0x184:rt:t2:t3 0x18c:rd:t3:t2 0x190:rt:t3:t2 0x194:rt:v1:t4 0x19c:rs:v1:t4 0x19c:rt:v1:t5 0x1a0:rs:v1:t5 0x1a0:rt:t4:t8 0x1a4:rs:t4:t8 0x1ac:rt:v1:t5 */
/* taiiku_slot -- TAIIKU overlay, Address: 0x8013FE34, Size: 488 bytes */
#include "types.h"

typedef union Slot {
    s32 w;
    struct {
        s16 lo;
        s16 hi;
    } h;
} Slot;

typedef struct Rec12 {
    s32 w[3];
} Rec12;

typedef struct Frame {
    Rec12 r;
    s32 pad;
} Frame;

extern Rec12 taiiku_slot_tbl;
extern Slot taiiku_tama_cpu_pos[];
extern u16 taiiku_tama_cpu_tmr[];
extern u8 taiiku_tama_cpu_stop[];
extern u8 taiiku_tama_rank[];

extern s32 get_taiiku_seg(s32);
extern s32 rand(void);

void taiiku_slot(void)
{
    u32 mark;
    Frame buf;
    Slot *acc;
    u8 *flag;
    u8 *scan;
    s32 i;
    s32 j;
    s32 off;
    s32 t;

    buf.r = taiiku_slot_tbl;

    flag = taiiku_tama_cpu_stop;
    acc = taiiku_tama_cpu_pos;
    mark = 0xFFFF;
    i = 0;
    off = 0;
    do {
        if (acc->h.hi < 3584) {
            if (*flag == 0) {
                acc->w = acc->w + *(s32 *)((u8 *)&buf + off);
            }
        } else if (taiiku_tama_cpu_tmr[i] != mark) {
            taiiku_tama_cpu_tmr[i] = (u16)mark;
            j = 0;
            if (taiiku_tama_rank[0] != 255) {
                scan = taiiku_tama_rank;
                do {
                    j = j + 1;
                    scan = scan + 1;
                } while (*scan != 255);
            }
            taiiku_tama_rank[j] = (u8)(i + 1);
        }

        if (get_taiiku_seg(i + 1) == 1 || get_taiiku_seg(i + 1) == 2 ||
            get_taiiku_seg(i + 1) == 4 || get_taiiku_seg(i + 1) == 5) {
            if ((rand() & 0xFFF) == 0) {
                *flag = 1;
            }
        }

        off = off + 4;
        acc = acc + 1;
        if (*flag == 1U) {
            t = taiiku_tama_cpu_tmr[i] + 1;
            taiiku_tama_cpu_tmr[i] = (u16)t;
            if ((u32)(t & 0xFFFF) >= 241) {
                *flag = 0;
                taiiku_tama_cpu_tmr[i] = 0;
            }
        }

        i = i + 1;
        flag = flag + 1;
    } while (i != 3);
}

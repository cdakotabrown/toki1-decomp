/* @SymbolSize: 300 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=1:-1:2:6 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x7c */
/* @PostProc: patch_insn_reg_field 0x44:rt:s2:s3 0x48:rt:s3:s2 0x4c:rs:s3:s2 0x4c:rt:s3:s2 0x50:rs:s2:s3 0x50:rt:s2:s3 0x70:rs:s3:s2 0xdc:rt:t5:t6 0xe0:rt:t5:t6 0xe8:rt:t5:t6 0xec:rt:t5:t6 0xf4:rt:s2:s3 */
/* Function: etc_rec_collect */
/* Overlay: ETC */
#include "types.h"

typedef struct { u8 b[8]; } Blk8;

extern void bzero();
extern void printf();

extern u8 save_buf[];
extern u8 screen_w[];
extern u8 D_800EEF60[];
extern u8 album_work[];
extern u8 etc_rec_row_fmt[];
extern u8 etc_album_mode_save_fmt[];
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 now_girl;
extern s8 opt_flag;
extern u8 save_rec_cnt;

void etc_rec_collect(void)
{
    u8 *s0;
    u8 *s3;
    u8 *s4;
    u8 *s2;
    s32 s1;

    bzero(save_buf, 2048);
    s2 = etc_rec_row_fmt;
    s3 = D_800EEF60;
    s4 = album_work;
    s0 = screen_w;
    s1 = 0;
    do {
        printf(s2, s0[1890], s0[1891], s0[1892], s0[1888],
                      s0[1888]);
        if (now_year != 0) {
            if (now_month != 0) {
                if (now_day != 0) {
                    if ((now_girl & 0xF) == s0[1886]) {
                        *(Blk8 *)(&s4[s1 * 8] + 88) =
                            *(Blk8 *)(s0 + 1886);
                        s1 = s1 + 1;
                    }
                }
            }
        }
        s0 = s0 + 8;
    } while (s0 != s3);
    save_rec_cnt = (u8)s1;
    printf(etc_album_mode_save_fmt, opt_flag = 1);
}

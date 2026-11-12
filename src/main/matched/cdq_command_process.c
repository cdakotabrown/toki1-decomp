/* Address: 0x800457FC */
/* Size: 2852 bytes (713 instructions) */
/* Region: Game (IDO 5.2), -Wc,-pic0 (retail frame is 80; pic1 gives 96) */
/* @SymbolStart: 0x800457FC */
/* @SymbolSize: 2852 */
/* @CompileFlags: -Wc,-pic0 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 head;
    u8 tail;
    u8 flags;
    s8 phase;
} CdqCtl;
CdqCtl cdq_head = { 0 };

typedef struct {
    u8  cmd;
    u8  pad0[3];
    s32 f4;
    u32 f8;
    u8  pad1[11];
    u8  f23;
} QueueEntry;
extern QueueEntry cdq_que[];

typedef struct {
    s32 res;
    u8  status;
    u8  count;
} CdqStat;
CdqStat cdq_result = { 0 };

extern u8  cd_mode;
extern s32 cdq_cur_addr;
extern u8  cd_result_buf;
extern u8  cdq_stat_buf;

extern u8  cdq_footer_chk;
extern u16 cdq_command_process_tbl0;
extern s32 cd_read_sync_poll_tbl0;

void cdq_command_process(void)
{
    s32 pad;
    u32 top;
    u32 lo24;
    u32 mask;
    u8 buf56[8];
    u8 buf48[8];
    s32 i;
    s32 ret;
    u32 w;

    if (cdq_head.head != cdq_head.tail) {
        cd_mark_disp();
        if (cdq_head.phase == -1) {
            switch (cdq_que[cdq_head.head].cmd) {
            case 0:
                if (CdSync(1, buf48) == 0) {
                    break;
                }
                cdq_result.status = 0;
                if (CdControl(9, 0, &cdq_stat_buf) == 1) {
                    cdq_head.phase = 1;
                }
                break;
            case 3:
                SsSetMVol(127, 127);
                cdq_head.flags = 0;
                cdq_result.status = 0;
                if (CdControl(3, 0, 0) == 1) {
                    cdq_head.phase = -1;
                    cdq_head.head++;
                }
                break;
            case 13:
                if (CdSync(1, buf48) == 0) {
                    break;
                }
                cdq_head.flags = 0;
                cdq_result.status = 0;
                if (CdControl(13, &cdq_que[cdq_head.head].f8, &cdq_stat_buf) == 1) {
                    cdq_head.phase = -1;
                    cdq_head.head++;
                }
                break;
            case 9:
                if (CdSync(1, buf48) == 0) {
                    break;
                }
                cdq_head.flags = 0;
                cdq_result.status = 0;
                if (CdControl(9, 0, &cdq_stat_buf) == 1) {
                    cdq_head.phase = 1;
                }
                break;
            case 14:
                if (CdSync(1, buf48) == 0) {
                    break;
                }
                cdq_head.flags = 0;
                cdq_result.status = 0;
                if (CdControl(14, &cdq_que[cdq_head.head].f8, &cdq_stat_buf) == 1) {
                    cdq_head.phase = -1;
                    cdq_head.head++;
                }
                break;
            case 2:
                if (CdSync(1, buf48) == 0) {
                    break;
                }
                cdq_head.flags = 0;
                cdq_result.status = 0;
                CdIntToPos(cdq_que[cdq_head.head].f4, buf56);
                if (CdControl(2, buf56, &cdq_stat_buf) == 1) {
                    cdq_head.phase = -1;
                    cdq_head.head++;
                }
                break;
            case 21:
                if (CdSync(1, buf48) == 0) {
                    break;
                }
                cdq_head.flags = 0;
                cdq_result.status = 0;
                CdIntToPos(cdq_que[cdq_head.head].f4, buf56);
                if (CdControlF(21, buf56) == 1) {
                    cdq_head.phase = 1;
                }
                break;
            case 27:
                if (CdSync(1, buf48) == 0) {
                    break;
                }
                cdq_head.flags = 0;
                cdq_result.status = 0;
                CdReadyCallback(0);
                CdDataCallback(0);
                StUnSetRing();
                CdIntToPos(cdq_que[cdq_head.head].f4, buf56);
                while (CdControlB(2, buf56, 0) == 0) {
                }
                while (CdRead2(200) == 0) {
                }
                cdq_result.status = 0;
                cdq_head.phase = 1; break;
            case 10:
                cdq_head.flags = 0;
                cdq_result.status = 0;
                while (CdControl(10, 0, 0) == 0) {
                }
                cdq_head.head++;
                cdq_head.phase = -1;
                break;
            case 6:
                if (CdSync(1, buf48) == 0) {
                    break;
                }
                cdq_head.flags = 0;
                cdq_result.status = 0;
                w = cdq_que[cdq_head.head].f8;
                lo24 = (w & 0xFFFFFF) + 0x80000000;
                top = w >> 24;
                cdq_cur_addr = (w & 0xFFFFFF) + (top << 11) + 0x7FFFFFFC;
                w = lo24;
                printf(" CDQ.EXE %x\n", cdq_head.head, top);
                CdReadyCallback(0);
                CdDataCallback(0);
                cd_mode = 132;
                while (CdControlB(14, &cd_mode, 0) == 0) {
                }
                CdIntToPos(cdq_que[cdq_head.head].f4, buf56);
                while (CdControlB(2, buf56, 0) == 0) {
                }
                StUnSetRing();
                while (CdRead(top, lo24, 128) == 0) {
                }
                cdq_head.phase = 1;
                break;
            }
        } else if (cdq_head.phase == 1) {
            switch (cdq_que[cdq_head.head].cmd) {
            case 0:
                if (CdSync(1, &cdq_stat_buf) == 0) {
                    break;
                }
                if (CdControl(1, 0, &cdq_stat_buf) != 1) {
                    break;
                }
                if (cdq_stat_buf & 0x40) {
                    break;
                }
                cdq_result.status = 2;
                cdq_head.head++;
                cdq_head.phase = -1;
                break;
            case 9:
                if (CdSync(1, &cdq_stat_buf) == 0) {
                    break;
                }
                if (CdControl(1, 0, &cdq_stat_buf) != 1) {
                    break;
                }
                if (cdq_stat_buf & 0x40) {
                    break;
                }
                cdq_result.status = 2;
                cdq_head.head++;
                cdq_head.phase = -1;
                break;
            case 21:
                if (CdControl(1, 0, &cdq_stat_buf) != 1) {
                    break;
                }
                if (cdq_stat_buf & 0x40) {
                    break;
                }
                cdq_result.status = 2;
                cdq_head.head++;
                cdq_head.phase = -1;
                break;
            case 6:
                if (CdSync(1, &cdq_stat_buf) == 0) {
                    break;
                }
                ret = CdReadSync(1, &cd_result_buf);
                cd_read_sync_poll_tbl0 = ret;
                if (ret > 0) {
                    break;
                }
                if (ret == 0) {
                    if (cdq_footer_chk != 0) {
                        if ((*(u32 *)cdq_cur_addr & 0xFFFF0000) != 0x10120000) {
                            cdq_head.flags = 0;
                            printf("FOOTER ERROR");
                            cdq_command_process_tbl0++;
                            if ((cdq_command_process_tbl0 & 3) == 3) {
                                printf("RESET!");
                                CdControl(10, 0, 0);
                                for (i = 0; i != 512; i++) {
                                    VSync(0);
                                }
                            }
                            if (cdq_head.head != 0) {
                                cdq_head.head--;
                            } else {
                                cdq_head.head = 47;
                            }
                            cdq_head.phase = -1;
                        } else {
                            cdq_head.phase = -1;
                            cdq_head.head++;
                            cdq_result.status = 2;
                            cdq_head.flags |= 1;
                            cdq_result.count++;
                        }
                    } else {
                        cdq_head.phase = -1;
                        cdq_head.head++;
                        cdq_result.status = 2;
                        cdq_head.flags |= 1;
                        cdq_result.count++;
                    }
                } else if (ret < 0) {
                    if (cdq_head.head != 0) {
                        cdq_head.head--;
                    } else {
                        cdq_head.head = 47;
                    }
                    cdq_result.status = 0;
                    cdq_head.flags = 0;
                    cdq_head.phase = -1;
                }
                break;
            case 27:
                if (CdSync(1, &cdq_stat_buf) == 5) {
                    printf("SEEK_ERROR RETRY");
                    cdq_head.phase = -1;
                    cd_mode_set();
                    break;
                }
                while (CdControl(1, 0, &cdq_stat_buf) == 0) {
                }
                if ((cdq_stat_buf & 0x20) == 0) {
                    break;
                }
                cdq_head.flags = 0;
                cdq_head.phase = -1;
                if (cdq_que[cdq_head.head].f23 != 255) {
                    cdq_result.status = 4;
                }
                cdq_head.head++;
                break;
            }
        }
    } else if (CdSync(1, 0) == 5) {
        if (get_CD_l_com() == 27) {
            printf(" DISK ERROR XA STOP ");
            cdq_head.phase = -1;
            if (cdq_head.head != 0) {
                cdq_head.head--;
            } else {
                cdq_head.head = 47;
            }
            cd_mode_set();
        } else {
            printf("NOP ");
            CdControl(1, 0, 0);
        }
    } else if (cdq_result.status == 4) {
        cdq_result.res = check_head_position();
    }

    if (cdq_head.head >= 48) {
        cdq_head.head = 0;
    }
}

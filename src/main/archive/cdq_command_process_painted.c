/* Function: func_8004580C */
/* Address: 0x8004580C */
/* Size: 2940 bytes (735 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_8004580C.py */
/* CONFIRMED UNMATCHABLE FROM C (2026-03-23): */
#include "types.h"

typedef struct {
    u8  state;
    u8  pad[3];
    s32 value;
    s32 data_raw;
    s32 field12;
    s32 field16;
    u8  pad20[3];
    u8  unk23;
} StateEntry;

static u8 get_CD_l_com(void);
u8 get_CD_State2(void);
u8 get_reads(void);
u8 get_CD_State(void);

extern void cd_mark_disp(void);
extern void cd_mode_set(void);
extern s32  check_head_position(void);
extern s32  CdSync(s32 a0, void *a1);
extern s32  CdControl(s32 a0, s32 a1, void *a2);
extern void SsSetMVol(s32 a0, s32 a1);
extern void CdIntToPos(s32 a0, void *a1);
extern s32  CdControlF(s32 a0, void *a1);
extern void CdReadyCallback(s32 a0);
extern void CdDataCallback(s32 a0);
extern void StUnSetRing(void);
extern s32  CdControlB(s32 a0, void *a1, s32 a2);
extern s32  CdRead2(s32 a0);
extern void printf(void *a0);
extern void VSync(s32 a0);
extern s32  CdReadSync(s32 a0, void *a1);
extern s32  CdRead(s32 a0, s32 a1, s32 a2);

extern s8         cd_head_pos;
extern u8         cdq_head;
extern u8         cd_state2;
extern StateEntry cdq_que[48];
extern u8         cd_state;
extern u8         cd_reads;
extern u8         cdq_stat_buf[16];
extern u8         cd_result_buf[16];
extern s32        cdq_cur_addr;
extern u8         cd_mode;
extern u8         D_800B5F30[];
extern u8         D_800B5F40[];
extern u8         D_800B5F50[];
extern u8         D_800B5F58[];
extern u8         D_800B5F6C[];
extern u8         D_800B5F84[];
extern u8         cdq_footer_chk;
extern u16        cdq_command_process_tbl0;
extern s32        cd_read_sync_poll_tbl0;
extern s32        cdq_result;

void func_8004580C(void) {
    s32 sp56;
    s32 sp52;
    u8  sp40[8];
    u8  sp32[8];
    s32 sp28;

    if (cd_head_pos == -1) {
        goto special_entry;
    }

    cd_mark_disp();

    if (cd_head_pos != -1) {

        goto second_switch;
    }

    switch (cdq_que[cdq_head].state) {
    case 0:
        if (CdSync(1, sp32) == 0) { goto epilogue; }
        cd_state = 0;
        if (CdControl(9, 0, cdq_stat_buf) == 1) { cd_head_pos = 1; }
        goto epilogue;

    case 3:
        SsSetMVol(127, 127);
        cd_state2 = 0;
        cd_state = 0;
        if (CdControl(3, 0, (void *)0) == 1) {
            cdq_head++;
            cd_head_pos = -1;
        }
        goto epilogue;

    case 13:
        if (CdSync(1, sp32) == 0) { goto epilogue; }
        cd_state2 = 0;
        cd_state = 0;
        if (CdControl(13, (s32)&cdq_que[cdq_head].data_raw, cdq_stat_buf) == 1) {
            cdq_head++;
            cd_head_pos = -1;
        }
        goto epilogue;

    case 9:
        if (CdSync(1, sp32) == 0) { goto epilogue; }
        cd_state2 = 0;
        cd_state = 0;
        if (CdControl(9, 0, cdq_stat_buf) == 1) { cd_head_pos = 1; }
        goto epilogue;

    case 14:
        if (CdSync(1, sp32) == 0) { goto epilogue; }
        cd_state2 = 0;
        cd_state = 0;
        if (CdControl(14, (s32)&cdq_que[cdq_head].data_raw, cdq_stat_buf) == 1) {
            cdq_head++;
            cd_head_pos = -1;
        }
        goto epilogue;

    case 2:
        if (CdSync(1, sp32) == 0) { goto epilogue; }
        cd_state2 = 0;
        cd_state = 0;
        CdIntToPos(cdq_que[cdq_head].value, sp40);
        if (CdControl(2, (s32)sp40, cdq_stat_buf) == 1) {
            cdq_head++;
            cd_head_pos = -1;
        }
        goto epilogue;

    case 21:
        if (CdSync(1, sp32) == 0) { goto epilogue; }
        cd_state2 = 0;
        cd_state = 0;
        CdIntToPos(cdq_que[cdq_head].value, sp40);
        if (CdControlF(21, sp40) == 1) { cd_head_pos = 1; }
        goto epilogue;

    case 27:
        if (CdSync(1, sp32) == 0) { goto epilogue; }
        cd_state2 = 0;
        cd_state = 0;
        CdReadyCallback(0);
        CdDataCallback(0);
        StUnSetRing();
        CdIntToPos(cdq_que[cdq_head].value, sp40);
        while (CdControlB(2, sp40, 0) == 0) { }
        while (CdRead2(200) == 0) { }
        cd_state = 0;
        cd_head_pos = 1;
        goto epilogue;

    case 10:
        cd_state2 = 0;
        cd_state = 0;
        while (CdControl(10, 0, (void *)0) == 0) { }
        cdq_head++;
        cd_head_pos = -1;
        goto epilogue;

    case 6: {
        s32 raw;
        s32 lo24;
        if (CdSync(1, sp32) == 0) { goto epilogue; }
        cd_state2 = 0;
        raw  = cdq_que[cdq_head].data_raw;
        cd_state = 0;
        lo24 = raw & 0x00ffffff;
        sp52 = lo24 + (s32)0x80000000;
        sp56 = (s32)((u32)raw >> 24);
        cdq_cur_addr = (lo24 + sp56 * 2048) + (s32)0x7ffffffc;
        printf(D_800B5F30);
        CdReadyCallback(0);
        CdDataCallback(0);
        cd_mode = 132;
        while (CdControlB(14, &cd_mode, 0) == 0) { }
        CdIntToPos(cdq_que[cdq_head].value, sp40);
        while (CdControlB(2, sp40, 0) == 0) { }
        StUnSetRing();
        while (CdRead(sp56, sp52, 128) == 0) { }
        cd_head_pos = 1;
        goto epilogue;
    }

    default:
        goto epilogue;
    }

second_switch:
    if (cd_head_pos != 1) {
        goto epilogue;
    }
    switch (cdq_que[cdq_head].state) {
    case 0:
        if (CdSync(1, cdq_stat_buf) == 0) { goto epilogue; }
        if (CdControl(1, 0, cdq_stat_buf) != 1) { goto epilogue; }
        if (cdq_stat_buf[0] & 0x40) { goto epilogue; }
        cd_state = 2;
        cdq_head++;
        cd_head_pos = -1;
        goto epilogue;

    case 9:
        if (CdSync(1, cdq_stat_buf) == 0) { goto epilogue; }
        if (CdControl(1, 0, cdq_stat_buf) != 1) { goto epilogue; }
        if (cdq_stat_buf[0] & 0x40) { goto epilogue; }
        cd_state = 2;
        cdq_head++;
        cd_head_pos = -1;
        goto epilogue;

    case 21:
        if (CdControl(1, 0, cdq_stat_buf) != 1) { goto epilogue; }
        if (cdq_stat_buf[0] & 0x40) { goto epilogue; }
        cd_state = 2;
        cdq_head++;
        cd_head_pos = -1;
        goto epilogue;

    case 6: {
        s32 ret;
        if (CdSync(1, cdq_stat_buf) == 0) { goto epilogue; }
        ret = CdReadSync(1, cd_result_buf);
        cd_read_sync_poll_tbl0 = ret;
        if (ret > 0) { goto epilogue; }
        if (ret < 0) {
            if (cdq_head == 0) {
                cdq_head = 47;
            } else {
                cdq_head--;
            }
            cd_state = 0;
            cd_state2 = 0;
            cd_head_pos = -1;
            goto epilogue;
        }
        if (cdq_footer_chk == 0) {
            cdq_head++;
            cd_state = 2;
            cd_state2 |= 1;
            cd_reads++;
            cd_head_pos = -1;
            goto epilogue;
        }
        if ((*(s32 *)cdq_cur_addr & 0xffff0000) == 0x10120000) {
            cdq_head++;
            cd_state = 2;
            cd_state2 |= 1;
            cd_reads++;
            cd_head_pos = -1;
            goto epilogue;
        }
        cd_state2 = 0;
        printf(D_800B5F40);
        cdq_command_process_tbl0++;
        if ((cdq_command_process_tbl0 & 3) == 3) {
            printf(D_800B5F50);
            CdControl(10, 0, (void *)0);
            sp28 = 0;
            do {
                VSync(0);
                sp28++;
            } while (sp28 != 512);
        }
        if (cdq_head == 0) {
            cdq_head = 47;
        } else {
            cdq_head--;
        }
        cd_head_pos = -1;
        goto epilogue;
    }

    case 27:
        if (CdSync(1, cdq_stat_buf) == 5) {
            printf(D_800B5F58);
            cd_head_pos = -1;
            cd_mode_set();
            goto epilogue;
        }
        while (CdControl(1, 0, cdq_stat_buf) == 0) { }
        if (!(cdq_stat_buf[0] & 0x20)) { goto epilogue; }
        cd_state2 = 0;
        cd_head_pos = -1;
        if (cdq_que[cdq_head].unk23 != 255) {
            cd_state = 4;
        }
        cdq_head++;
        goto epilogue;

    default:
        goto epilogue;
    }

special_entry:

    if (CdSync(1, (void *)0) == 5) {
        if (get_CD_l_com() == 27) {
            printf(D_800B5F6C);
            cd_head_pos = -1;
            if (cdq_head == 0) {
                cdq_head = 47;
            } else {
                cdq_head--;
            }
            cd_mode_set();
        } else {
            printf(D_800B5F84);
            CdControl(1, 0, (void *)0);
        }
    } else {
        if (cd_state == 4) {
            cdq_result = check_head_position();
        }
    }

epilogue:
    if (cdq_head >= 48U) {
        cdq_head = 0;
    }
}

static u8 get_CD_l_com(void) {
    return cdq_que[((u32)cdq_head + 47U) % 48U].state;
}

u8 get_CD_State2(void) {
    return cd_state2;
}

u8 get_reads(void) {
    return cd_reads;
}

u8 get_CD_State(void) {
    return cd_state;
}

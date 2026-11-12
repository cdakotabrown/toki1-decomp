/* Address: 0x8008A1F8 */
/* Size: 804 bytes (201 instructions) */
/* @Measured: 2026-09-17 6/201 exact (3.1%), compiled 195 vs target 201 words, Fixup 0 -- SEMI */
/* @Compiler: gcc260 */
/* @CompileFlags: --fill-epilogue */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void printf(s32 *a0, s32 *a1, s32 a2, s32 a3, s32 arg4);
extern s32 CD_sync(s32 a0, u8 *a1);
extern s32 VSync(s32 a0);
extern s32 CheckCallback(void);
extern void callback(void);
extern void CD_init(void);

extern s32 CD_debug;
extern s32 CD_comstr[];
extern s32 libcd_cmd_param_count_tbl[];
extern s32 libcd_reg_index_ptr;
extern s32 libcd_reg_cmd_resp_ptr;
extern s32 libcd_reg_param_ptr;
extern u8 libcd_last_cmd;
extern s32 cd_status;
extern s32 CD_intstr;
extern s32 D_800E2B8C_arr[];
extern s32 libcd_timeout_deadline;
extern s32 libcd_timeout_spin_count;
extern s32 libcd_cur_func_name;
extern u8 libcd_sync_result_buf[];

s32 CD_cw(u8 s1, u8 *s0, u8 *s3, s32 s2) {
    s32 v0, v1, idx;

    if (CD_debug >= 2) {
        idx = s1;
        printf((s32 *)0x800B9010, (s32 *)CD_comstr[idx], 0, 0, 0);
    }

    idx = s1;
    v1 = libcd_cmd_param_count_tbl[idx];

    if (v1 != 0 && s0 == 0) {
        if (CD_debug > 0) {
            printf((s32 *)0x800B9018, (s32 *)CD_comstr[idx], 0, 0, 0);
        }
        return -2;
    }

    if (s1 == 2) {
        s32 i;
        for (i = 0; i < 4; i++) {
            ((u8 *)0x800E2B80)[i] = s0[i];
        }
    }

    CD_sync(0, 0);

    cd_status = 0;
    v1 = D_800E2B8C_arr[idx];

    if (v1 != 0) {
        CD_intstr = 0;
    }

    *(u8 *)libcd_reg_index_ptr = 0;

    if (libcd_cmd_param_count_tbl[idx] > 0) {
        s32 i = 0;
        while (i < libcd_cmd_param_count_tbl[idx]) {
            *(u8 *)libcd_reg_param_ptr = *s0;
            s0++;
            i++;
        }
    }

    libcd_last_cmd = s1;
    *(u8 *)libcd_reg_cmd_resp_ptr = libcd_last_cmd;

    if (s2 != 0) {
        return 0;
    }

    v0 = VSync(-1);
    libcd_timeout_deadline = v0 + 480;
    libcd_timeout_spin_count = 0;
    libcd_cur_func_name = (s32)0x800B9028;

    if (cd_status != 0) {

        if (s3 != 0) {
            s32 i;
            u8 *src;
            u8 *dst;
            src = libcd_sync_result_buf;
            dst = s3;
            i = 7;
            do {
                *dst++ = *src++;
                i--;
            } while (i != -1);
        }

        v0 = cd_status;
        return (v0 ^ 5) < 1 ? -1 : 0;
    }

    while (1) {
        v0 = VSync(-1);

        if (libcd_timeout_deadline >= v0) {
            libcd_timeout_spin_count++;
            if (libcd_timeout_spin_count <= 0x00F00000) {
                continue;
            }
        }

        printf((s32 *)0x800B8F90, libcd_cur_func_name,
                     CD_comstr[libcd_last_cmd],
                     D_800E2B8C_arr[cd_status],
                     D_800E2B8C_arr[CD_intstr]);
        CD_init();
        return -1;
    }

    if (CheckCallback() != 0) {
        callback();
    }

    if (cd_status == 0) {

    }

    if (s3 != 0) {
        s32 i;
        u8 *src;
        u8 *dst;
        src = libcd_sync_result_buf;
        dst = s3;
        i = 7;
        do {
            *dst++ = *src++;
            i--;
        } while (i != -1);
    }

    v0 = cd_status;
    return (v0 ^ 5) < 1 ? -1 : 0;
}

/* Function: Gslight_normal */
/* Address: 0x8009D988 */
/* Size: 448 bytes (112 instructions) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 -mhard-float --fill-epilogue */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions 0x4 0xc */
/* @PostProc: swap_instructions 0x8 0x14 */
/* @PostProc: swap_instructions 0xc 0x1c */
/* @PostProc: swap_instructions 0x10 0x1c */
/* @PostProc: swap_instructions 0x14 0x2c */
/* @PostProc: swap_instructions 0x18 0x24 */
/* @PostProc: swap_instructions 0x1c 0x28 */
/* @PostProc: swap_instructions 0x20 0x30 */
/* @PostProc: swap_instructions 0x24 0x2c */
/* @PostProc: swap_instructions 0x28 0x30 */
/* @PostProc: swap_instructions 0xa0 0xa8 */
/* @PostProc: swap_instructions 0xa8 0xb0 */
/* @PostProc: swap_instructions 0xd4 0xdc */
/* @PostProc: swap_instructions 0xdc 0xe4 */
/* @PostProc: swap_instructions 0x108 0x110 */
/* @PostProc: swap_instructions 0x110 0x118 */
/* @PostProc: swap_instructions 0x13c 0x144 */
/* @PostProc: swap_instructions 0x144 0x14c */
/* @PostProc: swap_instructions 0x194 0x198 */
/* @PostProc: swap_instructions 0x19c 0x1b0 */
/* @PostProc: swap_instructions 0x1a0 0x1b0 */
/* @PostProc: swap_instructions 0x1a4 0x1b0 */
/* @PostProc: swap_instructions 0x1a8 0x1b0 */
/* @PostProc: swap_instructions 0x1ac 0x1b0 */
#include "types.h"

extern s32 gte_scope_f3(s32*, u8*, s32*);
extern s32 gte_scope_tf3(s32*, u8*, s32*);
extern s32 gte_scope_g3(s32*, u8*, s32*);
extern s32 gte_scope_tg3(s32*, u8*, s32*);
extern void gte_flat3(u8*, s32*, s32, s32*);
extern void gte_texture_flat3(u8*, s32*, s32, s32*);
extern void gte_gouraud3(u8*, s32*, s32, s32*);
extern void gte_texture_gouraud3(u8*, s32*, s32, s32*);
extern void printf(char*, s32);

extern s32 GsOUT_PACKET_P;

extern char libgs_gte_cmd_unassigned_msg[];

s32 Gslight_normal(u8 *arg0, s32 *arg1, s32 *arg2, u32 arg3) {
    s32 result;
    u32 countTotal;
    s32 primCount;
    s32 tempVal;
    u8 type;

    result = 0;
    countTotal = 0;

    if (arg3 != 0) {
        do {
            primCount = GsOUT_PACKET_P;
            type = arg0[3] & 0xFC;

            switch (type) {
                case 0x20:
                    if (gte_scope_f3(&tempVal, arg0, arg1) != 0) {
                        result++;
                        gte_flat3(arg0, arg2, tempVal, &primCount);
                    }
                    break;
                case 0x24:
                    if (gte_scope_tf3(&tempVal, arg0, arg1) != 0) {
                        result++;
                        gte_texture_flat3(arg0, arg2, tempVal, &primCount);
                    }
                    break;
                case 0x30:
                    if (gte_scope_g3(&tempVal, arg0, arg1) != 0) {
                        result++;
                        gte_gouraud3(arg0, arg2, tempVal, &primCount);
                    }
                    break;
                case 0x34:
                    if (gte_scope_tg3(&tempVal, arg0, arg1) != 0) {
                        result++;
                        gte_texture_gouraud3(arg0, arg2, tempVal, &primCount);
                    }
                    break;
                case 0x28:
                case 0x2C:
                case 0x38:
                case 0x3C:
                    break;
                default: {
                    s32 unused[2];
                    printf(libgs_gte_cmd_unassigned_msg, arg0[0]);
                    break;
                }
            }

            GsOUT_PACKET_P = primCount;

            arg0 += ((arg0[1] << 2) + 4);

            countTotal++;
        } while (countTotal < arg3);
    }

    return result;
}

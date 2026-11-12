/* Function: Gslight_mate */
/* Address: 0x8009DD08 */
/* Size: 428 bytes (107 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --fill-epilogue */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions 0xc 0x10 */
/* @PostProc: swap_instructions 0x10 0x14 */
/* @PostProc: swap_instructions 0x14 0x18 */
/* @PostProc: swap_instructions 0x18 0x1c */
/* @PostProc: swap_instructions 0x1c 0x20 */
/* @PostProc: swap_instructions 0x20 0x28 */
/* @PostProc: swap_instructions 0x24 0x2c */
/* @PostProc: swap_instructions 0x28 0x2c */
/* @PostProc: swap_instructions 0x2c 0x30 */
/* @PostProc: swap_instructions 0x70 0x74 */
/* @PostProc: swap_instructions 0x74 0x78 */
/* @PostProc: swap_instructions 0xa8 0xac */
/* @PostProc: swap_instructions 0xac 0xb0 */
/* @PostProc: swap_instructions 0xec 0xf4 */
/* @PostProc: swap_instructions 0xf0 0xfc */
/* @PostProc: swap_instructions 0xf4 0x104 */
/* @PostProc: swap_instructions 0xf8 0x104 */
/* @PostProc: swap_instructions 0xfc 0x108 */
/* @PostProc: swap_instructions 0x140 0x148 */
/* @PostProc: swap_instructions 0x144 0x150 */
/* @PostProc: swap_instructions 0x148 0x158 */
/* @PostProc: swap_instructions 0x14c 0x158 */
/* @PostProc: swap_instructions 0x150 0x15c */
#include "types.h"

extern s32 gte_scope_f3(s32*, u8*, s32*);
extern s32 gte_scope_tf3(s32*, u8*, s32*);
extern s32 gte_scope_g3(s32*, u8*, s32*);
extern s32 gte_scope_tg3(s32*, u8*, s32*);
extern void gte_flat_mat3(u8*, u8*, s32, s32);
extern void gte_texture_flat_mat3(u8*, u8*, s32, s32);
extern void gte_gouraud_mat3(u8*, u8*, u8*, u8*, s32, s32);
extern void gte_texture_gouraud_mat3(u8*, u8*, u8*, u8*, s32, s32);
extern void printf(char*, s32);

extern s32 GsOUT_PACKET_P;
extern char libgs_gte_cmd_unassigned_msg[];

s32 Gslight_mate(u8 *arg0, s32 *arg1, u8 *arg2) {
    s32 tempVal;
    s32 primCount;
    u8 type;

    primCount = GsOUT_PACKET_P;
    type = arg0[3] & 0xFC;

    switch (type) {
        case 0x20:
            if (gte_scope_f3(&tempVal, arg0, arg1) == 0) {
                return 0;
            }
            gte_flat_mat3(arg0, arg2 + (*(u16*)(arg0 + 8) << 3), tempVal,
                          primCount);
            break;
        case 0x24:
            if (gte_scope_tf3(&tempVal, arg0, arg1) == 0) {
                return 0;
            }
            gte_texture_flat_mat3(arg0, arg2 + (*(u16*)(arg0 + 16) << 3), tempVal,
                          primCount);
            break;
        case 0x30:
            if (gte_scope_g3(&tempVal, arg0, arg1) == 0) {
                return 0;
            }
            gte_gouraud_mat3(arg0,
                          arg2 + (*(u16*)(arg0 + 8) << 3),
                          arg2 + (*(u16*)(arg0 + 12) << 3),
                          arg2 + (*(u16*)(arg0 + 16) << 3),
                          tempVal, primCount);
            break;
        case 0x34:
            if (gte_scope_tg3(&tempVal, arg0, arg1) == 0) {
                return 0;
            }
            gte_texture_gouraud_mat3(arg0,
                          arg2 + (*(u16*)(arg0 + 16) << 3),
                          arg2 + (*(u16*)(arg0 + 20) << 3),
                          arg2 + (*(u16*)(arg0 + 24) << 3),
                          tempVal, primCount);
            break;
        case 0x28:
        case 0x2C:
        case 0x38:
        case 0x3C:
            break;
        default:
            printf(libgs_gte_cmd_unassigned_msg, arg0[0]);
            return 0;
    }

    GsOUT_PACKET_P = primCount;
    return 1;
}

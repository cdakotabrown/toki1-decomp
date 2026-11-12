/* Address: 0x8009DB48 */
/* Size: 448 bytes (112 instructions) */
/* Region: Library (0x80086000-0x800BFFFF) */
/* @Compiler: gcc260 */
/* @CompileFlags: -mhard-float --fill-epilogue */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0xa0 --offset2 0xa4 */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xd8 */
/* @PostProc: swap_instructions --offset1 0x108 --offset2 0x10c */
/* @PostProc: swap_instructions --offset1 0x13c --offset2 0x140 */
/* @PostProc: swap_instructions --offset1 0x194 --offset2 0x198 */
/* @PostProc: swap_instructions --offset1 0x19c --offset2 0x1b0 */
/* @PostProc: swap_instructions --offset1 0x1a0 --offset2 0x1b0 */
/* @PostProc: swap_instructions --offset1 0x1a4 --offset2 0x1b0 */
/* @PostProc: swap_instructions --offset1 0x1a8 --offset2 0x1b0 */
/* @PostProc: swap_instructions --offset1 0x1ac --offset2 0x1b0 */
#include "types.h"

extern s32 GsOUT_PACKET_P;
extern char libgs_gte_cmd_unassigned_msg[];

extern s32 gte_scope_f3(s32 *out, u8 *evt, s32 a2);
extern s32 gte_scope_tf3(s32 *out, u8 *evt, s32 a2);
extern s32 gte_scope_g3(s32 *out, u8 *evt, s32 a2);
extern s32 gte_scope_tg3(s32 *out, u8 *evt, s32 a2);
extern void gte_flat_fog3(u8 *evt, s32 a1, s32 parsed, s32 *cursor);
extern void gte_texture_flat_fog3(u8 *evt, s32 a1, s32 parsed, s32 *cursor);
extern void gte_gouraud_fog3(u8 *evt, s32 a1, s32 parsed, s32 *cursor);
extern void gte_texture_gouraud_fog3(u8 *evt, s32 a1, s32 parsed, s32 *cursor);
extern void printf(char *fmt, s32 arg);

s32 Gslight_normal_fog(u8 *events, s32 arg1, s32 arg2, u32 count)
{
    s32 scratch;
    s32 saved;
    s32 ok;
    u32 i;

    ok = 0;
    for (i = 0; i < count; i++) {
        saved = GsOUT_PACKET_P;

        switch (events[3] & 0xFC) {
        case 0x20:
            if (gte_scope_f3(&scratch, events, arg1)) {
                gte_flat_fog3(events, arg2, scratch, &saved);
                ok++;
            }
            break;
        case 0x24:
            if (gte_scope_tf3(&scratch, events, arg1)) {
                gte_texture_flat_fog3(events, arg2, scratch, &saved);
                ok++;
            }
            break;
        case 0x28:
            break;
        case 0x2C:
            break;
        case 0x30:
            if (gte_scope_g3(&scratch, events, arg1)) {
                gte_gouraud_fog3(events, arg2, scratch, &saved);
                ok++;
            }
            break;
        case 0x34:
            if (gte_scope_tg3(&scratch, events, arg1)) {
                gte_texture_gouraud_fog3(events, arg2, scratch, &saved);
                ok++;
            }
            break;
        case 0x38:
            break;
        case 0x3C:
            break;
        default:
            printf(libgs_gte_cmd_unassigned_msg, events[0]);
            break;
        }

        GsOUT_PACKET_P = saved;
        events += events[1] * 4 + 4;
    }

    return ok;
}

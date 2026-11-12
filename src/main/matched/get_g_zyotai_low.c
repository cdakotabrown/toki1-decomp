/* Function: get_g_zyotai_low */
/* Address: 0x80086804 */
/* Size: 36 bytes (9 instructions) */
/* Region: Library (0x80086000+), but see note 1 -- this TU is game-style IDO. */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: convert_or_to_addu --exclude-offsets 0x14 */
typedef unsigned char u8;
typedef int s32;

extern s32 get_g_zyotai_s(s32);

u8 get_g_zyotai_low(s32 arg0)
{
    return (u8)(get_g_zyotai_s(arg0) & 0x7F);
}

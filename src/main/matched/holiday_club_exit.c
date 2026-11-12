/* Address: 0x80078848 */
/* Size: 164 bytes (41 instructions) */
/* Compiler: IDO 5.2 (via ido-static-recomp) */
/* Region: GCC (0x80041000-0x80080FFF) */
/* Post-processing required: patch_frame_size.py --old 24 --new 40
 * Region: GCC (0x80041000-0x80080FFF)
 *
 * Build:
 *   ./tools/ido-static-recomp/build/5.2/out/cc -O2 -G0 -mips1 -EL -non_shared -c holiday_club_exit.c -o holiday_club_exit.o
 *   python3 tools/ido/patch_frame_size.py holiday_club_exit.o func_80078848_patched.o --old 24 --new 40
 *
 * Calls class_clear_f and basyo_range_check.
 * If basyo_range_check returns 1:
 *   - Checks if (now_day + 6) / 7 == 3
 *   - Checks if now_month % 3 == 0
 *   - Checks if now_youbi == 6
 *   - If all true, calls goto_step(115), else goto_sub_step(5)
 * If basyo_range_check returns != 1:
 *   - calls goto_sub_step(5)
 */
typedef unsigned char u8;
typedef unsigned int u32;
typedef int s32;

extern u8 now_day;
extern u8 now_month;
extern u8 now_youbi;

extern void class_clear_f(void);
extern s32 basyo_range_check(void);
extern void goto_step(s32 a0);
extern void goto_sub_step(s32 a0);

void holiday_club_exit(void) {
    s32 result;
    u32 val1;
    u32 mod_result;

    class_clear_f();
    result = basyo_range_check();

    if (result == 1) {
        val1 = (u32)(now_day + 6);
        if ((val1 / 7u) == 3u) {
            mod_result = (u32)now_month % 3u;
            if (mod_result == 0) {
                if (now_youbi == 6) {
                    goto_step(115);
                    return;
                }
            }
        }
    }

    goto_sub_step(5);
}

/* Likely Symbol: none (a proximity o.bin name was retracted 2026-10-09) */
/* Address: 0x80083830 */
/* Size: 120 bytes (30 instructions) */
/* Region: IDO (0x80081000-0x800BFFFF) */
/* @CompileFlags: -Wc,-pic1 */
/* Post-processing required: Frame size patch from 32 to 48 bytes.
 * @CompileFlags: -Wc,-pic1
 */
typedef unsigned char u8;
typedef signed int s32;

extern int get_g_zyotai_h(u8 arg0);
extern void bustup_sprite_init(int a0, int a1, int a2, int a3, int stack_arg);
extern void sub_sub_step_up(void);

extern u8 now_girl;
extern int bustup_check_init_tbl0;
extern int bustup_check_init_tbl1;
extern int bustup_check_init_tbl2;
extern int bustup_check_init_tbl3;

void zyotai_sprite_set(void)
{
    int v1;
    int t6;

    t6 = get_g_zyotai_h(now_girl);
    v1 = t6;
    t6 = v1 & 0x7F;
    v1 = (unsigned char) t6;

    if (now_girl < 12u)
    {
    }
    else
    {
        v1 = 0;
    }

    bustup_sprite_init(bustup_check_init_tbl3, bustup_check_init_tbl0, bustup_check_init_tbl1, bustup_check_init_tbl2, v1);
    sub_sub_step_up();
}

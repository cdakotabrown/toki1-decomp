/* Function: bustup_face_step (bustup_face_step) */
/* Address: 0x80083540 */
/* Size: 64 bytes (16 instructions) */
/* @CompileFlags: -Wc,-pic1 */
extern unsigned char now_girl;

extern int get_g_zyotai_h(int arg0);
extern void bustup_face_change(int arg0);
extern void sub_sub_step_up(void);

void bustup_face_step(void) {
    int temp;
    unsigned char masked;

    temp = get_g_zyotai_h(now_girl);
    masked = (unsigned char)(temp & 0x7F);
    bustup_face_change((int)masked);
    sub_sub_step_up();
}

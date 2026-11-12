typedef signed short s16;

extern s16 k_sub_reset_point;

extern void k_reset(s16 arg);

void k_sub_reset(void) {
    k_reset(k_sub_reset_point);
}

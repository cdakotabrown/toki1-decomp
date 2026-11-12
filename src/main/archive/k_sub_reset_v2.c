typedef signed short s16;

extern void k_reset(s16 arg);

#define k_sub_reset_point (*(volatile s16*)0x800BA464)

void k_sub_reset(void) {
    k_reset(k_sub_reset_point);
}

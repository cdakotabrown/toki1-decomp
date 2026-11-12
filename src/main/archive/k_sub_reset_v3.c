typedef signed short s16;

extern void k_reset(s16 arg);

void k_sub_reset(void) {

    k_reset(*(s16*)0x800BA464);
}

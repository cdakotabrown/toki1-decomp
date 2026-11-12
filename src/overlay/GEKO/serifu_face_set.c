/* serifu_face_set -- GEKO overlay, Address: 0x8013375C, Size: 144 bytes (36 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 get_g_zyotai_s(s32 a0);
extern void bustup_face_step3(void);
extern void bustup_face_step4(void);
extern u8 now_girl;
extern s32 serifu_no;
void serifu_face_set(void)
{
  s32 result;
  unsigned int mode;
  result = (unsigned short) get_g_zyotai_s(now_girl);
  if ((((u32) (result & 0x7F)) < 2U) && (serifu_no != 0))
  {
    mode = now_girl;
    if (((mode == 2) || (mode == 7)) || (mode == 9))
    {
      bustup_face_step4();
    }
    else
    {
      bustup_face_step3();
    }
  }
  else
  {
    sub_sub_step_up();
  }
}

// 0x08027ed8 FUN_08027ed8 size=76

undefined4 FUN_08027ed8(uint param_1)

{
  undefined4 local_c;
  
  local_c = CONCAT13(0xff,(int3)CONCAT31(CONCAT21((short)((param_1 >> 0xb & 0x1f) * 0x107 + 7 >> 5),
                                                  (char)(((param_1 & 0x7ff) >> 5) * 0x103 + 3 >> 6))
                                         ,(char)((param_1 & 0x1f) * 0x107 + 7 >> 5)));
  return local_c;
}


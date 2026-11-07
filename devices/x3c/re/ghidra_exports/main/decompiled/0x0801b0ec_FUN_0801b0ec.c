// 0x0801b0ec FUN_0801b0ec size=102

uint FUN_0801b0ec(ushort param_1,uint param_2,uint param_3)

{
  uint uVar1;
  ushort local_10;
  
  local_10 = (ushort)param_2;
  if (param_3 < 3) {
    uVar1 = param_2 & 0xffff;
  }
  else {
    uVar1 = ((uint)((int)(short)(param_1 >> 0xb) * (int)(short)(local_10 >> 0xb)) >> 5) << 0xb |
            (uint)((int)(short)(param_1 & 0x1f) * (int)(short)(local_10 & 0x1f)) >> 5 |
            ((uint)((int)(short)((param_1 & 0x7e0) >> 5) * (int)(short)((param_2 & 0x7ff) >> 5)) >>
            6) << 5;
    if (param_3 != 0xff) {
      uVar1 = FUN_08027e60(uVar1,param_2);
      uVar1 = uVar1 & 0xffff;
    }
  }
  return uVar1;
}


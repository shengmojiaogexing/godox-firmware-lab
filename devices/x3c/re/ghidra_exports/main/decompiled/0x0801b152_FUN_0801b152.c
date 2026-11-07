// 0x0801b152 FUN_0801b152 size=112

uint FUN_0801b152(ushort param_1,uint param_2,uint param_3)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  
  if (param_3 < 3) {
    uVar1 = param_2 & 0xffff;
  }
  else {
    uVar1 = (param_2 >> 0xb & 0x1f) - (uint)(param_1 >> 0xb);
    if ((int)uVar1 < 1) {
      uVar1 = 0;
    }
    uVar2 = ((param_2 & 0x7ff) >> 5) - ((param_1 & 0xffff07ff) >> 5);
    if ((int)uVar2 < 1) {
      uVar2 = 0;
    }
    uVar3 = (param_2 & 0x1f) - (param_1 & 0x1f);
    if ((int)uVar3 < 1) {
      uVar3 = 0;
    }
    uVar1 = (uVar1 & 0x1f) << 0xb | (uVar2 & 0x3f) << 5 | uVar3 & 0x1f;
    if (param_3 != 0xff) {
      uVar1 = FUN_08027e60(uVar1,param_2);
      uVar1 = uVar1 & 0xffff;
    }
  }
  return uVar1;
}


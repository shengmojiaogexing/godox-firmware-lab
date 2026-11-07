// 0x08007330 FUN_08007330 size=26

void FUN_08007330(uint param_1)

{
  uint uVar1;
  int extraout_r1;
  uint extraout_r2;
  undefined4 extraout_r3;
  undefined4 uVar2;
  
  uVar1 = 0;
  uVar2 = DAT_0800734c;
  while (uVar1 < param_1) {
    FUN_08007294(uVar2);
    param_1 = extraout_r2;
    uVar2 = extraout_r3;
    uVar1 = extraout_r1 + 1U & 0xffff;
  }
  return;
}


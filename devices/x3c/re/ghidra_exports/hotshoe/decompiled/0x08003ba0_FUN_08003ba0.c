// 0x08003ba0 FUN_08003ba0 size=138

undefined4 FUN_08003ba0(int param_1,byte *param_2)

{
  uint *puVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  undefined4 uVar6;
  
  puVar1 = DAT_08003c2c;
  uVar6 = 0;
  if (param_1 == 0) {
    if (-1 < (int)(DAT_08003c2c[2] << 0x19)) {
      param_2[3] = 0;
    }
    iVar3 = FUN_08003a70(*param_2);
    iVar2 = FUN_08003a70(param_2[1]);
    uVar5 = FUN_08003a70(param_2[2]);
    uVar5 = iVar3 << 0x10 | iVar2 << 8 | uVar5;
    uVar4 = (uint)param_2[3] << 0x16;
  }
  else {
    if ((int)(DAT_08003c2c[2] << 0x19) < 0) {
      FUN_08003a60(*param_2);
    }
    else {
      param_2[3] = 0;
    }
    uVar5 = (uint)param_2[2] | (uint)*param_2 << 0x10;
    uVar4 = (uint)param_2[1] << 8 | (uint)param_2[3] << 0x16;
  }
  FUN_08003c88();
  iVar3 = FUN_08003cb4();
  if (iVar3 != 0) {
    *puVar1 = (uVar5 | uVar4) & DAT_08003c30;
    FUN_08003c78();
    if (((int)(puVar1[2] << 0x1a) < 0) || (iVar3 = FUN_08003e54(), iVar3 != 0)) {
      uVar6 = 1;
    }
  }
  puVar1[9] = 0xff;
  return uVar6;
}


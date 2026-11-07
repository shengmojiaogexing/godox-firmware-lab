// 0x080206d8 FUN_080206d8 size=112

undefined4 FUN_080206d8(undefined4 param_1,undefined4 param_2,undefined4 param_3,uint param_4)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  
  uVar1 = FUN_08020648(param_1);
  uVar2 = FUN_08020648(param_2);
  uVar3 = FUN_08020648(param_3);
  FUN_080207a4();
  iVar4 = FUN_08020570();
  if (iVar4 != 1) {
    return 0;
  }
  *DAT_08020748 =
       (uVar1 & 0x3f) << 0x10 | (uVar2 & 0x7f) << 8 | uVar3 & 0x7f | (param_4 & 1) << 0x16;
  FUN_080205a0();
  if (-1 < (int)(DAT_08020748[2] << 0x1a)) {
    FUN_08020774();
  }
  FUN_080207b4();
  return 1;
}


// 0x0801bb20 FUN_0801bb20 size=166

void FUN_0801bb20(uint *param_1)

{
  longlong lVar1;
  int iVar2;
  uint uVar3;
  uint *puVar4;
  
  iVar2 = FUN_0801be08();
  if (iVar2 == 0) {
    if (((int)(DAT_0801bbcc[0x26] << 0x11) < 0) && ((int)(DAT_0801bbcc[0x26] << 0x13) < 0)) {
      *param_1 = DAT_0801bbd4;
    }
    else {
      *param_1 = DAT_0801bbc8;
    }
  }
  else if (iVar2 == 1) {
    *param_1 = DAT_0801bbc8 << 1;
  }
  else if (iVar2 == 2) {
    puVar4 = DAT_0801bbcc + -1;
    uVar3 = DAT_0801bbc8;
    if ((int)(*puVar4 << 9) < 0) {
      uVar3 = DAT_0801bbc8 << 1;
    }
    lVar1 = (ulonglong)uVar3 * (ulonglong)((*puVar4 & 0x7fff) >> 6);
    uVar3 = FUN_08004b58((int)lVar1,(int)((ulonglong)lVar1 >> 0x20),
                         (uint)*(byte *)(DAT_0801bbd0 + -0x16 + ((*puVar4 & 0x7ffff) >> 0x10)) *
                         (*puVar4 & 0xf),0);
    *param_1 = uVar3;
  }
  else {
    *param_1 = DAT_0801bbc8;
  }
  puVar4 = DAT_0801bbcc;
  uVar3 = *param_1 >> *(sbyte *)(DAT_0801bbd0 + ((*DAT_0801bbcc & 0xff) >> 4));
  param_1[1] = uVar3;
  iVar2 = DAT_0801bbd0;
  param_1[3] = uVar3 >> *(sbyte *)(DAT_0801bbd0 + -0x10 + ((*puVar4 & 0x1fff) >> 10));
  param_1[2] = uVar3 >> *(sbyte *)(iVar2 + -8 + ((*puVar4 & 0xffff) >> 0xd));
  return;
}


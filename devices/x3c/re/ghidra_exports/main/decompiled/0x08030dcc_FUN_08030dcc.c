// 0x08030dcc FUN_08030dcc size=94

undefined4 FUN_08030dcc(int param_1,undefined4 param_2,uint param_3)

{
  uint uVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  
  uVar4 = 0;
  uVar1 = (*(ushort *)(param_1 + 0x2a) & 0x1ff) >> 3;
  while ((uVar4 < uVar1 &&
         ((uVar5 = *(uint *)(*(int *)(param_1 + 0xc) + uVar4 * 8 + 4), -1 < (int)(uVar5 << 7) ||
          ((uVar5 & 0xffffff) != param_3))))) {
    uVar4 = uVar4 + 1;
  }
  if (uVar1 == uVar4) {
    uVar2 = 0;
  }
  else {
    iVar3 = FUN_08032dcc(*(undefined4 *)(*(int *)(param_1 + 0xc) + uVar4 * 8),param_2);
    if (iVar3 == 0) {
      uVar2 = 0;
    }
    else {
      FUN_08030ca4(param_1,param_3,param_2);
      uVar2 = 1;
    }
  }
  return uVar2;
}


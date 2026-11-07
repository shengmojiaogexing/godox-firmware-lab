// 0x08021a30 FUN_08021a30 size=46

undefined4 FUN_08021a30(int param_1,int param_2,uint param_3)

{
  int iVar1;
  uint uVar2;
  
  uVar2 = 0;
  while( true ) {
    if (param_3 <= uVar2) {
      return 1;
    }
    iVar1 = FUN_080216a8(param_1,*(undefined1 *)(param_2 + uVar2));
    if (iVar1 != 3) break;
    param_1 = param_1 + 1;
    uVar2 = uVar2 + 1 & 0xffff;
  }
  return 0;
}


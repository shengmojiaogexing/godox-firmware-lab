// 0x0800c7f8 FUN_0800c7f8 size=134

int FUN_0800c7f8(uint param_1,int param_2,uint param_3,int param_4,uint param_5)

{
  undefined1 uVar1;
  int iVar2;
  uint uVar3;
  
  FUN_0800ca64();
  iVar2 = FUN_0800c9b8(param_1);
  if (iVar2 == 0) {
    uVar3 = 0;
    while ((uVar3 < param_3 && (iVar2 = FUN_0800c9b8(*(undefined1 *)(param_2 + uVar3)), iVar2 == 0))
          ) {
      uVar3 = uVar3 + 1 & 0xff;
    }
  }
  FUN_0801c118(5);
  FUN_0801c118(5);
  if (iVar2 == 0) {
    FUN_0800ca64();
    iVar2 = FUN_0800c9b8(param_1 | 1);
    if (iVar2 == 0) {
      for (uVar3 = 0; uVar3 < param_5; uVar3 = uVar3 + 1 & 0xff) {
        if (uVar3 == param_5 - 1) {
          uVar1 = FUN_0800c910(1);
          *(undefined1 *)(param_4 + uVar3) = uVar1;
        }
        else {
          uVar1 = FUN_0800c910(0);
          *(undefined1 *)(param_4 + uVar3) = uVar1;
        }
      }
    }
    FUN_0800cabc();
  }
  return iVar2;
}


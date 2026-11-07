// 0x0803b4ee FUN_0803b4ee size=132

void FUN_0803b4ee(int *param_1)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  
  iVar4 = *param_1;
  uVar3 = 0;
  for (uVar1 = FUN_0803a6be(iVar4); uVar1 != 0; uVar1 = uVar1 >> 1) {
    if ((uVar1 & 1) != 0) {
      uVar2 = FUN_0803a5f2(iVar4,uVar3);
      if ((uVar2 & 1) != 0) {
        FUN_0803a5e8(iVar4,uVar3,1);
        FUN_0803abb8(param_1,uVar3 & 0xff);
      }
      if ((int)(uVar2 << 0x1c) < 0) {
        FUN_0803a5e8(iVar4,uVar3,8);
        FUN_0803ac2c(param_1,uVar3 & 0xff);
        if (*(byte *)((int)param_1 + 0x2e6) != 0) {
          *(uint *)(*param_1 + 0x800) =
               *(uint *)(*param_1 + 0x800) & 0xfffff80f |
               (*(byte *)((int)param_1 + 0x2e6) & 0x7f) << 4;
          *(undefined1 *)((int)param_1 + 0x2e6) = 0;
        }
      }
      if ((int)(uVar2 << 0x1b) < 0) {
        FUN_0803a5e8(iVar4,uVar3,0x10);
      }
    }
    uVar3 = uVar3 + 1;
  }
  return;
}


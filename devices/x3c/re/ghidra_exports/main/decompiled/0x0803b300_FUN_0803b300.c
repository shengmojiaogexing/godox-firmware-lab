// 0x0803b300 FUN_0803b300 size=152

void FUN_0803b300(int *param_1)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  
  iVar4 = *param_1;
  uVar3 = 0;
  for (uVar1 = FUN_0803a6b0(iVar4); uVar1 != 0; uVar1 = uVar1 >> 1) {
    if ((uVar1 & 1) != 0) {
      uVar2 = FUN_0803a4ae(iVar4,uVar3);
      if ((uVar2 & 1) != 0) {
        *(uint *)(iVar4 + 0x834) = *(uint *)(iVar4 + 0x834) & ~(1 << (uVar3 & 0xff));
        FUN_0803a4a4(iVar4,uVar3,1);
        FUN_0803a9f8(param_1,uVar3 & 0xff);
      }
      if ((int)(uVar2 << 0x1c) < 0) {
        FUN_0803a4a4(iVar4,uVar3,8);
      }
      if ((int)(uVar2 << 0x1b) < 0) {
        FUN_0803a4a4(iVar4,uVar3,0x10);
      }
      if ((int)(uVar2 << 0x19) < 0) {
        FUN_0803a4a4(iVar4,uVar3,0x40);
      }
      if ((int)(uVar2 << 0x1e) < 0) {
        FUN_0803a4a4(iVar4,uVar3,2);
      }
      if ((int)(uVar2 << 0x18) < 0) {
        FUN_0803a8d4(param_1,uVar3);
      }
    }
    uVar3 = uVar3 + 1;
  }
  return;
}


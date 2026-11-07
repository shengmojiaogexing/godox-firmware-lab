// 0x0802b8b2 FUN_0802b8b2 size=70

void FUN_0802b8b2(int param_1,uint param_2)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  
  if (param_1 != 0) {
    if (-((int)((uint)*(byte *)(param_1 + 0x1c) << 0x1e) >> 0x1f) != param_2) {
      *(byte *)(param_1 + 0x1c) = *(byte *)(param_1 + 0x1c) & 0xfd | (byte)((param_2 & 1) << 1);
      iVar1 = FUN_0802b754(param_1);
      if (iVar1 != 0) {
        uVar2 = FUN_08022204(param_1);
        iVar3 = FUN_0802ad74(**(undefined4 **)(param_1 + 0xc),0xe,uVar2);
        if (iVar3 == 1) {
          FUN_080302d0(iVar1);
          return;
        }
      }
    }
  }
  return;
}


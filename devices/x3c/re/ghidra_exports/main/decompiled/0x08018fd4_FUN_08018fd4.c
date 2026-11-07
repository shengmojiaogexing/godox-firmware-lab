// 0x08018fd4 FUN_08018fd4 size=132

void FUN_08018fd4(int param_1)

{
  short sVar1;
  uint uVar2;
  undefined4 uVar3;
  int iVar4;
  
  uVar2 = (uint)*(byte *)(param_1 + 0x42);
  if (-1 < (int)(uVar2 << 0x1e)) {
    sVar1 = *(short *)(param_1 + 0x40);
    if ((sVar1 != 0) && (sVar1 != -1)) {
      *(short *)(param_1 + 0x40) = sVar1 + -1;
    }
  }
  iVar4 = *(int *)(param_1 + 0x38);
  if ((*(short *)(param_1 + 0x40) == 0) && ((iVar4 == 0 || ((int)(uVar2 << 0x1e) < 0)))) {
    FUN_0801801e(DAT_08019058,param_1);
    FUN_08018fa8();
    if (*(code **)(param_1 + 0xc) != (code *)0x0) {
      (**(code **)(param_1 + 0xc))(param_1);
    }
    if (*(code **)(param_1 + 0x10) != (code *)0x0) {
      (**(code **)(param_1 + 0x10))(param_1);
    }
    FUN_0802d6e4(param_1);
    return;
  }
  *(int *)(param_1 + 0x30) = -*(int *)(param_1 + 0x3c);
  if (iVar4 != 0) {
    if (-1 < (int)(uVar2 << 0x1e)) {
      *(int *)(param_1 + 0x30) = -*(int *)(param_1 + 0x34);
    }
    *(byte *)(param_1 + 0x42) =
         *(byte *)(param_1 + 0x42) & 0xfd | (byte)((((int)uVar2 >> 1) + 1U & 1) << 1);
    uVar3 = *(undefined4 *)(param_1 + 0x20);
    *(undefined4 *)(param_1 + 0x20) = *(undefined4 *)(param_1 + 0x28);
    *(undefined4 *)(param_1 + 0x28) = uVar3;
    uVar3 = *(undefined4 *)(param_1 + 0x2c);
    *(int *)(param_1 + 0x2c) = iVar4;
    *(undefined4 *)(param_1 + 0x38) = uVar3;
  }
  return;
}


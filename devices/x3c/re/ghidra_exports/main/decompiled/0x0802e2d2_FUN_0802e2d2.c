// 0x0802e2d2 FUN_0802e2d2 size=122

void FUN_0802e2d2(int param_1,uint param_2)

{
  int iVar1;
  int iVar2;
  undefined1 auStack_30 [16];
  undefined1 auStack_20 [16];
  
  iVar1 = FUN_08030384(param_1);
  if ((int)(param_2 << 0x1b) < 0) {
    FUN_0802f0ec(param_1,auStack_20,auStack_30);
    FUN_08030308(param_1,auStack_20);
    FUN_08030308(param_1,auStack_30);
  }
  *(uint *)(param_1 + 0x24) = *(uint *)(param_1 + 0x24) & ~param_2;
  if ((param_2 & 1) != 0) {
    FUN_080302d0(param_1);
    iVar2 = FUN_08030384(param_1);
    if (iVar2 != 0) {
      FUN_0802ee0a(param_1);
      FUN_08030436();
      FUN_08030436(param_1);
    }
  }
  iVar2 = FUN_08030384(param_1);
  if ((iVar2 == iVar1) && ((param_2 & 0x1800000) == 0)) {
    return;
  }
  FUN_0802ee0a(param_1);
  FUN_08030436();
  return;
}


// 0x08031248 FUN_08031248 size=72

void FUN_08031248(undefined4 param_1)

{
  int iVar1;
  undefined1 auStack_28 [16];
  undefined1 auStack_18 [16];
  
  FUN_0802f0ec(param_1,auStack_18,auStack_28);
  iVar1 = FUN_080274b0(auStack_18);
  if ((iVar1 != 0) || (iVar1 = FUN_080274b0(auStack_28), iVar1 != 0)) {
    iVar1 = FUN_080274b0(auStack_18);
    if (iVar1 != 0) {
      FUN_08030308(param_1,auStack_18);
    }
    iVar1 = FUN_080274b0(auStack_28);
    if (iVar1 != 0) {
      FUN_08030308(param_1,auStack_28);
    }
  }
  return;
}


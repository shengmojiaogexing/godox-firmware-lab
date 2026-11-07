// 0x080282e6 FUN_080282e6 size=40

undefined4 FUN_080282e6(int *param_1)

{
  undefined4 *puVar1;
  uint uVar2;
  
  if (param_1 == (int *)0x0) {
    param_1 = (int *)FUN_0802821c();
  }
  if (param_1 == (int *)0x0) {
    return 0;
  }
  puVar1 = (undefined4 *)*param_1;
  uVar2 = (*(byte *)(puVar1 + 7) & 0x3f) >> 4;
  if ((uVar2 == 1) || (uVar2 == 3)) {
    return *puVar1;
  }
  return puVar1[1];
}


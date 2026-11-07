// 0x0803c15c FUN_0803c15c size=72

longlong FUN_0803c15c(uint param_1,int param_2)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  
  uVar1 = (uint)((param_1 & 0x80000000) != 0) << 0x1f;
  uVar2 = (param_1 & 0x7fffffff) >> 3;
  uVar4 = param_1 << 1 ^ param_1;
  if (uVar4 != 0) {
    param_1 = param_1 << 0x1d;
    param_2 = (uVar1 | uVar2) + 0x38000000;
  }
  if (uVar4 != 0 && (uVar4 & 0x7f000000) != 0) {
    return CONCAT44(param_2,param_1);
  }
  if ((uVar2 & 0x8000000) == 0) {
    return (ulonglong)uVar1 << 0x20;
  }
  iVar3 = FUN_0803c3aa(uVar1 | param_1 >> 0x1d | uVar2 << 3,param_2);
  return (ulonglong)((uint)(iVar3 < 0) << 0x1f | 0x7ff00000) << 0x20;
}


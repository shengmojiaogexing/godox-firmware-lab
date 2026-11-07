// 0x08036d66 FUN_08036d66 size=84

void FUN_08036d66(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = FUN_0802ed10(param_1);
  if (iVar1 == 0) {
    return;
  }
  iVar2 = *(int *)(iVar1 + 0x10);
  if (iVar2 != 0) {
    FUN_0802ad74(iVar2,0x25,0);
  }
  if (*(int *)(iVar1 + 0x10) != 0) {
    FUN_0802ad74(param_1,0x26,0);
  }
  *(int *)(iVar1 + 0x10) = param_1;
  if (param_1 != 0) {
    FUN_0802ad74(param_1,0x27,0);
  }
  if (*(int *)(iVar1 + 0x10) != 0) {
    FUN_0802ad74(iVar2,0x28,0);
  }
  FUN_080302d0(param_1);
  return;
}


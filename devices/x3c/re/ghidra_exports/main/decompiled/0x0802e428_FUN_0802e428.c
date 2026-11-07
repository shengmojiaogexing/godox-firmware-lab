// 0x0802e428 FUN_0802e428 size=114

void FUN_0802e428(int param_1)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  
  FUN_080302d0(param_1);
  iVar2 = FUN_0802ee0a(param_1);
  if (iVar2 != 0) {
    FUN_08031248(iVar2);
  }
  iVar3 = 0;
  bVar1 = false;
  if (iVar2 == 0) {
    iVar3 = FUN_0802ed10(param_1);
    if (iVar3 == 0) {
      return;
    }
    if (*(int *)(iVar3 + 0x10) == param_1) {
      bVar1 = true;
    }
  }
  FUN_08035d4c(param_1);
  if (iVar2 != 0) {
    FUN_080317f8(iVar2);
    FUN_080305d2(iVar2,0);
    FUN_08031248(iVar2);
    FUN_0802ad74(iVar2,0x22,0);
    FUN_0802ad74(iVar2,0x24,0);
  }
  if (bVar1) {
    *(undefined4 *)(iVar3 + 0x10) = 0;
  }
  return;
}


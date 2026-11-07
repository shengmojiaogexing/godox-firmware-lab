// 0x0802add2 FUN_0802add2 size=24

void FUN_0802add2(int param_1)

{
  int iVar1;
  int iVar2;
  undefined8 uVar3;
  
  if (*(char *)(param_1 + 8) == '\x13') {
    uVar3 = FUN_0802ad6c();
    iVar1 = (int)((ulonglong)uVar3 >> 0x20);
    iVar2 = *(int *)uVar3;
    if (iVar1 < iVar2) {
      iVar1 = iVar2;
    }
    *(int *)uVar3 = iVar1;
  }
  return;
}


// 0x0803ae84 FUN_0803ae84 size=40

void FUN_0803ae84(int *param_1)

{
  int iVar1;
  
  iVar1 = FUN_0803a8ca(*param_1);
  if (iVar1 == 1) {
    *(uint *)(*param_1 + 0xe00) = *(uint *)(*param_1 + 0xe00) | 1;
    return;
  }
  return;
}


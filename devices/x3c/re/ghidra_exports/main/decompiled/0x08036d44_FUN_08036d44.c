// 0x08036d44 FUN_08036d44 size=34

void FUN_08036d44(undefined4 *param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_0802ed10(*param_1);
  uVar2 = FUN_0803204c();
  *(undefined4 *)(iVar1 + 0x14) = uVar2;
  uVar2 = *param_1;
  *(undefined4 *)(iVar1 + 0x10) = uVar2;
  FUN_0802ad74(uVar2,0x26,0);
  return;
}


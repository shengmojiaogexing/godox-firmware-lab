// 0x08036cf0 FUN_08036cf0 size=84

void FUN_08036cf0(undefined4 *param_1)

{
  int iVar1;
  
  iVar1 = FUN_0802ed10(*param_1);
  FUN_0802ad74(*(undefined4 *)(iVar1 + 0x10),0x27,0);
  FUN_0802ad74(*(undefined4 *)(iVar1 + 0x14),0x28,0);
  if ((*(int *)(iVar1 + 0x14) != 0) && ((int)((uint)*(byte *)(iVar1 + 0x28) << 0x1e) < 0)) {
    FUN_0802e428();
  }
  *(undefined4 *)(iVar1 + 0x14) = 0;
  *(byte *)(iVar1 + 0x28) = *(byte *)(iVar1 + 0x28) & 0xfe;
  *(undefined4 *)(iVar1 + 0x18) = 0;
  FUN_08030dcc(*param_1,0x60,0);
  FUN_080302d0(*(undefined4 *)(iVar1 + 0x10));
  return;
}


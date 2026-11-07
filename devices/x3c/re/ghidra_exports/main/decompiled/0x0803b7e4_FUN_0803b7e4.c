// 0x0803b7e4 FUN_0803b7e4 size=46

void FUN_0803b7e4(undefined4 *param_1,uint param_2)

{
  int iVar1;
  
  if ((int)(param_2 << 0x18) < 0) {
    iVar1 = (param_2 & 0x7f) * 0xb + 3;
  }
  else {
    iVar1 = (param_2 & 0x7f) * 0xb + 0x5b;
  }
  FUN_0803a602(*param_1,param_1 + iVar1);
  *(undefined1 *)((int)(param_1 + iVar1) + 0xd) = 1;
  return;
}


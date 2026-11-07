// 0x0800c7c0 FUN_0800c7c0 size=56

int FUN_0800c7c0(undefined4 param_1,int param_2,uint param_3)

{
  uint uVar1;
  int iVar2;
  
  uVar1 = 0;
  iVar2 = 0;
  FUN_0800ca64();
  FUN_0800c9b8(param_1);
  while ((uVar1 < param_3 && (iVar2 = FUN_0800c9b8(*(undefined1 *)(param_2 + uVar1)), iVar2 == 0)))
  {
    uVar1 = uVar1 + 1 & 0xff;
  }
  FUN_0800cabc();
  return iVar2;
}


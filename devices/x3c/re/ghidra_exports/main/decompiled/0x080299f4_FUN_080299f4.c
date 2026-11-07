// 0x080299f4 FUN_080299f4 size=28

undefined4 FUN_080299f4(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = DAT_08029a10;
  uVar2 = 0;
  if (param_1 != -1) {
    uVar2 = *(undefined4 *)(DAT_08029a10 + param_1 * 8);
    *(undefined4 *)(DAT_08029a10 + param_1 * 8) = 0;
    *(undefined4 *)(iVar1 + param_1 * 8 + 4) = 0;
  }
  return uVar2;
}


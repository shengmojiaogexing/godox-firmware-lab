// 0x0803aaa8 FUN_0803aaa8 size=266

undefined4 FUN_0803aaa8(int *param_1,int param_2,int param_3,int param_4)

{
  uint uVar1;
  int iVar2;
  uint *puVar3;
  
  *param_1 = param_2;
  puVar3 = (uint *)(param_2 + 0x800);
  *(undefined1 *)(param_1 + 0xb9) = 1;
  *(undefined1 *)((int)param_1 + 0x2e6) = 0;
  param_1[1] = param_3;
  param_1[2] = param_4;
  FUN_0803ad46(param_1);
  FUN_0803a454(param_1);
  FUN_0803a830(param_2);
  FUN_0803a6d8(param_2);
  FUN_0803a74c(param_2,0);
  FUN_0803a844(*param_1);
  *puVar3 = *puVar3 & 0xffffe7ff;
  *puVar3 = *puVar3 | 3;
  FUN_0803a684(param_2,0x10);
  FUN_0803a660(param_2);
  *(undefined4 *)(param_2 + 0x818) = 0xffffffff;
  *(undefined4 *)(param_2 + 0x81c) = 0;
  *(undefined4 *)(param_2 + 0x810) = 0;
  *(undefined4 *)(param_2 + 0x814) = 0;
  uVar1 = 0;
  do {
    *(undefined4 *)(param_2 + uVar1 * 4 + 0x104) = 0;
    uVar1 = uVar1 + 1;
  } while (uVar1 < 8);
  FUN_0803b0c0(param_1);
  uVar1 = 0;
  do {
    iVar2 = param_2 + uVar1 * 0x20;
    puVar3 = (uint *)(iVar2 + 0x900);
    if ((int)*puVar3 < 0) {
      *puVar3 = 0;
      *puVar3 = *puVar3 | 0x40000000;
      *puVar3 = *puVar3 | 0x8000000;
    }
    else {
      *puVar3 = 0;
    }
    *(undefined4 *)(iVar2 + 0x910) = 0;
    *(undefined4 *)(iVar2 + 0x908) = 0xff;
    uVar1 = uVar1 + 1;
  } while (uVar1 < 8);
  uVar1 = 0;
  do {
    iVar2 = param_2 + uVar1 * 0x20;
    puVar3 = (uint *)(iVar2 + 0xb00);
    if ((int)*puVar3 < 0) {
      *puVar3 = 0;
      *puVar3 = *puVar3 | 0x40000000;
      *puVar3 = *puVar3 | 0x8000000;
    }
    else {
      *puVar3 = 0;
    }
    *(undefined4 *)(iVar2 + 0xb10) = 0;
    *(undefined4 *)(iVar2 + 0xb08) = 0xff;
    uVar1 = uVar1 + 1;
  } while (uVar1 < 8);
  *(uint *)(param_2 + 0x810) = *(uint *)(param_2 + 0x810) | 0x100;
  *(undefined4 *)(param_2 + 0x18) = 0;
  *(undefined4 *)(param_2 + 0x14) = 0xbfffffff;
  *(undefined4 *)(param_2 + 0x18) = DAT_0803abb4;
  FUN_0803a9ec(param_1);
  FUN_0803a83a(param_2);
  return 0;
}

